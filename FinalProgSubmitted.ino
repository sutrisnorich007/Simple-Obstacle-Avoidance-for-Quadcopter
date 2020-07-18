#include "Adafruit_VL53L0X.h"
#include <Servo.h> //servo library

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

Servo FlightMode; //servo declaration
Servo OutThrottle;

const int In_Throttle_pin=8; //pin declaration
const int In_FlightMode_pin=9;
const int Out_Throttle_pin=12;
const int Out_FlightMode_pin=13;

int In_Throttle;
int In_FlightMode;
int In_FlightMode_a;
int Distance;


void setup() {
  OutThrottle.attach(Out_Throttle_pin);
  FlightMode.attach(Out_FlightMode_pin);
  Serial.begin(115200);
  // wait until serial port opens for native USB devices
  while (! Serial) {
    delay(1);
  }
  
  Serial.println("Adafruit VL53L0X test");
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
  // power 
  Serial.println(F("VL53L0X API Simple Ranging example\n\n")); 
}

void Printer(){
  Serial.print("D=");  Serial.print(Distance);

  Serial.print("In_Throttle=");  Serial.print(In_Throttle);

  Serial.print("In_FlightMode=");  Serial.println(In_FlightMode);
  
}

void loop() {
  VL53L0X_RangingMeasurementData_t measure;
    
//  Reading a measurement
  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

  if (measure.RangeStatus != 4) {  // phase failures have incorrect data
    Distance=measure.RangeMilliMeter;
  } else {
  Distance = 9999;
  }
  Serial.println(Distance); 
  In_Throttle = pulseIn(In_Throttle_pin, HIGH);  //Read throttle
  In_FlightMode = pulseIn(In_FlightMode_pin, HIGH); //Read Flight Mode
  if (Distance <= 1000){   //If obstacle detected
  OutThrottle.writeMicroseconds(1800);// Value from the payload testing
  //delayMicroseconds(300);
  FlightMode.writeMicroseconds(2000); // Value to Set Mode to Loiter & Throttle naik
  //Serial.println("AVOID IN USE");
  //delay (1000); // delay if needed to clear obstacle
  } else{
  OutThrottle.writeMicroseconds(In_Throttle); 
  //delayMicroseconds(2000);
  FlightMode.writeMicroseconds(In_FlightMode-50);    // Bypass Mode and Throttle PWM  
//  Printer(); 
  }
}
