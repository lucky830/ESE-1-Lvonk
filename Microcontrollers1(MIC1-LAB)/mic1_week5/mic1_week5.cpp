#include <Arduino.h>

void setup()
{
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop()
{
  digitalWrite(LED_BUILTIN, HIGH); // change state of the LED by setting the pin to the HIGH voltage level
  delay(1000);                     // wait for a second
  digitalWrite(LED_BUILTIN, LOW);  // change state of the LED by setting the pin to the LOW voltage level
  delay(1000);                     // wait for a second
}