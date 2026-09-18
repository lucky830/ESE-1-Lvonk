/**
 * L298N Motor Driver - Two DC Motor Control
 * Author: Omar Draidrya
 * Website: omartronics.com
 *
 * Controls forward, stop, and reverse for two DC motors
 * using the L298N dual H-bridge with PWM speed control.
 */

#define IN1 9    // Motor A direction pin 1
#define IN2 8    // Motor A direction pin 2
#define IN3 7    // Motor B direction pin 1
#define IN4 6    // Motor B direction pin 2
#define ENA 10   // Motor A speed control (PWM)
#define ENB 5    // Motor B speed control (PWM)

const int motorSpeed = 140;
const int motorSpeed2 = 150;

void setup() {
  // Set all motor control pins as outputs
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  // Start with motors stopped
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  delay(2000);
}

void loop() {
  // --- Move both motors FORWARD at half speed ---
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, motorSpeed2);   // Motor A at ~50% speed
  analogWrite(ENB, motorSpeed);   // Motor B at ~50% speed
  delay(2500);

  // --- STOP both motors ---
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  delay(1000);

  // --- Move both motors BACKWARD at half speed ---
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, motorSpeed2);
  analogWrite(ENB, motorSpeed);
  delay(2500);

  // --- STOP both motors ---
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  delay(1000);

  // digitalWrite(IN1, HIGH);
  // digitalWrite(IN2, LOW);
  // digitalWrite(IN3, LOW);
  // digitalWrite(IN4, HIGH);
  // analogWrite(ENA, motorSpeed);   // Motor A at ~50% speed
  // analogWrite(ENB, motorSpeed);   // Motor B at ~50% speed
  // delay(1000);

  // digitalWrite(IN1, LOW);
  // digitalWrite(IN2, HIGH);
  // digitalWrite(IN3, HIGH);
  // digitalWrite(IN4, LOW);
  // analogWrite(ENA, motorSpeed);   // Motor A at ~50% speed
  // analogWrite(ENB, motorSpeed);   // Motor B at ~50% speed
  // delay(1000);

}