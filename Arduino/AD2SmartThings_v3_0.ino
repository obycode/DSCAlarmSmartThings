 
/** 
 * AD2SmartThings v3.0.0
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
 *    TX     8	     18
 *
 * Pin Configuration for Arduino Mega to ThingSheild
 * Use standard jumper wires to connect:
 *  Jumper      Mega  ThingShield
 * TX        14        2   
 * RX        15        3
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
#define PIN_LED         13


#define BUFFER_SIZE 300 // max message length from ADT

SmartThings smartthing(HW_SERIAL3, messageCallout);  //constructor for hardware serial port with ST_Anything Library

// set global variables
char buffer[BUFFER_SIZE];  //stores characters in AD2Pi to build up a message
boolean isDebugEnabled=false;  //set to true to debug
int bufferIdx;  //counts characters as they come in from AD2Pi
String previousStr;  //used as part of algorithm to minimize excessive logging
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
}
 
void loop()
{
  char data;
  // run smartthing logic
  smartthing.run();
  // capture IT-100 messages
  if(Serial1.available() >0) {  
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
 
   //check to see if string message is different
   if (str.equals(previousStr))
      {
      // do nothing to avoid excessive logging to SmartThings hub
      }
    else
    {
      previousStr = str;     
      int quoteIndex = str.indexOf('"');
      String cmd = str.substring(quoteIndex, str.length());
      if (str.indexOf("!RFX:") >=0 || str.equals("!>null")) {
        // do nothing
      }
      else if(cmd.length() > 0)
      {
        smartthing.send(cmd); 
      }
      else if (str.indexOf("!CONFIG>ADDRESS=") >=0)
      {
        smartthing.send(str.substring(8,18));
        delay (3000);
      }
      else
      {
        smartthing.send(str); 
      }
    }
  }

void messageCallout(String message)
{ 
  String code;
  String cmd;
  if(message.length()>0) //avoids processing ping from hub
  {
    Serial.println("Received from hub:" + message);
    String code = message.substring(0,6);
    if (isDebugEnabled) {
      Serial.println("code =" + code);
    }
    String cmd = message.substring(6);
    if (isDebugEnabled) {
      Serial.println("cmd =" + cmd);
    }
    if (code.equals("[CODE]")) {
      Serial1.println(cmd);  //send AD2Pi the command to pass on to Alarm Panel
      if (isDebugEnabled) {
        Serial.println("Sent AD2Pi: "+cmd);
      }
    }
    if (code.equals("[CONF]")) {
      Serial1.println("C" + cmd);  //send configuration command to AD2Pi
      if (isDebugEnabled) {
        Serial.println("Sent AD2Pi: C" + cmd);
      }
    }
    else if (code.equals("[FUNC]")) {
      if (isDebugEnabled) {
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
void sendUpdate()
// periodically have the Arduino send the alarm panel status
{
  previousStr="";
}


