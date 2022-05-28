/*
 * attiny_pwm.c
 *
 * Created: 23/05/2022 12:26:27 PM
 * Author : sli43
 */ 


#include <avr/io.h>
#include <util/delay.h>

#define N_1    (_BV(CS00))
#define N_8    (_BV(CS01))
#define N_64   (_BV(CS01)|_BV(CS00))
#define N_256  (_BV(CS02))
#define N_1024 (_BV(CS02)|_BV(CS00))
#define F_PWM (_BV(WGM01)|_BV(WGM00)|_BV(WGM02))
#define CTC (_BV(WGM01))
#define ADC_3 (_BV(MUX1)|_BV(MUX0))
#define S_2 _BV(ADPS0)
#define S_4 _BV(ADPS1)
#define S_8 (_BV(ADPS1)|_BV(ADPS0)
#define S_16 _BV(ADPS2)
#define S_128 (_BV(ADPS2)|_BV(ADPS1)|_BV(ADPS0))
#define F_CPU 960000
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

/* When timer is set to Fast PWM Mode, the freqency can be
calculated using equation: F = F_CPU / (N * 256) 

 -> F(N_1) = 4.687kHz
 -> F(N_8) = 585Hz
 -> F(N_64) = 73Hz
 -> F(N_256) = 18Hz
 -> F(N_1024) = 4Hz */
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
    /* setup */
    pwm_init();
    pwm_set_frequency(N_1);
	ADC_init();
    /* loop */
   while (1)
   {
	   uint32_t reading = ADC_Read();
	   
	   pwm_set_duty(map(reading, 0, 1023, 0, 255));
	   _delay_ms(1);
   }
}