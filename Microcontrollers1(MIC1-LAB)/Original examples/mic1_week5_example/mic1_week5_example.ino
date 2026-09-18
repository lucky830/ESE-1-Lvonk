/*
  MIC1 Week 5. Sensors and Actuators 1

  Author <Student name>
         <Student number>
  Date   dd/mm/yyyy
*/

// Include the LCD library
// Must use A4 for SDA and A5 for SCL
#include <lcd16x2_i2c.h>

// Associate the HC-SR04 sensor pins with the arduino pins
const int trigPin = 6;
const int echoPin = 7;

// Previous values
unsigned long previousMillis = 0;

// Interval at which to measure the Ping sensor
unsigned long interval = 100;

void setup() {

  // Setup HC-SR04 pins
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
  digitalWrite(trigPin, LOW);

  // Initialize the LCD
  lcd_init();
  lcd_backlight(on);
  lcd_set_cursor(0, 0);
  lcd_put("MIC1 Week 5");
}

void loop() {

  // --------------------------------------------------------------------------

  // Check to see if the interval has passed
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // Save the current time so it can be used to calculate if the has passed
    previousMillis = currentMillis;

    // Create a 10 microsecond pulse to trigger the sensor
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Measure the pulse returned by the sensor
    // Set timeout to 10 ms
    unsigned long duration = pulseIn(echoPin, HIGH, 10000);

    lcd_set_cursor(0, 1);

    // Check validity of the measured value
    if (duration == 0) {
      // Notify user of invalid measurement
      lcd_put("Distance invalid");
    } else {
      // Calculate the distance in cm
      int distance_cm = duration * 0.034 / 2;
      String distance_cm_str = String(distance_cm);

      // Show distance on LCD with leading zeros
      // and trailing spaces (to clear the line)
      if (distance_cm < 10) {
        lcd_put("00");
      } else if (distance_cm < 100) {
        lcd_put("0");
      }
      lcd_put(distance_cm_str.c_str());
      lcd_put(" cm          ");
    }
  }
}
