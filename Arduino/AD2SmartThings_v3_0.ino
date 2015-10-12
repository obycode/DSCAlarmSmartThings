 
/** 
 * AD2SmartThings v3.5.0
 * Couple your Ademco/Honeywell Alarm to your SmartThings Graph using an AD2PI, an Arduino and a ThingShield
 * The Arduino passes all your alarm messages to your SmartThings Graph where they can be processed by the Device Type
 * Use the Device Type to control your alarm or use SmartApps to integrate with other events in your home graph
 *
 *
 ****************************************************************************************************************************
 * Libraries:
 * Timer library was created by Simon Monk as modified by JChristensen  https://github.com/JChristensen/Timer.  Note: if you
 * download the library from the source, you must rename the zip file to "Timer" before importing into the Arduino IDE.
 * 
 * An enhanced SmartThings Library was created by  Dan G Ogorchock & Daniel J Ogorchock and their version is required for this implementation.
 * Their enhanced library can found at:
 * https://github.com/DanielOgorchock/ST_Anything/tree/master/Arduino/libraries/SmartThings
 *
 * SoftwareSerial library was default library provided with Arduino IDE
 *
 *
 ****************************************************************************************************************************
 *
 * Pin Configuration for AD2Pi to Arduino Mega
 * Use standard jumper wires to connect:
 *  Jumper   AD2PI   Mega
 *    GND    6       GND
 *   3.3V    1       3.3V
 *    RX     10	     19
 *    TX     8	      18
 *
 * Pin Configuration for Arduino Mega to ThingSheild
 * Use standard jumper wires to connect:
 *  Jumper      Mega  ThingShield
 *    TX        14        2   
 *    RX        15        3
 *    
 *
 *
 * Credit: thanks to github contributor vassilisv for the intial idea and to AlarmDecoder.com for suggesting to use
 * serial out feature of the AD2Pi to connect to the Arduino card.  This project also benefitted imenseley from code 
 * shared by SmartThings contributor  @craig
 * 
 * Thanks to Dan G Ogorchock & Daniel J Ogorchock for the updated SmartThings library.  This library is required for the ThingShield
 * to use the hardware serial port on the Mega and for general performance enhancements.
 */

#include <SoftwareSerial.h>  
#include <SmartThings.h>  //be sure you are using the library from ST_ANYTHING which has support for hardware serial on the Arduino Mega
#include <Timer.h>

#define PIN_LED       13
#define BUFFER_SIZE   300 // max message length from ADT
#define numZones      36 // Set the number of zones in your system

SmartThings smartthing(HW_SERIAL3, messageCallout);  //constructor for hardware serial port with ST_Anything Library

// set global variables
char buffer[BUFFER_SIZE];  //stores characters in AD2Pi to build up a message
boolean isDebugEnabled=false;  //set to true to debug
//For debugging purposes you can turn on different levels of logging:
//1 (Low Verbosity) - Zone and Command messages that are sent to SmartThings
//2 (Medium Verbosity) - Low Verbosity messages, AD2PI configuration from SmartThings, print out of all zone status after each fault
//3 (High Verbosity) - Low and Medium Verbosity messages, raw data messages from AD2PI
int debugVerbosity=1; 
int bufferIdx;  //counts characters as they come in from AD2Pi
String previousChimeStatus;
String previousAlarmStatus;
int lastZone;  //stores the last zone number to fault to compare as faults are cycled by system
int zoneStatusList[numZones + 1]; //stores each zone's status.  Adding 1 to numZones since element 0 will be a count of faulted zones and elements greater than 0 will equote to specific zone number

Timer t;
  
void setup()
{
  // initialize AD2 serial stream
  Serial1.begin(115200);                
  bufferIdx = 0;

  //debug stuff
  if (isDebugEnabled) {
    Serial.begin(9600);         // setup serial with a baud rate of 9600
    Serial.println("setup..");  // print out 'setup..' on start
  }
  // set SmartThings Shield LED
  smartthing.shieldSetLED(0, 0, 0); // shield led is off
  
  //set device to periodically check in
  //this is part of algorithm to reduce excessive logging to the hub
  t.every(10L*60L*1000L,sendUpdate); //send update every 10min
  
  //initialize array counter and zones to 0.  0 = inactive, 1 = active
  for (int i = 0; i < (numZones + 1); i = i + 1) {
    zoneStatusList[i] = 0;
  }
  lastZone = 0;
}
 
