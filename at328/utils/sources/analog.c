#include <avr/io.h>
#include <stdint.h>

void adc_init()
{
 ADMUX |= (1<<REFS0); //SELECT Vref = AVcc
 ADCSRA |= (1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);  //set prescaller to 128
 ADCSRA |= (1<<ADEN);    //enable ADC
}

uint8_t	analog_read(uint8_t channel)
{
    ADMUX = (ADMUX & 0xf0) | (channel & 0x0f); // set ADC channel w/ security mask
    ADMUX |= (1 << ADLAR);   // clear for 8 bit resolution
	ADCSRA |= (1 << ADSC);    // Start the ADC conversion
    while(ADCSRA & (1 << ADSC));    // this line waits for the ADC to finish 
    return (ADCH);					//et voila, le resultat
}
