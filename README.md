# Composites Curing Oven Controller

For the composites oven team at Portland State. 

v0 - the original schematic representation of the current working breadboard setup.  
v1 - the finished Arduino shield version on a printed circuit board. this works!  
v2 - additions to the v1 Arduino shield design.  
v3 - developing final version for OSHPark order.

**Note**  
On March 6th the oven controller is now being worked on in the mme-capstone repo in the PSAS organization: https://github.com/psas/mme-capstone

### Version 3

v3 is the final version of the oven controller installed on a 4’×8’×4’ curing oven at Portland State in the Mechanical Engineering department.

The blue case is 3D-printed and gets attached to the side of the oven. The electronics are a three-board stack with an Arduino Uno R3 on the bottom; a custom oven control board set up in the middle with screw-in Euroterminals for start switch, relay, and thermocouple wires; and a 1.8” TFT LCD Shield from Adafruit for the display on top.

The display allows the user to program up to nine steps in a temperature profile. Each step lets the user specify a ramp rate, hold temperature, and hold time. Once all the steps are set, the user should set the relay enable switch to ON and then tell the program to begin running the profile.

The program automatically detects which thermocouples are present and uses the temperature as input to a PID loop which controls ramp rate by turning on and off up to four relays which are, for our configuration, controlling two heaters, one fan, and one vent.

<img src="http://wickerbox.net/img/ovencontrol/067_1.JPG">

<img src="http://wickerbox.net/img/ovencontrol/068_1.JPG">

<img src="http://wickerbox.net/img/ovencontrol/088_1.JPG">

<img src="http://wickerbox.net/img/ovencontrol/IMG_1836.JPG">