void loop()
{
  char data;
  // run smartthing logic
  smartthing.run();
  // capture IT-100 messages
  if(Serial1.available() > 0) {  
    data = Serial1.read();   
    if (isDebugEnabled) {
      Serial.print(data);
    }
    // if end of message then evaluate and send to the cloud
    if (data == '\r' && bufferIdx > 0) { 
      if (isDebugEnabled) {
      Serial.println(); //for debug purposes only sends carriage return at end of message from AD2Pi
      }
      processAD2();
    }
    // otherwise continue build array from message (ignore \n)
    else if (data != '\n')  {
      buffer[bufferIdx] = data; 
      bufferIdx++;
      // check for buffer overruns
      if (bufferIdx >= BUFFER_SIZE) 
      {
        smartthing.send("ER:Buffer overrun"); 
        bufferIdx = 0;
      }
    }
  }
  t.update(); 
}    

//Process AD2 messages
void processAD2() {
  // create String object
  buffer[bufferIdx] = '\0'; //adds null at end of buffer
  bufferIdx = 0; // reset  counter for next message
  String str(buffer);
  
  if (str.indexOf("!RFX:") >= 0 || str.indexOf("!EXP:") >= 0 || str.equals("!>null")) {
    // do nothing
    if (isDebugEnabled && debugVerbosity >= 2) {
      Serial.println("Skipping SmartThings Update - Found !RFX or !EXP or !>null: " + str);
    }
  } else if (str.indexOf("!CONFIG>ADDRESS=") >= 0) {
    smartthing.send(str.substring(8,18));
    if (isDebugEnabled && debugVerbosity == 3) {
      Serial.println("SmartThings Update - Config Command Processed: " + str);
    }
    delay (3000);
  } else if (str.indexOf("!Sending.done") >= 0) {
    smartthing.send(str);
    if (isDebugEnabled && debugVerbosity == 3) {
      Serial.println("SmartThings Update - Config Command Processed: " + str);
    }
  } else {
    //The following was gathered from http://www.alarmdecoder.com/wiki/index.php/Protocol
    String rawPanelCode = getValue(str, ',', 0);
    //During exit now messages sometimes an extra [ appears at the beginning of the rawPanelCode, remove it if found
    rawPanelCode.replace("[[", "[");
    String chimeStatus = (rawPanelCode.substring(9,10) == "1") ? "chimeOn" : "chimeOff";
    String zoneString = getValue(str, ',', 1);
    int zoneNumber = zoneString.toInt();
    String rawPanelBinary = getValue(str, ',', 2);
    String keypadMsg = getValue(str, ',', 3);
    //During exit now messages sometimes the alarm messages run together and there are 2 messages in one line.  The follow code detects that sitation and extracts the message.
    //Example: [0011000100000000----],017,[f70000071017008008020000000000],"A[0011000100000000----],016,[f70000071016008008020000000000],"ARMED ***STAY***May Exit Now  16"
    if (keypadMsg.indexOf("[") >= 0 && keypadMsg.indexOf("]") >= 0) {
      keypadMsg = getValue(str, ',', 6);
    }
    keypadMsg.replace("\"", "");
    keypadMsg.trim();
    
    String alarmStatus;
    if (rawPanelCode.substring(2,3) == "0" && rawPanelCode.substring(3,4) == "0") {
      alarmStatus = "disarmed";
    } else if (rawPanelCode.substring(2,3) == "1") {
        alarmStatus = "armedAway";
    } else if (rawPanelCode.substring(3,4) == "1") {
      alarmStatus = "armedStay";
    }

    String faultList;
    
    if (isDebugEnabled && debugVerbosity >= 1) {
      if (debugVerbosity == 3) {
        Serial.print("rawPanelCode: " + rawPanelCode + ", ");
        Serial.print("rawPanelBinary: " + rawPanelBinary + ", ");
      }
      Serial.print("chimeStatus: " + chimeStatus + ", ");
      Serial.print("alarmStatus: " + alarmStatus + ", ");
      Serial.print("zoneString: " + zoneString + ", ");
      Serial.println("keypadMsg: " + keypadMsg);
    }
    
    if (chimeStatus != previousChimeStatus) {
      smartthing.send("CHIMESTATUS:" + chimeStatus);
      previousChimeStatus = chimeStatus;
      if (isDebugEnabled && debugVerbosity >= 1) {
        Serial.println("Chime status changed to " + chimeStatus + ", updating SmartThings");
      }
    }
    
    if (keypadMsg.length() > 0) {
      if (keypadMsg.indexOf("Exit Now") >= 0) {
        alarmStatus.replace("armed", "arming");
      }
      
      if (alarmStatus != previousAlarmStatus) {
        smartthing.send("ALARMSTATUS:" + alarmStatus);
        previousAlarmStatus = alarmStatus;
        delay (3000);
        smartthing.send(keypadMsg);
        if (isDebugEnabled && debugVerbosity >= 1) {
          Serial.println("Alarm status changed to " + alarmStatus + ", updating SmartThings: " + keypadMsg);
        }
      }
      
      if (keypadMsg.indexOf("Hit * for faults") >= 0) {
        String sendCommand = "***";
        Serial1.println(sendCommand);  //send AD2Pi the command to pass on to Alarm Panel
        if (isDebugEnabled && debugVerbosity >= 1) {
          Serial.println("Faults were not displayed, Sent AD2Pi: " + sendCommand);
        } 
      } else if (alarmStatus.indexOf("arming") >= 0 || keypadMsg.indexOf("DISARMED") >= 0) {
        if (zoneStatusList[0] > 0) {
          faultList = getActiveList(1, numZones + 1);
          lastZone = 0;
          smartthing.send(keypadMsg + "|" + faultList);
          if (isDebugEnabled && debugVerbosity >= 1) {
            if (alarmStatus.indexOf("arming") >= 0) {
              Serial.println("Message contains Exit Now - faultList: " + faultList);
            } else {
              Serial.println("Message contains disarmed - faultList: " + faultList);
            }
          }
        } else {
          smartthing.send(keypadMsg);
          if (isDebugEnabled && debugVerbosity >= 1) {
            if (alarmStatus.indexOf("arming") >= 0) {
              Serial.println("Message contains Exit Now - no faults skipping SmartThings Update");
            } else {
              Serial.println("Message contains disarmed - no faults skipping SmartThings Update");
            }
          }
        }
      } else {
        if (zoneStatusList[zoneNumber] == 0) {
          //New Fault, mark active and send to SmartThings
          lastZone = zoneNumber;
          zoneStatusList[zoneNumber] = 1;
          zoneStatusList[0] = zoneStatusList[0] + 1;
          //Send only 1 new fault since others were previously sent
          smartthing.send(keypadMsg + "|" + zoneNumber);
          if (isDebugEnabled && debugVerbosity >= 1) {
            Serial.println("New fault detected: " + String(zoneNumber));
            if (debugVerbosity >= 2) {
              printArray(zoneStatusList, numZones + 1);
            }
          }
        } else {  
          if (zoneNumber == lastZone && zoneStatusList[0] == 1) {
            //Do nothing: Only 1 fault repeating
            if (isDebugEnabled && debugVerbosity >= 1) {
              Serial.println("Fault repeating: zoneNumber(" + String(zoneNumber) + ") == lastZone(" + String(lastZone) + ")");
            }
          } else if (zoneNumber == lastZone && zoneStatusList[0] > 1) {
            //Faults(s) dropped from list.  Gather a list of those zones and mark inactive.
            faultList = getActiveList(1, numZones + 1);
            //Since we don't know what zone remove current zone and set back to active in zoneStatusList array and update counter
            faultList.replace(String(zoneNumber) + ",", "");
            zoneStatusList[zoneNumber] = 1;
            zoneStatusList[0] = zoneStatusList[0] + 1;
            smartthing.send("ZONES NOW INACTIVE: " + String(faultList) + "|" + String(faultList));
            if (isDebugEnabled && debugVerbosity >= 1) {
              Serial.println("zoneNumber(" + String(zoneNumber) + ") == lastZone(" + String(lastZone) + "): Faults Dropped from list and marked inactive: " + String(faultList));
            }
          } else if (zoneNumber < lastZone) {
            //Fault list starting over, determine if any faults dropped from list between zone 1 and current zone
            faultList = getActiveList(1, zoneNumber);
            if (faultList != "") {
              smartthing.send("ZONES NOW INACTIVE: " + String(faultList) + "|" + String(faultList));
              if (isDebugEnabled && debugVerbosity >= 1) {
                Serial.println("zoneNumber(" + String(zoneNumber) + ") < lastZone(" + String(lastZone) + "): Faults Dropped from list and marked inactive: " + String(faultList));
              }
            }
            lastZone = zoneNumber;
          } else if (zoneNumber > lastZone) {
            //Fault list progressing, determine if any faults dropped from list between previous and current zone
            faultList = getActiveList(lastZone + 1, zoneNumber);
            if (faultList != "") {
              smartthing.send("ZONES NOW INACTIVE: " + String(faultList) + "|" + String(faultList));
              if (isDebugEnabled && debugVerbosity >= 1) {
                Serial.println("zoneNumber(" + String(zoneNumber) + ") > lastZone(" + String(lastZone) + "): Faults Dropped from list and marked inactive: " + String(faultList));
              }
            }
            lastZone = zoneNumber;
          }
        }
      }
    } else {
      smartthing.send(str);
      if (isDebugEnabled && debugVerbosity >= 2) {
        Serial.println("keypadMsg length = 0, sent the following to SmartThings: " + str);
      }
    }
  }
}

