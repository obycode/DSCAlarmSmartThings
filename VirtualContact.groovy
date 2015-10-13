/**
 *  Virtual Contact Sensor
 *
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
  definition (name: "Virtual Contact Sensor", namespace: "com.obycode", author: "obycode") {
    capability "Contact Sensor"
        command "open"
        command "close"
  }

  simulator {
    // TODO: define status and reply messages here
  }

  tiles(scale: 2) {
    multiAttributeTile(name:"contact", type: "generic", width: 6, height: 4, canChangeIcon: true){
      tileAttribute ("device.contact", key: "PRIMARY_CONTROL") {
        attributeState "open", label: '${name}', icon:"st.contact.contact.open", backgroundColor:"#ffa81e"
        attributeState "closed", label:'${name}', icon:"st.contact.contact.closed", backgroundColor:"#79b821"
      }
    }

    main "contact"
    details(["contact"])
  }
}

// parse events into attributes
def parse(String description) {
  log.debug "Virtual Contact Parsing '${description}'"
    // initialize to closed state
    if (description == "updated") {
      sendEvent(name: "contact", value: "closed")
    }
}

def open() {
  sendEvent(name: "contact", value: "open")
}

def close() {
  sendEvent(name: "contact", value: "closed")
}
