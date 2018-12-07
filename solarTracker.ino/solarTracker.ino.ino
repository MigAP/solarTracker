#include <Servo.h>
#include <SPI.h>
#include "SdFat.h"
SdFat SD;

#define OLED_RESET 4

// Powermeter IO and variables 
unsigned long interval = 100;
unsigned long previousMillis = 0;
const int chipSelect = 10;

const int solarPin {A4}; 
float resistorVoltage; 
float resistorCurrent;

File TimeFile;
File VoltFile;
File CurFile;

// Photoresistor analogs input definitions 
const int northResisor = A0; 
const int southResisor = A1; 
const int eastResisor = A2; 
const int westResisor = A3; 

// ServoMotor pin 
const int servo1Pin = 5; 
const int servo2Pin = 6;

// ServoMotor Definition 
Servo servo1; //  East-West orientation 
Servo servo2; // North-South orientation 

// Mesured servo's time limit to change rotation sense 
const unsigned int servoTimeLimit1 =1650 ;
const unsigned int servoTimeUpperLimit1=2430; 
const unsigned int servoTimeBottomLimit2=710; 

const unsigned int servoTimeLimit2 = 910; 
const unsigned int servoTimeUpperLimit2=1240; 
const unsigned int servoTimeBottomLimit1=740; 


// Servo's microservos value 
unsigned int servo1SetPoint = servoTimeLimit1; 
unsigned int servo2SetPoint = servoTimeLimit2; 

// Variables used to stock the values of the analog reads 
unsigned int analogSouth;
unsigned int analogNorth; 
unsigned int analogWest; 
unsigned int analogEast;  

const int iluminationRangeNS = 50;  
const int iluminationRangeEW = 30;  
int iluminationDiference; 

void logData(unsigned long currentMillis, float resistorCurrent, float resistorVoltage); 

void setup() {

	// Analogs input definitons 
	pinMode(southResisor, INPUT); 
	pinMode(northResisor, INPUT); 
	pinMode(westResisor,INPUT); 
	pinMode(eastResisor, INPUT); 

	// Servo configuration 
	servo1.attach(servo1Pin); 
	servo2.attach(servo2Pin); 
	servo1.writeMicroseconds(servo1SetPoint); 
	servo2.writeMicroseconds(servo2SetPoint); 
	Serial.begin(9600); 
	SD.begin(chipSelect);
}

void loop() {

	// Read the values of the photoresistors 
	analogSouth = analogRead(southResisor)+40;
	analogNorth = analogRead(northResisor); // To take account of the diference between the resistors 
	analogWest = analogRead(westResisor); // To take account of the diference between the resistors 
	analogEast = analogRead(eastResisor)+10; 

	// East-West Orientation 
	// We check if we need to change de orientation first
	iluminationDiference = analogEast - analogWest; 
	Serial.println("DiferenceEW "+String(iluminationDiference)) ; 
	Serial.println("SetpointEW "+String(servo2SetPoint)); 
	if ( iluminationDiference > iluminationRangeEW || iluminationDiference < - (iluminationRangeEW)){

		if( analogEast >  analogWest){
			servo1SetPoint+=10; 

			// Servo's setpoint verification 
			if(servo1SetPoint > servoTimeUpperLimit1)
				servo1SetPoint = servoTimeUpperLimit1; 

			servo1.writeMicroseconds(servo1SetPoint); 
		}
		else{

			servo1SetPoint-=10; 

			// Servo's setpoint verification 
			if(servo1SetPoint < servoTimeBottomLimit1)
				servo1SetPoint = servoTimeBottomLimit1; 

			servo1.writeMicroseconds(servo1SetPoint); 
		}

	}
		
	// North-South Orientation 
	// We check if we need to change the orientation : 
	iluminationDiference = analogNorth - analogSouth; 

	Serial.println("DiferenceNS "+String(iluminationDiference)) ; 
	Serial.println("SetpointNS "+String(servo2SetPoint)); 
		
	if( (iluminationDiference > iluminationRangeNS) ||(iluminationDiference < (-iluminationRangeNS))){

		if(analogNorth < analogSouth){
			servo2SetPoint += 10; 

			// Servo setpoint verification 
			if(servo2SetPoint > servoTimeUpperLimit2)
				servo2SetPoint = servoTimeUpperLimit2; 

			servo2.writeMicroseconds(servo2SetPoint); 
		}
		else{
			servo2SetPoint -= 10; 

			// Servo setpoint verification 
			if(servo1SetPoint < servoTimeBottomLimit2)
				servo2SetPoint = servoTimeUpperLimit2; 

			servo2.writeMicroseconds(servo2SetPoint); 
		}
	}
	
	// Log Data to the SD card 
	unsigned long currentMillis = millis();

  	if (currentMillis - previousMillis >= interval)
	  {
	    previousMillis = currentMillis;
	    resistorVoltage = analogRead(solarPin)*(5.0/1023.0); 
	    resistorCurrent = resistorVoltage/10.0; // current in mA
	    Serial.println("Time "+ String(currentMillis)); 
	    Serial.println("Current "+String(resistorCurrent)); 
	    Serial.println("Voltage "+ String(resistorVoltage)); 
	    logData(currentMillis, resistorCurrent, resistorVoltage); 
	  }	
	delay(10); 
}

void logData(unsigned long currentMillis, float resistorCurrent, float resistorVoltage ){

  TimeFile = SD.open("TIME.txt", FILE_WRITE);
  if (TimeFile) {
    TimeFile.println(currentMillis);
    TimeFile.close();
  }

  VoltFile = SD.open("VOLT.txt", FILE_WRITE);
  if (VoltFile) {
    VoltFile.println(resistorVoltage);
    VoltFile.close();
  }

  CurFile = SD.open("CUR.txt", FILE_WRITE);
  if (CurFile) {
    CurFile.println(resistorCurrent);
    CurFile.close();
  }
  
}