void messageCallout(String message) { 
  String code;
  String cmd;
  if(message.length() > 0) { //avoids processing ping from hub
    Serial.println("Received from hub:" + message);
    String code = message.substring(0,6);
    if (isDebugEnabled && debugVerbosity >= 2) {
      Serial.println("code =" + code);
    }
    String cmd = message.substring(6);
    if (isDebugEnabled && debugVerbosity >= 2) {
      Serial.println("cmd =" + cmd);
    }
    if (code.equals("[CODE]")) {
      Serial1.println(cmd);  //send AD2Pi the command to pass on to Alarm Panel
      if (isDebugEnabled && debugVerbosity >= 2) {
        Serial.println("Sent AD2Pi: " + cmd);
      }
    }
    if (code.equals("[CONF]")) {
      Serial1.println("C" + cmd);  //send configuration command to AD2Pi
      if (isDebugEnabled && debugVerbosity >= 2) {
        Serial.println("Sent AD2Pi: C" + cmd);
      }
    }
    else if (code.equals("[FUNC]")) {
      if (isDebugEnabled && debugVerbosity >= 2) {
        Serial.println("Sending AD2Pi ASCII:" + cmd);
      }
      if (cmd.equals("A")) {
        Serial1.write(1);
        Serial1.write(1);
        Serial1.write(1);
      }
      else if (cmd.equals("B")) {
        Serial1.write(2);
        Serial1.write(2);
        Serial1.write(2);
      }
      else if (cmd.equals("C")) {
        Serial1.write(3);
        Serial1.write(3);
        Serial1.write(3);
      }
    }
  }
}

String getValue(String data, char separator, int index) {
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i) == separator || i == maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

String getActiveList(int start, int end) {
  String faultList;
  for (int i = start; i < end; ++i) {
    if (zoneStatusList[i] == 1) {	
      faultList = faultList + String(i) + ",";
      zoneStatusList[i] = 0;
    }
  }
  //Update Count in element 0
  int faultCount = 0;
  for (int i = 1; i < numZones + 1; ++i) {
    if (zoneStatusList[i] == 1) {	
      faultCount = faultCount + 1;
    }
  }
  zoneStatusList[0] = faultCount;
  if (isDebugEnabled && debugVerbosity >= 2) {
    printArray(zoneStatusList, numZones + 1);
  }
  return faultList;
}

void printArray(int *a, int n) {
  for (int i = 0; i < n; i++) {
    if (i == 0) {
      Serial.print("Count:");
    } else {
      Serial.print(String(i) + ":");
    }
    Serial.print(a[i], DEC);
    Serial.print(' ');
  }
  Serial.println();
}

void sendUpdate() {  // periodically have the Arduino send the alarm panel status
  smartthing.send("AD2ST Checkin");
}


