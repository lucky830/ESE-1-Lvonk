/*
  MIC1 Week 1. Basics

  Author Lucas Vonk
         2189041
  Date   03/09/2026
*/

// C++ code
//
/*
  AnalogReadSerial
  Reads an analog input (potentiometer) on pin 0,
  prints the result to the serial monitor.

  OPEN THE SERIAL MONITOR TO VIEW THE OUTPUT FROM
  THE POTENTIOMETER >>

  Attach the center pin of a potentiometer to pin
  A0, and the outside pins to +5V and ground.

  This example code is in the public domain.
*/

int sensorValue = 0;

void setup()
{
  pinMode(A0, INPUT);
  Serial.begin(9600);
}

void loop()
{
  // read the input on analog pin 0:
  sensorValue = analogRead(A0);
  Serial.println(sensorValue);
  // print out the value you read:
  
//  switch(sensorValue) {
//   case 1013:
//     Serial.println("1");
//     break;
//   case 930:
//     Serial.println("2");
//     break;
//   case 818:
//     Serial.println("3");
//     break;
//   case 682:
//     Serial.println("4");
//     break;
//   case 511:
//     Serial.println("5");
//     break;
//   case 205:
//     Serial.println("6");
//     break;
//   case 114:
//     Serial.println("7");
//     break;
//   case 31:
//     Serial.println("8");
//     break;
  //default:
   // Serial.println(sensorValue);
	//break;
// }
  delay(100); // Delay a little bit to improve simulation performance
}


// int LEDPinArray[] = { 8, 9, 10, 11, 12, 13 };  // All pins of the leds in a array
// // int LEDcount = 6;                               // Set amount of leds in the array
// int LEDcount = (sizeof(LEDPinArray) / sizeof(LEDPinArray[0]));



// void setup() {
//   // Reference: https://www.w3schools.com/cpp/cpp_for_loop.asp
//   for (int i = 0; i < LEDcount; i++) {
//     pinMode(LEDPinArray[i], OUTPUT);
//     digitalWrite(LEDPinArray[i], LOW);
//   }
// }


// void loop() {

//   for (int i = 0; i < LEDcount; i++) {
//     digitalWrite(LEDPinArray[i], HIGH);
//     delay(25);
//     digitalWrite((LEDPinArray[i]), LOW);
//   }
// }
