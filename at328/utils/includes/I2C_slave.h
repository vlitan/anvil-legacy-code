#ifndef I2C_SLAVE_H
#define I2C_SLAVE_H

volatile uint8_t buffer_address;
volatile uint8_t txbuffer[0xFF];
volatile uint8_t rxbuffer[0xFF];
/*this is not what the original author wanted to do
but this is the only way I could make it work*/
volatile uint8_t data;


void I2C_init(uint8_t address);
void I2C_stop(void);
ISR(TWI_vect);

#endif // I2C_SLAVE_H
