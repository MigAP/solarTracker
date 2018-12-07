#include <Servo.h>

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

const int iluminationRange = 50;  
int iluminationDiference; 

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
}

void loop() {

	// Read the values of the photoresistors 
	analogSouth = analogRead(southResisor)+40;
	analogNorth = analogRead(northResisor); // To take account of the diference between the resistors 
	analogWest = analogRead(westResisor); // To take account of the diference between the resistors 
	analogEast = analogRead(eastResisor)+40; 

	// // East-West Orientation 
	// // We check if we need to change de orientation first
	// iluminationDiference = analogEast - analogWest; 
	// if ( iluminationDiference > iluminationRange || iluminationRange < - (iluminationRange)){

	// 	if( analogEast >  analogWest){
	// 		servo1SetPoint+=10; 

	// 		// Servo's setpoint verification 
	// 		if(servo1SetPoint > servoTimeUpperLimit1)
	// 			servo1SetPoint = servoTimeUpperLimit1; 

	// 		servo1.writeMicroseconds(servo1SetPoint); 
	// 	}
	// 	else{

	// 		servo1SetPoint-=10; 

	// 		// Servo's setpoint verification 
	// 		if(servo1SetPoint < servoTimeBottomLimit1)
	// 			servo1SetPoint = servoTimeBottomLimit1; 

	// 		servo1.writeMicroseconds(servo1SetPoint); 
	// 	}

	// }
		
	// North-South Orientation 
	// We check if we need to change the orientation : 
	iluminationDiference = analogNorth - analogSouth; 

	Serial.println("Diference "+String(iluminationDiference)) ; 
	Serial.println("Setpoint "+String(servo2SetPoint)); 
		
	if( (iluminationDiference > iluminationRange) ||(iluminationDiference < (-iluminationRange))){

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
		
	delay(10); 
}