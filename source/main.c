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

enum Notes_states{button1, button2, button3, silent, init}Note_state;

unsigned char A0;
unsigned char A1;
unsigned char A2;

void Tick_Note(){
	switch(Note_state){
		case init:
			Note_state = silent;
			break;
		case silent:
			set_PWM(0);
			if(!A0 && !A1 && !A2){
				Note_state = silent;
			}
			else if( A0 && !A1 && !A2){
				Note_state = button1;
			}
			else if( !A0 && A1 && !A2){
				Note_state= button2;
			}
			else if( !A0 && !A1 && A2 ){
				Note_state= button3;
			}
			else if( (A0 + A1 + A2) > 1 ){
			        Note_state= silent;
			}	       
			break;
		case button1:
			set_PWM(261.63);
			if(!A0 && !A1 && !A2){
				Note_state= silent;
			}
			else if( A0 && !A1 && !A2){
				Note_state= button1;
			}
			else if( !A0 && A1 && !A2){
				Note_state= button2;
			}
			else if( !A0 && !A1 && A2 ){
				Note_state= button3;
			}
			else if( (A0 + A1 + A2) > 1 ){
			        Note_state= silent;
			}
			break;
		case button2:
			set_PWM(293.66);
			if(!A0 && !A1 && !A2){
				Note_state= silent;
			}
			else if( A0 && !A1 && !A2){
				Note_state= button1;
			}
			else if( !A0 && A1 && !A2){
				Note_state= button2;
			}
			else if( !A0 && !A1 && A2 ){
				Note_state= button3;
			}
			else if( (A0 + A1 + A2) > 1 ){
			        Note_state= silent;
			}
			break;
		case button3:
			set_PWM(329.63);
			if(!A0 && !A1 && !A2){
				Note_state= silent;
			}
			else if( A0 && !A1 && !A2){
				Note_state= button1;
			}
			else if( !A0 && A1 && !A2){
				Note_state= button2;
			}
			else if( !A0 && !A1 && A2 ){
				Note_state= button3;
			}
			else if( (A0 + A1 + A2) > 1 ){
			        Note_state= silent;
			}
			break;
		default:
			Note_state = silent;
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
	A0 = ~PINA & 0x01;
	A1 = (~PINA & 0x02) >> 1;
	A2 = (~PINA & 0x04) >> 2;
	Tick_Note();
    } 
    return 1;
}
