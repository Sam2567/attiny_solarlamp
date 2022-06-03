/*
 * attiny_pwm.c
 *
 * Created: 23/05/2022 12:26:27 PM
 * Author : sli43
 */ 


#include <avr/io.h>
#include <util/delay.h>
#include "config.h"

#define sw1 ~PINB & 0x04


static void pwm_init(void)
{
    DDRB |= (1 << 0); // set PWM pin as OUTPUT
    TCCR0A |= F_PWM; // set timer mode to FAST PWM
    TCCR0A |= (1 << COM0A1); // connect PWM signal to pin (AC0A => PB0)
}

static void ADC_init(void){
	ADMUX |= ADC_3;
	ADCSRA|= (1 << ADEN);
}

uint32_t ADC_Read()
{
	uint32_t Ain,AinLow;
	//start conversion
	ADCSRA |=(1 << ADSC);
	//config ADC Prescaler Select Bits
	ADCSRA|= S_128;
	//wait until conversion complete
	while((ADCSRA&(1<<ADIF))==0);
	_delay_us(10);
	
	AinLow = ADCL;
	Ain = ADCH;
	Ain = Ain << 8;
	Ain = Ain | AinLow;
	return Ain;
}


static void pwm_set_frequency(uint32_t N)
{
    TCCR0B = (TCCR0B & ~((1<<CS02)|(1<<CS01)|(1<<CS00))) | N; // set prescaler
}

static void pwm_set_duty(uint8_t duty)
{

    OCR0A = duty; // set the OCR
}


uint32_t map(uint32_t x, uint32_t in_min, uint32_t in_max, uint32_t out_min, uint32_t out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int main(void)
{
	DDRB &=~(1 << 2);
	PORTB|=(1 << 2);
	DDRB|=(1 << 4);
    /* setup */
	ADC_init();
    /* loop */
   while (1)
   {
	   uint32_t reading = ADC_Read();
	   uint32_t map_value = map(reading, 0, 1023, 0, 255);
	   if(map_value < 170){
		   PORTB |=(1 << 4);
	   }else{
		   PORTB &=~(1 << 4);
	   }
	   _delay_ms(1);
   }
}