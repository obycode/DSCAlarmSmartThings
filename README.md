## AD2SmartThings
### ===================
### Integrate your Honeywell/Ademco alarm panel into your SmartThings physical graph

Do you have a home alarm panel and never use it?  Do you hate the hassle of remember to arm and having to run around to disarm?  

This is a relatively simple integration project that effectively integrates your Honeywell or Ademco alarm panel with SmartThings.  These panels are typically installed by ADT or other alarm companies.  You can choose to continue or not with your ADT  monitoring service.  This project works either way.  

After installing this project, you can control your alarm panel automagically!!!!!
* Have it arm automatically after everyone has left the house by integrating with your presence sensors!  And have it disarm when you arrive.
* Have it turn on automatically when you go to bed and turn off automatically when you wake by integrating with your Goodnight! automation and your GoodMorning! automation


Date: 8/20/2014

### Supported Alarm Panels
http://www.alarmdecoder.com/wiki/index.php/Supported_Panels

### Required hardware
------------------
* AD2Pi from www.alarmdecoder.com
* Arduino MEGA
* Arduino SmartThings shield
* Jumper wires

### Required software
-----------------
* Arduino IDE (>v1)
* Libraries and sketch in the Arduino folder in this repository
* SmartThings Device Type source code in this repository

### SetUp
-----------------

### Credits
--------
* Timer library from Simon Monk
* Thanks to authors: Vassilis Varveropoulos, Brice Dobry for the ArduinoMega ThingShield library and intial ideas
* Lots of good ideas from SmartThings @craig whose repository can be found at https://gist.github.com/e5b30109fdaec805d474.git



## Background

Why would you build a lawn sprinkler system controller when you can just buy one?  But you can’t just buy one... at least not the sprinkler system we were thinking about!!  Let’s start by first making the controller connected.  And of course, we will need iPhone controls.  Then we need to make it smart.  Smart like it knows when it has rained, is raining or will rain!   And then what if we could add cognition so that it actually learns how to water your lawn!  Not that would be really smart!!!

## High Level Project Steps

1. Order the hardware
2. Add SmartThings hub to your home network, download app to your iPhone
3. Obtain a Maker/Developer account for SmartThings (graph.api.smartthings.com)
4. Assemble the Arduino Mega, ThingShield and AD2Pi
5. Download the Arduino developer environment and import the AD2SmartThings sketch as well as the required libraries, including the SmartThingsMega and the Timer library.  
6. Add ArduinoMega/ThingShield to your SmartThings hub using your iPhone app
7. Go to graph.api.smartthings.com
  1. On My Device Types, create a new device type and paste in the device type code.  Save & Publish
8.  Go to My Devices, select the newly added Arduino and edit the Device Type and select the HomeAlarm device type (7.i.)
9.  Test out system 

## The Hardware
The AD2Pi was obtained from Alarm Decoder.  www.alarmdecoder.com
All other items from this project were easily obtained from Amazon via Amazon Prime

* AD2Pi
* Arduino MEGA
* Arduino SmartThings shield
* Jumper wires
* 9V power adapter

### An ArduinoMega with SmartThing ThingShield

<img src="https://cloud.githubusercontent.com/assets/5625006/4343989/ec207020-406e-11e4-99fc-15962e7c93e3.jpg" width="200px"  />

<img src="https://cloud.githubusercontent.com/assets/5625006/4343995/50e33b5a-406f-11e4-9dd5-1b73ab5f8e7f.jpg" width="200px"  />


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

Before stacking, bend the pin0 and pin1 of the ThingShield out to a 45 degree angle so they do not insert into the Arduino Mega.  See pictures.  

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

### Arduino Code 

To load the code onto the Arduino, you will need the Arduino developer environment:

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

The Main Tile gives the status of the system and allows you to switch on the alarm in "stay" mode.
Inside the device type:

There is a 2x2 StatusTile that is the same as the main tile
There are tiles to switch on the alarm in Stay and in Away mode
There is a tile to toggle the chime
And there are status tiles for up to 6 zones

A message tile displays all the messages sent from the AD2Pi

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
* Lots of good ideas from SmartThings @craig whose repository can be found at https://gist.github.com/e5b30109fdaec805d474.git

