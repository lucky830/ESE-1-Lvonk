
/*! ***************************************************************************
 *
 * \brief     I2C functions
 * \file      i2c.cpp
 * \author    Hugo Arends
 * \date      July 2025
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
 #include "i2c.h"

 #include <avr/io.h>

// -----------------------------------------------------------------------------
// Local type definitions
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Local function prototypes
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Local variables
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Local function implementation
// -----------------------------------------------------------------------------

/**
 * @brief Initializes the I2C master interface.
 *
 * Configures the SDA and SCL pins as outputs, enables pull-up resistors,
 * and sets the SCL bit rate to 100kHz based on the CPU frequency.
 */
void i2c_master_init(void)
{
  // Set SDA and SCL pins output
  DDRC |= (1<<DDC4) | (1<<DDC5);

  // Enable pullups
  PORTC |= (1<<DDC4) | (1<<DDC5);

  // Calculate TWBR value, see datasheet:
  // SCL_frequency = F_CPU / (16 + 2(TWBR) * PrescalerValue)
  //
  // Rewritten:
  // TWBR = ((F_CPU / SCL_frequency) - 16) / (2*PrescalerValue)
  //
  // Set SCL bit rate 100KHz
  TWBR = ((F_CPU / 100000) - 16) / (2 * 1);
}

/**
 * @brief Writes data to an I2C slave device.
 *
 * Generates a start condition, sends the slave address with write bit,
 * writes the specified number of bytes from the provided buffer, and
 * generates a stop condition.
 *
 * @param dev_address 7-bit I2C slave device address.
 * @param p Pointer to the data buffer to be written.
 * @param len Number of bytes to write.
 */
void i2c_write(const uint8_t dev_address, uint8_t *p, const uint8_t len)
{
  // Generate start condition
  TWCR = (1<<TWEN) | (1<<TWINT) | (1<<TWSTA);
  loop_until_bit_is_set(TWCR, TWINT);

  // Send slave address + W
  TWDR = (dev_address<<1);
  TWCR = (1<<TWEN) | (1<<TWINT);
  loop_until_bit_is_set(TWCR, TWINT);

  // Write the data
  for(uint8_t i=0; i<len; ++i)
  {
    TWDR = p[i];
    TWCR = (1<<TWEN) | (1<<TWINT);
    loop_until_bit_is_set(TWCR, TWINT);
  }

  // Stop condition
  TWCR = (1<<TWEN) | (1<<TWINT) | (1<<TWSTO);
}

/**
 * @brief Reads data from an I2C slave device.
 *
 * Generates a start condition, sends the slave address with read bit,
 * reads the specified number of bytes into the provided buffer, and
 * generates a stop condition.
 *
 * @param dev_address 7-bit I2C slave device address.
 * @param p Pointer to the buffer where read data will be stored.
 * @param len Number of bytes to read.
 */
void i2c_read(const uint8_t dev_address, uint8_t *p, const uint8_t len)
{
  // Generate start condition
  TWCR = (1<<TWEN) | (1<<TWINT) | (1<<TWSTA);
  loop_until_bit_is_set(TWCR, TWINT);

  // Send the slave address + R
  TWDR = (dev_address<<1) | 0x01;
  TWCR = (1<<TWEN) | (1<<TWINT);
  loop_until_bit_is_set(TWCR, TWINT);

  // Read the data
  for(uint8_t i=0; i<len; ++i)
  {
    TWCR = (1<<TWEN) | (1<<TWINT);
    loop_until_bit_is_set(TWCR, TWINT);
    p[i] = TWDR;
  }

  // Stop condition
  TWCR = (1<<TWEN) | (1<<TWINT) | (1<<TWSTO);
}
