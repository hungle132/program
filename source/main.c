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
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#endif


void ADC_init() {
ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
}

volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1;
unsigned long _avr_timer_cntcurr = 0;

void TimerOn(){

TCCR1B = 0x0B;

OCR1A = 125;

TIMSK1 = 0x02;

TCNT1 = 0;

_avr_timer_cntcurr = _avr_timer_M;

SREG |= 0x80;

}

void TimerOff(){
	TCCR1B = 0x00;
}

void TimerISR(){
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect){
	_avr_timer_cntcurr--;
	if(_avr_timer_cntcurr == 0){
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

void TimerSet(unsigned long M){
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}
// Pins on PORTA are used as input for A2D conversion
	//    The default channel is 0 (PA0)
	// The value of pinNum determines the pin on PORTA
	//    used for A2D conversion
	// Valid values range between 0 and 7, where the value
	//    represents the desired pin for A2D conversion
void Set_A2D_Pin(unsigned char pinNum) {
	ADMUX = (pinNum <= 0x07) ? pinNum : ADMUX;
	// Allow channel to stabilize
	static unsigned char i = 0;
	for ( i=0; i<15; i++ ) { asm("nop"); }
}



unsigned char pattern = 0x00;//0x80;
unsigned char row = 0x00;
unsigned char arr1[3] = {0x80,0x80,0x80};
unsigned char arr[3] = {0xFD,0xFB,0xF7};




unsigned char arr2[4] = {0x20,0x10,0x08,0x04};
unsigned char arr3[4] = {0xF1, 0xF5, 0xF5, 0xF1};


enum dis{a,b,c}state;

void led(){

	switch(state){
		case a:
			pattern = arr1[0];
			row = arr[0];
			state = b;
		break;
		case b:
			pattern = arr1[1];
			row = arr[1];
			state = c;
		break;
		case c:
			pattern = arr1[2];
			row = arr[2];
			state = a;
			break;
	default:
		state = a;
	}
	PORTC = pattern;
	PORTD = row;

}


//void Demo_Tick() {


//	for (unsigned int i = 0; i < 4; i++){
//	pattern = arr2[i];
//	row = arr3[i];
//	PORTC = pattern;
//	PORTD = row;
//	}
//}


int main(void) {
    /* Insert DDR and PORT initializations */
DDRA = 0x00; PORTA = 0xFF;
DDRB = 0xFF; PORTB = 0x00;
DDRC = 0xFF; PORTC = 0x00;
DDRD = 0xFF; PORTD = 0x00;

//ADC_init();
TimerOn();
TimerSet(1);
    while (1) {
	led();

	    while(!TimerFlag);
	    TimerFlag = 0;
    }
    return 1;
}
