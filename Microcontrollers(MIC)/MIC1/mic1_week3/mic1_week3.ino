/*
  MIC1 Week 3. Communication

  Author Lucas Vonk
         2189041
  Date   24/09/2026
*/

// Previous values
unsigned long previousMillis = 0;
int previousButtonState = 0;

// Interval at which to chase (milliseconds)
unsigned long interval = 100;

// The pin connected to the pushbutton
const int buttonPin = 21;

int ledState = 0;
int prevLedState = 0;
const int ledPin2 = 9;

// A String to hold incoming data
String inputString = "";
String studentNr = "2189041";

// A flag that indicates if the string is complete
bool stringComplete = false;

const int ledPin = 8;

void setup() {
  Serial.begin(9600);

  // Initialize the button pin as an input
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  // Read the button pin startup value
  previousButtonState = digitalRead(buttonPin);
  digitalWrite(ledPin, HIGH);
  

  Serial.print("Enter Studentnumber:");
}

void loop() {

  // --------------------------------------------------------------------------

  // Check to see if it's time to select the next LED
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // Save the current time so it can be used to calculate if the has passed
    previousMillis = currentMillis;
    switch(prevLedState) {
      case 0:
        digitalWrite(ledPin2, HIGH);
        ledState = 1;
        break;
      case 1:
        digitalWrite(ledPin2, LOW);
        ledState = 0;
        break;
    }
    prevLedState = ledState;

    // Serial.println("Timeout");
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
      // Serial.println("SW0 pressed");
      digitalWrite(ledPin, LOW);
    }

    // Delay a little bit to avoid bouncing
    delay(30);
  }

  // --------------------------------------------------------------------------

  if (stringComplete == true) {
    // Clear the flag
    stringComplete = false;

    Serial.println("received");


    // for(unsigned int i=0; i<inputString.length(); i++)
    //   {
    //   // Serial.print(" 0x");
    //   // Serial.print(inputString[i], HEX);
    //   // Serial.print("\traw:");
    //   Serial.print(inputString);
    //   }

    Serial.print(inputString);
    Serial.println();
    Serial.print(studentNr);
    Serial.println();

    if(inputString == studentNr){
      Serial.print("Hello Lucas!");
      digitalWrite(ledPin, HIGH);
    } 
    else if (inputString != studentNr) {
      Serial.println("Unknown");
    }



    // Clear the string:
    inputString = "";
  }
}


void serialEvent() {
  while (Serial.available()) {
    // Get the new byte
    char inChar = (char)Serial.read();

    if (inChar == '\n') {
      // Set the flag
      stringComplete = true;
    } else {
    // Add it to the inputString
    inputString += inChar;
    }
    // Is the incomming character a New Line ('\n')?


  }
}
