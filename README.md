## AD2SmartThings v2.0
### ===================
### Integrate your Honeywell or Ademco alarm panel into your SmartThings physical graph
ADT / Honeywell / Ademco / Vista / Alarm / SmartThings / Alarm Panel / Home Alarm

Do you have a home alarm panel and never use it?  Do you hate the hassle of remember to arm and having to run around to disarm?  

This is a relatively simple integration project that effectively integrates your Honeywell or Ademco alarm panel with SmartThings.  These panels are typically installed by ADT or other alarm companies.  You can choose to continue or not with your ADT  monitoring service.  This project works either way.  

After installing this project, you can control your alarm panel automagically!!!!!
* Have it arm automatically after everyone has left the house by integrating with your presence sensors!  And have it disarm when you arrive.
* Have it turn on automatically when you go to bed and turn off automatically when you wake by integrating with your Goodnight! automation and your GoodMorning! automation
* Control your alarm panel via a MiniMote or activate from other sensors or devices


Date: Feb 8, 2015
This is Version 2.0 release.  To use this release you have to install or update both the Device Type v2.0 and the Arduino Sketch 2.0.  This release is not backwards compatable with V1.X code.  

## High Level Project Steps

1. Order the hardware
2. Add SmartThings hub to your home network, download app to your iPhone
3. Obtain a Maker/Developer account for SmartThings (graph.api.smartthings.com)
4. Assemble the Arduino Mega, ThingShield and AD2Pi
5. Download the Arduino developer environment and import the AD2SmartThings sketch as well as the required libraries, including the SmartThingsMega and the Timer library.  
6. Add ArduinoMega/ThingShield to your SmartThings hub using your iPhone app
7. Go to graph.api.smartthings.com
  1. On My Device Types, create a new device type and paste in the device type code.  Save & Publish
8.  Go to My Devices, select the newly added Arduino and edit the Device Type and select the HomeAlarm device type (step 7.i.)
9.  Configure preferences on your mobile device.
9.  Thoroughly est out system!  No Warranty (see license terms).

## The Hardware List
The AD2Pi was obtained from Alarm Decoder.  www.alarmdecoder.com
All other items from this project were easily obtained from Amazon via Amazon Prime

* AD2Pi
* Arduino MEGA
* Arduino SmartThings shield
* Jumper wires
* 9V power adapter

### An ArduinoMega, SmartThing ThingShield and AD2Pi

<img src="https://cloud.githubusercontent.com/assets/5625006/4343989/ec207020-406e-11e4-99fc-15962e7c93e3.jpg" width="200px"  />

<img src="https://cloud.githubusercontent.com/assets/5625006/4343995/50e33b5a-406f-11e4-9dd5-1b73ab5f8e7f.jpg" width="200px"  />

<img src="https://cloud.githubusercontent.com/assets/5625006/4344057/36bc5f3c-4073-11e4-8ef3-446b950dc4ab.jpg" width="200px"  />


### Wiring

For the wiring, I used individual male to male jumper cables to wire the project (see below).  I found these at Amazon such as **Male to Male Solderless Flexible Breadboard Jumper Cable Wires 65Pcs for Arduino by Sunkee**.

<img src="https://cloud.githubusercontent.com/assets/5625006/4343969/333ffca2-406d-11e4-95ef-287c4dfa869b.jpg" width="200px"  />


### Power Supplies
For a power supply to power up the ArduinoMega I used a 9V power supply to run the Arduino+ThingShield.  This was  purchased at Amazon as well by searching for "Arduino Power Supply".  Right now, I am using the Super Power Supply® AC / DC Adapter Charger Cord Plug - 9V 650mA compatible with Arduino Freeduino Duemilanove Uno Mega Hobby Electronics, which was available by Amazon Prime.   CAUTION: There are 9V power supplies availble on Amazon that do not work for the Arduino (they are made for musical instrament controllers) and some that perform very poorly on Arduino.  Be sure to read the reviews!  

