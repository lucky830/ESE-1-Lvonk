/*
  MIC1 Week 1. Basics

  Author Lucas Vonk
         <Student number>
  Date   03/09/2026
*/

int LEDPinArray[] = { 8, 9, 10, 11, 12, 13 };  // All pins of the leds in a array
// int LEDcount = 6;                               // Set amount of leds in the array
int LEDcount = (sizeof(LEDPinArray) / sizeof(LEDPinArray[0]));



void setup() {
  // Reference: https://www.w3schools.com/cpp/cpp_for_loop.asp
  for (int i = 0; i < LEDcount; i++) {
    pinMode(LEDPinArray[i], OUTPUT);
    digitalWrite(LEDPinArray[i], LOW);
  }
}


void loop() {

  for (int i = 0; i < LEDcount; i++) {
    digitalWrite(LEDPinArray[i], HIGH);
    delay(25);
    digitalWrite((LEDPinArray[i]), LOW);
  }
}
