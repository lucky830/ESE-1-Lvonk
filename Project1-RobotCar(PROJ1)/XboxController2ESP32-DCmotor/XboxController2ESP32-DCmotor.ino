#include <Arduino.h>
#include <BLEGamepadClient.h>

XboxController controller;

int ledPin = 8;  // LED connected to digital pin 9
float SpeedA;
float SpeedB;

#define IN1 6    // Motor A direction pin 1
#define IN2 7    // Motor A direction pin 2
#define ENA 5   // Motor A speed control (PWM)

bool connected;

void setup() {
  // nothing happens in setup
  Serial.begin(115200);
  controller.begin();

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);
  // Start with motors stopped
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  Serial.println("Connect the controller...");
}

void loop() {

  if (controller.isConnected()) {
    XboxControlsState s;controller.read(&s);
    SpeedA = s.rightTrigger * 255;
    SpeedB = s.leftTrigger * 255;

    if (SpeedA > 50 && SpeedB < 2) {
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      analogWrite(ENA, SpeedA);
      analogWrite(ledPin, (255.0f - SpeedA));
      Serial.printf("> Speed: %.2f\n", SpeedA);
    } else if (SpeedB > 50 && SpeedA < 2) {
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      analogWrite(ENA, SpeedB);
      analogWrite(ledPin, (255.0f - SpeedB));
      Serial.printf("< Speed: %.2f\n", SpeedB);
    } else if (SpeedA > 2 && SpeedB > 2) {
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      Serial.println("One trigger at a time!!!");
      Serial.printf("L: %.2f\tR: %.2f", SpeedB, SpeedA);
      analogWrite(ledPin, 255);
      delay(100);
    } else {
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      analogWrite(ledPin, 255);
      }

    connected = true;
  }
  else {
    if (connected == true) {Serial.println("controller not connected");}
    connected = false;
  }

  delay(20);
}
