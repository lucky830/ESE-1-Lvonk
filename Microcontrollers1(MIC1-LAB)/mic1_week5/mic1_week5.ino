/*
  MIC1 Week 5. Sensors and Actuators 1

  Author <Student name>
         <Student number>
  Date   dd/mm/yyyy
*/

// LCD: SDA to A4, SCL to A5
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int trigPin = 6;
const int echoPin = 7;
const int buttonPin = 2;  // Button between D2 and GND

unsigned long previousMillis = 0;
unsigned long interval = 100;
int previousButtonState = HIGH;

int distance_cm = 0;
int distance_max = 0;
bool distanceValid = false;

void setup() {
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
  digitalWrite(trigPin, LOW);
  pinMode(buttonPin, INPUT_PULLUP);
  previousButtonState = digitalRead(buttonPin);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("MIC1 Week 5");
}

void loop() {
  unsigned long currentMillis = millis();

  // Measure the distance every 100 ms
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    unsigned long duration = pulseIn(echoPin, HIGH, 10000);
    distanceValid = (duration != 0);

    if (!distanceValid) {
      lcd.setCursor(0, 1);
      lcd.print("Distance invalid");
    } else {
      distance_cm = duration * 0.034 / 2;

      // Remember the largest distance measured
      if (distance_cm > distance_max) {
        distance_max = distance_cm;
      }
      showBar();
    }
  }

  // Check the button again after 30 ms to avoid contact bounce
  int buttonState = digitalRead(buttonPin);
  if (buttonState != previousButtonState) {
    delay(30);
    buttonState = digitalRead(buttonPin);

    if (buttonState != previousButtonState) {
      previousButtonState = buttonState;

      // Reset the marker once when the button is pressed
      if (buttonState == LOW && distanceValid) {
        distance_max = distance_cm;
        showBar();
      }
    }
  }
}

void showBar() {
  // LCD columns start at 0, so 10 cm puts the marker in column 9
  int markerColumn = distance_max - 1;

  // Keep the marker inside the 16-column display
  if (markerColumn < 0) {
    markerColumn = 0;
  }
  if (markerColumn > 15) {
    markerColumn = 15;
  }

  lcd.setCursor(0, 1);
  for (int column = 0; column < 16; column++) {
    if (column == markerColumn) {
      lcd.print('|');
    } else if (column < distance_cm) {
      lcd.print('#');
    } else {
      lcd.print(' ');  // Clear any old bar characters
    }
  }
}