  /*************************************************** 
  On/ Off control 1 thermocouples 
 ****************************************************/

#include "Adafruit_MAX31855.h"

#define OFF 0
#define ON  1

// Pins connection
int swReset       = 0;
int thermoCS1     = 1;
int thermoCS2     = 2;
int thermoCS3     = 3;
int sdCS          = 4;
int heater1       = 5;
int heater2       = 6;
int thermoCS4     = 7;
int lcdDC         = 8;
int thermoCS5     = 9;
int lcdCS         = 10;
int spiMOSI       = 11;
int spiMISO       = 12;
int spiCLK        = 13;
int swStart       = A0;
int fan           = A1;
int vent          = A2;
int joyRead       = A3;

// Indicate status variables: 0 off; 1: on
int start      =  OFF;
int stopnow    =  ON;

// constant
double tempSet         =  350;        // farenheit
double tempInc         =  3 ;         // Temperature increament per minutes
double tempErrRange    =  0;
double tempThreshold   =  3;          // Temperature limit to turn on both heaters
int ADCThreshold       =  900;
int timeDelay          =  5000;
float timeMax          =  12780;      // (sec) timeMax=time_ramping+time_at_350; (set at 2 hours), initial temp: 70*F

// calibration 

double sensor1  =  0;
double sensor2  =  -4;
double sensor3  =  0;
double sensor4  =  0;
double sensor5  =  0;

// variables
double tempErr;
double tempSensor1;
double tempSensor2;
double tempSensor3;
double tempSensor4;
double tempSensor5;
double tempAverage;
double tempInitial;         // room temp

double currTempSet;
double tempSetLow;
double tempSetHigh;

double startTime;
double currTime;

// Thermocouples
Adafruit_MAX31855 thermocouple1(spiCLK, thermoCS1, spiMISO);
Adafruit_MAX31855 thermocouple2(spiCLK, thermoCS2, spiMISO);
Adafruit_MAX31855 thermocouple3(spiCLK, thermoCS3, spiMISO);
Adafruit_MAX31855 thermocouple4(spiCLK, thermoCS4, spiMISO);
Adafruit_MAX31855 thermocouple5(spiCLK, thermoCS5, spiMISO);

void all_heaters_off()
{
  digitalWrite(heater1, LOW);  // turn off the heaters
  delay(100);
  digitalWrite(heater2, LOW);  // turn off the heaters
  Serial.println("  OFF");  
}

void one_heater_off()
{
  digitalWrite(heater1, LOW);  // turn off the heaters
  delay(100);
  Serial.println("  1 OFF");  
}

void all_heaters_on()
{
  digitalWrite(heater1, HIGH);  // turn on the heaters
  delay(100);
  digitalWrite(heater2, HIGH);  // turn on the heaters
  delay(100);
  Serial.println("  ON");  
}
 
void one_heater_on()
{
  digitalWrite(heater1, HIGH);  // turn on the heaters
  delay(100);
  Serial.println("  1 ON");  
} 
  
void setup() {
  Serial.begin(9600);
  pinMode(heater1, OUTPUT);
  pinMode(heater2, OUTPUT);
  pinMode(fan, OUTPUT);
  pinMode(vent, OUTPUT);
  // wait for MAX chips to stabilize
  delay(1000);
  all_heaters_off();
  Serial.println("MAX31855 test");
  startTime= millis();
}

int scan_button(int swButton)
{
  int ADCreading = analogRead(swButton);
//  Serial.println(ADCreading);
  if (ADCreading < ADCThreshold) 
         return (1);
  else   return (0);
}

void tempPrint()
{
  Serial.print("Current temperature F = ");
 // Serial.println(tempSensor1,"\t", tempSensor2,"\t",tempSensor3,"\t",tempSensor4,"\t",tempSensor5);
  Serial.print(tempSensor1);
  Serial.print(" ");
  Serial.print(tempSensor2);
  Serial.print(" ");
  Serial.print(tempSensor3);
  Serial.print(" ");
  Serial.print(tempSensor4);
  Serial.print(" ");
  Serial.print(tempSensor5); 
  Serial.print("Average temperature F = ");
  Serial.println(tempAverage);
}


void sensors_reading()
{
    // scanning all of the sensors and switches
  start = scan_button(swStart);
  float tempSum=0;
  int count=0;
  tempSensor1 = thermocouple1.readFarenheit()+sensor1;
  if (tempSensor1>32) // eliminate the SPI disconection 
    {
      count=count+1;
      tempSum=tempSum+tempSensor1;
    }
  delay(10);
  tempSensor2 = thermocouple2.readFarenheit()+sensor2;
    if (tempSensor2>32) // eliminate the SPI disconection 
    {
      count=count+1;
      tempSum=tempSum+tempSensor2;
    }
  delay(10);
  tempSensor3 = thermocouple3.readFarenheit()+sensor3;
    if (tempSensor3>32) // eliminate the SPI disconection 
    {
      count=count+1;
      tempSum=tempSum+tempSensor3;
    }
  delay(10);
  tempSensor4 = thermocouple4.readFarenheit()+sensor4;
    if (tempSensor4>32) // eliminate the SPI disconection 
    /*{
      count=count+1;
      tempSum=tempSum+tempSensor4;
    }*/
  delay(10);
  tempSensor5 = thermocouple5.readFarenheit()+sensor5;
    if (tempSensor5>32) // eliminate the SPI disconection 
    {
      count=count+1;
      tempSum=tempSum+tempSensor5;
    }
  delay(10);
  tempAverage = tempSum/count;
  tempPrint();
}

void loop() {
  all_heaters_off();
  sensors_reading();
  tempInitial = tempAverage;         // initial start temp
  startTime=millis(); 
  // compared and controled
  while (start&(!stopnow))
  {
    currTime=millis()-startTime;   
    currTempSet=min (350, currTime/1000/60*tempInc+tempInitial);
    Serial.print("Set temperature     F = ");
    Serial.println(currTempSet);
    tempSetLow= currTempSet-tempErrRange;
    tempSetHigh= currTempSet+tempErrRange;  
 
    if ((tempAverage<tempSetLow)&&(tempAverage>32) ) 
    {
      if (tempAverage<tempSetLow-tempThreshold) all_heaters_on();
      else one_heater_on();
    }
    else
    {
      if (tempAverage>tempSetHigh+tempThreshold)
      {
        all_heaters_off();
      } 
      else one_heater_off();
    }
    if ((currTime/1000)>=timeMax) 
      {
        stopnow=1;
      }
    delay(timeDelay);
    sensors_reading();
   }
  if (stopnow)
  {
    Serial.println("Mission complete, please wait until the oven cold down");
    while(1);  // stay here until turn off the controller.
  }
  delay(timeDelay);
  
} 