### Project Housing
I simply strapped the Arduino onto a foam board and inserted it into my alarm panel box


### Wiring the Project

* Wiring diagrams are included in the header of the Arduino sketch.


### Wiring The Arduino Controller

An **Arduino Mega* was used as the controller and was stacked with the SmartThings ThingShield.  *Note, set the DIP switch on the ThingShield  to D2/D3 if not already*.

Before stacking the ThingShield on top of the ArduinoMega, bend the pin0 and pin1 of the ThingShield out to a 45 degree angle so they do not insert into the Arduino Mega.  See pictures.  

<img src="https://cloud.githubusercontent.com/assets/5625006/4343966/e3bd10b6-406c-11e4-9120-7113a8e746bd.jpg" width="200px"  />   

<img src="https://cloud.githubusercontent.com/assets/5625006/4343981/87f90e4a-406e-11e4-96f1-d8fed5d3be1d.jpg" width="200px"  />

<img src="https://cloud.githubusercontent.com/assets/5625006/4343985/9f6a7b0e-406e-11e4-8e9c-8be488631336.jpg" width="200px"  />

<img src="https://cloud.githubusercontent.com/assets/5625006/4343982/91aef3a0-406e-11e4-8bc7-2fc97bf38a93.jpg" width="200px"  />


Note, the ThingShield pins are not labeled.  So you can identify the pins using labels on the ArduinoMega picture (above).

7. Connect the Arduino to USB power or to a 9V power supply using the appropriate ports.  
8. When not using the USB power supply, place a piece of black electrical tape over the USB port to prevent accidental shorting

### Wiring The AD2Pi to your Honeywell or Ademco alarm panel

There is excellent documentation on the Alarm Decoder web site, including an instructional video.  The wiring is the same for the AD2Pi as the AD2USB.

http://www.alarmdecoder.com/wiki/index.php/Panel_Configuration

## The Software

The code for this project is in a few files which are posted on github:

https://github.com/d8adrvn/ad2smartthings.git

### Arduino Sketch 

To load the sketch onto the Arduino, you will need the Arduino developer environment:

http://arduino.cc/en/main/software

Once the software is installed, the first thing to do is obtain the required libraries.  

* Timer library was created by Simon Monk as modified by JChristensen.  A copy is included in this repository/release for your convenience.
* SmartThings Mega library contained in this repository/release
* SoftwareSerial library was default library provided with Arduino IDE
 
Once you have the zip files downloaded you can import them within the Arduino IDE. Go to the Sketch:Import Library;Add Library drop down menu. Once you have added the libraries, they will show up under Sketch:Add Library:Contributed as "Timer" and "SmartThingsMega".  Be sure the Timer library is installed named as "Timer"

You can connect the Arduino Uno to your computer via an USB cable, create a new sketch, paste the code from github into the Arduino IDE and then transfer to tehe ArduinoMEGA

Pairing instructions for the Arduino to the SmartThings hub can be found at SmartThings.com and are copied here:

“To join the shield to your SmartThings hub, go to “Add SmartThings” mode in the
SmartThings app by hitting the “+” icon in the desired location, and then press the Switch button on the shield. You should see the shield appear in the app.

To unpair the shield, press and hold the Switch button for 6 seconds and release. The shield will now be unpaired from your SmartThings Hub.”

Once you have finished transfering the code to the Arduino, you can remove the USB and power the Arduino using a 9V transformer.

Its more than a good idea to put a piece of electrical tape over the USB port to prevent accidental grounding of the port!

### Device Type Code 

The device type code allows you to control the ArduinoMega via the SmartThings physical graph.  The  controller device type code has the following features:

The Main Tile gives the status of the system and allows you to disarm the system (by pressing the tile).

Inside the device type:

<img src="https://cloud.githubusercontent.com/assets/5625006/6100596/3478d29e-afd8-11e4-9dfe-2945dfd7754a.PNG" width="200px"  />

