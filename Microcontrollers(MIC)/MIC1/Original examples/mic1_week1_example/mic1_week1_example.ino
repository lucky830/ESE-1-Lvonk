/*
  MIC1 Week 1. Basics

  Author <Student name>
         <Student number>
  Date   dd/mm/yyyy
*/

// The setup function runs once when you press reset or power the board
void setup() {
  // Configure pins
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);

  // Set initial pin value
  digitalWrite(13, LOW);
  digitalWrite(12, LOW);
}

// The loop function runs over and over again forever
void loop() {
  // Previous LED off and next LED on
  digitalWrite(12, LOW);
  digitalWrite(13, HIGH);
  delay(200);

  // Previous LED off and next LED on
  digitalWrite(13, LOW);
  digitalWrite(12, HIGH);
  delay(200);
}
