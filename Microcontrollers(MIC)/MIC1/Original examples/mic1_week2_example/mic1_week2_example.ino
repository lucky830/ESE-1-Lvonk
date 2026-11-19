/*
  MIC1 Week 2. Digital

  Author <Student name>
         <Student number>
  Date   dd/mm/yyyy
*/

// Previous values
unsigned long previousMillis = 0;
int previousButtonState = 0;

// Interval at which to chase (milliseconds)
unsigned long interval = 1000;

// The pins connected to the LEDs
const int ledD1Pin = 13;
const int ledD2Pin = 12;
const int ledD3Pin = 11;
const int ledD4Pin = 10;
const int ledD5Pin = 9;
const int ledD6Pin = 8;

// The LED that is on
int led = ledD1Pin;

// The pin connected to the pushbutton
const int buttonPin = 21;

// Indicates if the LED chaser is stopped
bool stop = false;

void setup() {
  // Initialize the LED pins as output
  pinMode(ledD1Pin, OUTPUT);
  pinMode(ledD2Pin, OUTPUT);
  pinMode(ledD3Pin, OUTPUT);
  pinMode(ledD4Pin, OUTPUT);
  pinMode(ledD5Pin, OUTPUT);
  pinMode(ledD6Pin, OUTPUT);

  // All LEDs off
  digitalWrite(ledD1Pin, LOW);
  digitalWrite(ledD2Pin, LOW);
  digitalWrite(ledD3Pin, LOW);
  digitalWrite(ledD4Pin, LOW);
  digitalWrite(ledD5Pin, LOW);
  digitalWrite(ledD6Pin, LOW);

  // Initialize the button pin as an input
  pinMode(buttonPin, INPUT);

  // Read the button pin startup value
  previousButtonState = digitalRead(buttonPin);
}

void loop() {

  // --------------------------------------------------------------------------

  // Check to see if it's time to select the next LED
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // Save the current time so it can be used to calculate if the has passed
    previousMillis = currentMillis;

    if (stop == false) {
      // Write logic 0 to the pin, turning off the LED
      digitalWrite(led, LOW);

      // Select the next LED...
      led = led - 1;

      // ... making sure to wrap around when at the end
      if (led < ledD6Pin) {
        led = ledD1Pin;
      }

      // Write logic 1 to the pin, turning on the LED
      digitalWrite(led, HIGH);
    }
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
      // Toggle the variable 'stop' from true to false and vice versa
      stop = !stop;
    }

    // Delay a little bit to avoid bouncing
    // Only required if there is no hardware debouncing
    // delay(30);
  }
}
