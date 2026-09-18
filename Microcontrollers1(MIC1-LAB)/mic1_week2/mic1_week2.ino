/*
  MIC1 Week 2. Digital

  Author Lucas Vonk
         2189041
  Date   17/09/2026
*/

int LEDPinArray[] = { 8, 9, 10, 11, 12, 13 };         // All pins of the leds in a array
int LEDPinArrayReverse[] = { 13, 12, 11, 10, 9, 8 };  // All pins of the leds in a array
// int LEDcount = 6;                               // Set amount of leds in the array
int LEDcount = (sizeof(LEDPinArray) / sizeof(LEDPinArray[0]));

int direction = 0;
const int buttonPin = 3;
int state = 1;
const int stateButtonPin = 4;
int stateButtonPinState = 0;
int lastStateButtonPinState = 0;


void setup() {
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(stateButtonPin, INPUT_PULLUP);

  // Reference: https://www.w3schools.com/cpp/cpp_for_loop.asp
  for (int i = 0; i < LEDcount; i++) {
    pinMode(LEDPinArray[i], OUTPUT);
    digitalWrite(LEDPinArray[i], LOW);
  }
}


void loop() {
stateButtonPinState = digitalRead(stateButtonPin);
if (stateButtonPinState != lastStateButtonPinState && stateButtonPinState == LOW) {state = 1; delay(100);}
  
if (digitalRead(buttonPin) == LOW) {direction = !direction; delay(50);}

while (state == 1) {
  if (direction == 0) {
    for (int i = 0; i < LEDcount; i++) {
      digitalWrite(LEDPinArrayReverse[i], HIGH);
      delay(100);
      digitalWrite((LEDPinArrayReverse[i]), LOW);
      if (digitalRead(buttonPin) == LOW) {direction = !direction; delay(100); break;}
      if (digitalRead(stateButtonPin) == LOW) {state = 0; delay(100); break;}
    }
  } 
  else if (direction == 1) {
    for (int i = 0; i < LEDcount; i++) {
      digitalWrite(LEDPinArray[i], HIGH);
      delay(100);
      digitalWrite((LEDPinArray[i]), LOW);
      if (digitalRead(buttonPin) == LOW) {direction = !direction; delay(100); break;}
      if (digitalRead(stateButtonPin) == LOW) {state = 0; delay(100); break;}
    }
  }
  if (state == 0) {break;}
}

lastStateButtonPinState = stateButtonPinState;
}
