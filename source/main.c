/*	Author: lab
 *  Partner(s) Name: 
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
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

enum Notes_states{init, on, off, change, release}Note_state;

unsigned char A0;
unsigned char A1;
unsigned char A2;
unsigned char count = 0;

void Tick_Note(){
	switch(Note_state){
	
	case init:
		Note_state = on;
		break;
	case off:
		PWM_off();
		if(A0){
			Note_state = on;
		}
		else{
			Note_state = off;
		break;
	case on:
		PWM_on();
		set_PWM(261.63);
		count = 1;
		if(!A0){
			Note_state = off;
		}
		else{
			Note_state = change;
		}
		break;
	case change:
		if(count == 1){
			set_PWM(261.63);
		}
		if(count == 2){
			set_PWM(293.66);
		}
		if(count == 3){
			set_PWM(329.63);
		}
		if(count == 4){
			set_PWM(349.23);
		}
		if(count == 5){
			set_PWM(392.00);
		}
		if(count == 6){
			set_PWM(440.00);
		}
		if(count == 7){
			set_PWM(493.88);
		}
		if(count == 8){
			set_PWM(523.25);
		}
		if(A1){
			++count;
			if(count > 8){
				count = 8;
			}
			Note_state = change;
		}
		else if(A2){
			--count;
			if(count < 1){
				count = 1;
			}
			Note_state = change;
		}
		break;
	case default:
		Note_state = init;
		break;

	}

}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0X00; PINA = 0XFF;
	DDRB = 0X40; PORTB = 0x00;

	PWM_on();	
    /* Insert your solution below */
	Note_state = init;
	
      while (1) {
	A0 = (~PINA & 0x01); //on & off
	A1 = (~PINA & 0x02) >> 1; //up
	A2 = (~PINA & 0x04) >> 2; //down
	Tick_Note();


    } 
    return 1;
}
