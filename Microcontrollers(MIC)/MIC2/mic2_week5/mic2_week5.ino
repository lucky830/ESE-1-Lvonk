/*
  MIC2 Week 5. Sensors and Actuators

  Author <Student name>
         <Student number>
  Date   dd/mm/yyyy
*/
#include "timer0.h"
#include "lcd16x2_i2c.h"

int main(void) {

  // Initialize the LCD
  lcd_init();
  lcd_backlight(on);

  lcd_set_cursor(0, 0);
  lcd_put("MIC2 Week 5");
  lcd_set_cursor(0, 1);
  lcd_put("made by ESE");

  while (1) {
  }
}
