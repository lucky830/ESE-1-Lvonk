/*
  MIC1 Week 4. Analog

  Author <Student name>
         <Student number>
  Date   dd/mm/yyyy
*/

// Previous values
unsigned long previousMillis = 0;
int previousButtonState = 0;

// Interval at which to chase (milliseconds)
unsigned long interval = 100;

// The pin connected to the pushbutton
const int buttonPin = 21;

// The pin connected to the photoresistor
int photoresistorPin = A0;

// The value read by the ADC
int photoresistorValue = 0;

// A String to hold incoming data
String inputString = "";

// A flag that indicates if the string is complete
bool stringComplete = false;

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

    photoresistorValue = analogRead(photoresistorPin);

    Serial.print("Photoresistor value: ");
    Serial.println(photoresistorValue);
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
      Serial.println("SW0 pressed");
    }

    // Delay a little bit to avoid bouncing
    delay(30);
  }

  // --------------------------------------------------------------------------

  if (stringComplete == true) {
    // Clear the flag
    stringComplete = false;

    if (inputString == "5\n") {
      Serial.println("50%");
    }

    // Clear the string:
    inputString = "";
  }
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent() {
  while (Serial.available()) {
    // Get the new byte
    char inChar = (char)Serial.read();

    // Add it to the inputString
    inputString += inChar;

    // Is the incomming character a New Line ('\n')?
    if (inChar == '\n') {
      // Set the flag
      stringComplete = true;
    }
  }
}
