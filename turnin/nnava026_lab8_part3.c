/*	Author: Nicole Navarro
 *  Partner(s) Name: 
 *	Lab Section: 21
 *	Assignment: Lab #8  Exercise #3
 *	Video: https://youtu.be/gutvZlH7sLs
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#endif

void set_PWM(double frequency){
	static double current_frequency;

	if(frequency != current_frequency){
		if(!frequency) {TCCR3B &= 0X08;}
		else { TCCR3B |= 0x03; }

		if(frequency < 0.954) { OCR3A = 0XFFFF; }

		else if(frequency > 31250) { OCR3A = 0X0000; }

		else { OCR3A = (short) (8000000 / (128*frequency)) - 1; }

		TCNT3 = 0;
		current_frequency = frequency;

	}
}

void PWM_on(){
	TCCR3A = (1 << COM3A0);

	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);

	set_PWM(0);
}

void PWM_off(){
	TCCR3A = 0X00;
	TCCR3B = 0X00;
}

enum Notes_states{init, melody, start, offtime}Note_state;

unsigned char A0;
unsigned char count = 0;
unsigned char release = 0;

void Tick_Note(){
	switch(Note_state){
		case init:
			if(A0){
				Note_state = melody;
			}
			break;
		case melody:
			PWM_on();
			if(!A0){
				release = 1;
			}
			if(count == 0){
				set_PWM(261.63);	
			}
			if(count == 1){
				set_PWM(261.63);
			}
			if(count == 2){
				set_PWM(392.00);
			}
			if(count == 3){
				set_PWM(392.00);
			}
			if(count == 4){
				set_PWM(440.00);
			}
			if(count == 5){
				set_PWM(440.00);
			}
			if(count == 6){
				set_PWM(392.00);
			}
			++count;
			Note_state = offtime;
			if(count > 6){
				count = 0;
				Note_state = start;
			}
			break;
		case start:
			PWM_off();
			if(release == 0){
				if(!A0){
					release = 1;
					Note_state = start;
				}
			}
			else if(A0 && release){
				release = 0;
				Note_state = melody;
			}
			else{
				Note_state = start;
			}
			break;
		case offtime:
			Note_state = melody;
			break;
		default:
			Note_state = init;
			break;

	}

}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0X00; PINA = 0XFF;
	DDRB = 0X40; PORTB = 0x00;
	TimerSet(500);
	TimerOn();


	PWM_on();	
    /* Insert your solution below */
	Note_state = init;
	
      while (1) {
	A0 = (~PINA & 0x01);
	Tick_Note();
	while(!TimerFlag){};
	TimerFlag = 0;
    } 

    return 1;
}
