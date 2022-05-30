/*
 * config.h
 *
 * Created: 30/05/2022 1:21:16 PM
 *  Author: sli43
 */ 


#ifndef CONFIG_H_
#define CONFIG_H_
/*
Included libs
*/
#include <avr/io.h>
#include <util/delay.h>

/*

Timer Prescaler Selection bit
When timer is set to Fast PWM Mode, the freqency can be
calculated using equation: F = F_CPU / (N * 256) 

 -> F(N_1) = 4.687kHz
 -> F(N_8) = 585Hz
 -> F(N_64) = 73Hz
 -> F(N_256) = 18Hz
 -> F(N_1024) = 4Hz 
*/
#define N_1    (_BV(CS00))
#define N_8    (_BV(CS01))
#define N_64   (_BV(CS01)|_BV(CS00))
#define N_256  (_BV(CS02))
#define N_1024 (_BV(CS02)|_BV(CS00))

/*Timer mode config*/
#define F_PWM (_BV(WGM01)|_BV(WGM00)|_BV(WGM02))
#define CTC (_BV(WGM01))


/*ADC channel config*/
#define ADC_3 (_BV(MUX1)|_BV(MUX0))

/*
ADC Prescaler Select Bits
*/
#define S_2 _BV(ADPS0)
#define S_4 _BV(ADPS1)
#define S_8 (_BV(ADPS1)|_BV(ADPS0)
#define S_16 _BV(ADPS2)
#define S_128 (_BV(ADPS2)|_BV(ADPS1)|_BV(ADPS0))

/* CPU F*/
#define F_CPU 960000



#endif /* CONFIG_H_ */