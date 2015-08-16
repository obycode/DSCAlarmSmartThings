/**
*  AlarmThing
*
*  Author: obycode
*   based on work by Josh Foshee and vassilisv
*  Date: 2015-01-18
*  Copyright 2014 obycode
*
*  Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
*  in compliance with the License. You may obtain a copy of the License at:
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
*  Unless required by applicable law or agreed to in writing, software distributed under the License is distributed
*  on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the License
*  for the specific language governing permissions and limitations under the License.
*
*/

metadata {
  // Automatically generated. Make future change here.
  definition (name: "AlarmThing", namespace: "com.obycode", author: "obycode") {
    capability "Alarm"
    capability "Switch"

    attribute "alarmStatus", "string"
    attribute "zoneChanged", "string"

    command "armAway"
    command "armStay"
    command "disarm"
    command "clear"
    command "update"
    command "chimeToggle"
    command "panic"
  }

  // Simulator metadata
  simulator {
    // status messages
    status "ping": "catchall: 0104 0000 01 01 0040 00 6A67 00 00 0000 0A 00 0A70696E67"
    status "hello": "catchall: 0104 0000 01 01 0040 00 0A21 00 00 0000 0A 00 0A48656c6c6f20576f726c6421"
  }

  // UI tile definitions
  tiles {
    standardTile("alarmStatus", "device.alarmStatus", width: 2, height: 2, canChangeIcon: false, canChangeBackground: false) {
      state "ready", label: 'Ready', action: "armAway", icon: "st.Home.home2", backgroundColor: "#ffffff"
      state "disarmed", label: 'Ready', action: "armAway", icon: "st.Home.home2", backgroundColor: "#ffffff"
      state "notready", label: 'Not Ready', icon: "st.Home.home2", backgroundColor: "#ffa81e"
      state "away", label: 'Away', action: "disarm", icon: "st.Home.home3", backgroundColor: "#add8e6"
      state "stay", label: 'Stay', action: "disarm", icon: "st.Home.home4", backgroundColor: "#0000ff"
      state "arming", label: 'Arming', action: "disarm", icon: "st.Home.home2", backgroundColor: "#B8B8B8"
      state "alarm", label: 'Alarm', action: "clear", icon: "st.Home.home2", backgroundColor: "#ff0000"
    }
    standardTile("away", "device.switchAway", width: 1, height: 1, canChangeIcon: false, canChangeBackground: false) {
      state "on", label: "Away", action: "armAway", icon: "st.Home.home3", backgroundColor: "#add8e6"
      state "off", label: "Away", icon: "st.Home.home3", backgroundColor: "#ffffff"
    }
    standardTile("stay", "device.switchStay", width: 1, height: 1, canChangeIcon: false, canChangeBackground: false) {
      state "on", label: "Stay", action: "armStay", icon: "st.Home.home4", backgroundColor: "#0000ff"
      state "off", label: "Stay", icon: "st.Home.home4", backgroundColor: "#ffffff"
    }
    standardTile("chime", "device.chime", width:1, height: 1, canChangeIcon: false, canChangeBackground: false) {
      state "chimeOff", label:'Chime', action:'chimeToggle', icon:"st.secondary.off", backgroundColor: "#ffffff"
      state "chimeOn", label:'', action:'chimeToggle', icon:"st.secondary.beep", backgroundColor: "#ffffff"
    }
    standardTile("panic", "device.panic", width: 1, height: 1, canChangeIcon: false, canChangeBackground: true) {
      state "panic", label:'Panic', action:"panic", icon:"st.alarm.alarm.alarm", backgroundColor:"#ff0000"
      //state "confirm", label:'Confirm', action: "panic", icond:"st.alarm.alarm.alarm", backgroundColor:"#ff6600"
      //state "cancel", label:'Cancel', action: "disarm", icond:"st.alarm.alarm.alarm", backgroundColor:"#ff6600"
    }
    standardTile("refresh", "device.refresh", inactiveLabel: false, width: 1, height: 1, canChangeIcon: false, canChangeBackground: false) {
      state "default", action:"update", icon:"st.secondary.refresh"
    }
    main(["alarmStatus"])
    details(["alarmStatus","away","stay","chime","refresh","panic"])
  }
}

