/*! ***************************************************************************
 *
 * \brief     LCD 16x2 I2C functions
 * \file      lcd16x2_i2c.h
 * \author    Hugo Arends
 * \date      July 2025
 *
 *                  Arduino                 1602 LCD I2C
 *            ------------------+         +--------------+
 *                              |         |
 *                    A5/PC5/SCL+---------+SCL
 *                    A4/PC4/SDA+---------+SDA
 *                              |         |
 *                           5V0+---------+VCC
 *                           GND+---------+GND
 *                              |         |
 *            ------------------+         +--------------+
 *
 * \copyright 2025 HAN University of Applied Sciences. All Rights Reserved.
 *            \n\n
 *            Permission is hereby granted, free of charge, to any person
 *            obtaining a copy of this software and associated documentation
 *            files (the "Software"), to deal in the Software without
 *            restriction, including without limitation the rights to use,
 *            copy, modify, merge, publish, distribute, sublicense, and/or sell
 *            copies of the Software, and to permit persons to whom the
 *            Software is furnished to do so, subject to the following
 *            conditions:
 *            \n\n
 *            The above copyright notice and this permission notice shall be
 *            included in all copies or substantial portions of the Software.
 *            \n\n
 *            THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *            EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *            OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *            NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *            HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *            WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *            FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *            OTHER DEALINGS IN THE SOFTWARE.
 *
 *****************************************************************************/
#ifndef LCD16X2_I2C_H
#define LCD16X2_I2C_H

#include <stdint.h>

// -----------------------------------------------------------------------------
// Type definitions
// -----------------------------------------------------------------------------
typedef enum
{
    off = 0,
    on = 1,
}bl_t;

// -----------------------------------------------------------------------------
// Function prototypes
// -----------------------------------------------------------------------------
void lcd_init(void);
void lcd_backlight(const bl_t bl);
void lcd_clear(void);
void lcd_return_home(void);
void lcd_set_cursor(const uint8_t col, const uint8_t row);
void lcd_put(const char *str);
void lcd_put_int(int i);

#endif // LCD16X2_I2C_H
