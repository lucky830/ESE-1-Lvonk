/*
  MIC1 Week 6. Sensors and Actuators 2

  Author <Student name>
         <Student number>
  Date   dd/mm/yyyy
*/
// Include the EEPROM library
#include <EEPROM.h>

// Previous values
unsigned long previousMillis = 0;
int previousButtonState = 0;

// The pin connected to the pushbutton
const int buttonPin = 21;

// EEPROM data
const int N_DATA = 4;
const int data[N_DATA] = { 0, 1, 2, -1 };

// Interval at which to display the EEPROM contents (milliseconds)
unsigned long interval = 1000;

void setup() {
  Serial.begin(9600);

  // Initialize the button pin as an input
  pinMode(buttonPin, INPUT);

  // Read the button pin startup value
  previousButtonState = digitalRead(buttonPin);
}

void loop() {

  // --------------------------------------------------------------------------

  // Check to see if it's time for the next reading
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // Save the current time so it can be used to calculate if the has passed
    previousMillis = currentMillis;

    for (unsigned int i = 0; i < N_DATA; i++) {
      // The size of an int is two bytes. Use the sizeof() preprocessor
      // operator instead.
      unsigned int address = i * sizeof(int);
      int value = 0;

      EEPROM.get(address, value);

      Serial.print(value);
      Serial.print(" ");
    }
    Serial.println();
  }

  // --------------------------------------------------------------------------

  // Read the pushbutton input pin
  int buttonState = digitalRead(buttonPin);

  // Compare the buttonState to its previous state
  if (buttonState != previousButtonState) {
    // State has changed, so save the new state
    previousButtonState = buttonState;

    // Check the button value. If the value is LOW the button was pressed
    if (buttonState == LOW) {

      Serial.println("Switch press detected: writing EEPROM");

      for (unsigned int i = 0; i < N_DATA; i++) {
        // The size of an int is two bytes. Use the sizeof() preprocessor
        // define instead.
        unsigned int address = i * sizeof(int);
        int value = data[i];

        EEPROM.put(address, value);

        Serial.print(value);
        Serial.print(" ");
      }
      Serial.println();
    }

    // Delay a little bit to avoid bouncing
    // Only required if there is no hardware debouncing
    // delay(30);
  }
}