// Parse incoming device messages to generate events
def parse(String description) {
  log.debug description
  def msg = zigbee.parse(description)?.text
  log.debug "Received ${msg}"
  def result = []
  if (!msg || msg == "ping") {
    result += createEvent(name: null, value: msg)
  } else if ( msg.length() >= 4 ) {
    if ( msg.substring(0, 2) == "RD" ) {
      if (msg[3] == "0") {
        result += createEvent(name: "alarmStatus", value: "notready")
        // When status is "Not Ready" we cannot arm
        result += createEvent(name: "switchAway", value: "off")
        result += createEvent(name: "switchStay", value: "off")
      }
      else {
        result += createEvent(name: "alarmStatus", value: "ready")
        // When status is "Ready" we can arm
        result += createEvent(name: "switchAway", value: "on")
        result += createEvent(name: "switchStay", value: "on")
        result += createEvent(name: "switch", value: "off")
        result += createEvent(name: "panic", value: "off")
      }
    // Process arm update
    } else if ( msg.substring(0, 2) == "AR" ) {
      if (msg[3] == "0") {
        result += createEvent(name: "alarmStatus", value: "disarmed")
        result += createEvent(name: "switch", value: "off")
      }
      else if (msg[3] == "1") {
        if (msg[4] == "0" | msg[4] == "2") {
          result += createEvent(name: "alarmStatus", value: "away")
          result += createEvent(name: "switch", value: "on")
        }
        else if (msg[4] == "1" | msg[4] == "3") {
          result += createEvent(name: "alarmStatus", value: "stay")
          result += createEvent(name: "switch", value: "on")
        }
      }
      else if (msg[3] == "2") {
        result += createEvent(name: "alarmStatus", value: "arming")
        result += createEvent(name: "switch", value: "on")
      }
    // Process alarm update
    } else if ( msg.substring(0, 2) == "AL" ) {
      if (msg[3] == "1") {
        result += createEvent(name: "alarmStatus", value: "alarm")
      }
      // Process chime update
    } else if ( msg.substring(0, 2) == "CH" ) {
      if (msg[3] == "1")
        result += createEvent(name: "chime", value: "chimeOn")
      else
        result += createEvent(name: "chime", value: "chimeOff")
    // Process zone update
    } else if ( msg.substring(0, 2) == "ZN" ) {
      if (msg.substring(3, 6) == "609") {
        log.debug "sending zone opened ${msg.substring(6, 9)}"
        result += createEvent(name: "zoneChanged", value: "${msg.substring(6, 9)}.open")
      }
      else if (msg.substring(3, 6) == "610") {
        log.debug "sending zone closed ${msg.substring(6, 9)}"
        result += createEvent(name: "zoneChanged", value: "${msg.substring(6, 9)}.close")
      }
    }

  }

  log.debug "Parse returned:"
  result.each {
    log.debug "    ${it.descriptionText}"
  }
  return result
}

// Implement "switch" (turn alarm on/off)
def on() {
  armStay()
}

def off() {
  disarm()
}

// Commands sent to the device
def armAway() {
  log.debug "Sending arm command"
  zigbee.smartShield(text: "armAway").format()
}

def armStay() {
  log.debug "Sending arm stay command"
  zigbee.smartShield(text: "armStay").format()
}

def disarm() {
  log.debug "Sending disarm command"
  zigbee.smartShield(text: "disarm").format()
}

def strobe() {
  panic()
}

def siren() {
  panic()
}

def both() {
  panic()
}

def chimeToggle() {
  log.debug "Toggling chime"
  zigbee.smartShield(text: "chimeOn").format()
}

def panic() {
  log.debug "Sending panic command"
  zigbee.smartShield(text: "panic").format()
}

// TODO: Need to send off, on, off with a few secs in between to stop and clear the alarm
def clear() {
  disarm()
}

def update() {
  log.debug "Sending update command"
  zigbee.smartShield(text: "update").format()
}

def configure() {
  update()
}