* StatusTile -  Same as the main tile.  Shows status of system
* Stay - Activates the system in Stay node
* Away - Activates the system in Away mode
* Chime - toggles chime mode
* PANIC - a tile to activate the PANIC alarm.  To prevent false alrams, you must press this tile 3X. After pressing once, the tile will remind you to press twice more.  After pressing twice, the tile will remind you to press once more.  After pressing a thrid time, the PANIC alarm will sound.  Note, for this to work, you must set the Preferences to indicate which Panic code has been configured for your alarm.  If in doubt, try "B".  And test the system!
*Preferences - allows you to set various preferences.  You must enter your homeowner security code and you must select one of three choices for the Panic code.  
* Message Tile - displays all the messages sent from the AD2Pi

* Preferences - This allows you to configure the device type for your alarm panel.  You must enter your 4 digit homeowners alarm access code.  For the PANIC tile to work, you must select one of the three choices for your Panic code. This code must be valid for your system.  If in doubt, try "B" and then test your system
* 
<img src="https://cloud.githubusercontent.com/assets/5625006/6100607/4f67cdee-afd8-11e4-88ff-6df233021bb9.PNG" width="200px"  />

* Configure AD2Pi - this tile sends configuration commands to AD2Pi.  Enter the command in the Preferences section and then send the command to the AD2Pi by pressing this tile.  The main purpose is to set the AD2Pi with a valid address.  

<img src="https://cloud.githubusercontent.com/assets/5625006/6100610/67d0130a-afd8-11e4-89e6-4f03e2801406.PNG" width="200px"  />


* Zones - there are status tiles for up to 36 zones.  If you need fewer zones, you can simply trim the number of zones displayed by modifying the <details> command in the code.  Its that simple.   

<img src="https://cloud.githubusercontent.com/assets/5625006/6100601/47d9f9b2-afd8-11e4-8151-a38d3ad222e0.PNG" width="200px"  />


To install the device type code: 

1.  Go to graph.api.smartthings.com   
2.  Select My Device Types tab
3.  Select the +NewSmartDevice button
4.  Choose "From Code" tab
5.  Paste the device type code from this repository
6.  Go to the line of code with the "details" arguments
7.  Edit the number of zones in the details argument list to match the number of zones in your system.
8.  Save and Publish (for me) the device type.  You have to Publish!
9.  Go to My Devices tab 
10.  Select your Arduino and using the drop down, select your newly created AD2SmartThings device type (v2 or later).
11.  Go to your mobile device and the Arduino tile should now display as a Home Alarm tile.  Hint: on the iPhone, sometimes you have to kill the SmartThings app two times before a new device type update will display on the SmartThings iPhone app.

## Integration of Home Alarm Into Automated Actions:

The device type also establishes a switch and lock capability.  You can integrate your alarm panel into SmartApps and into Actions using these capabilities.

To automate turning on/off the Alarm in "Stay" mode, select the Home Alarm from the list of "switches" in your SmartThings device list

To To automate turning on/off the Alarm in "Away" mode, select the Home Alarm from the list of "locks" in your SmartThings device list.  "Locking" the alarm turns it on in Away Mode while "Un-Locking" turns it off

Have fun integrating!

## FAQs

* After installing the Device Type, be sure to both Save AND Publish (for me).  If you do not publish, they will not be able to communicate with the hub.  If you make changes to either code, Publish early and often to avoid IDE issues.

* The Arduino 9V power supply (transformer) can be purchased at Amazon by searching for "Arduino Power Supply".  CAUTION: There are 9V power supplies availble on Amazon that do not work for the Arduino (they are made for musical instrament controllers) and some that perform very poorly on Arduino.  Be sure to read the reviews!  

##Credits

* Timer library from Simon Monk
* Thanks to authors: Vassilis Varveropoulos, Brice Dobry for the ArduinoMega ThingShield library and intial ideas
* Lots of good ideas from SmartThings @craig 
* Thanks to Sean Matthews from AlarmDecoder for jumping in to help out with configuring device address, setting up Panic codes, and other help.
