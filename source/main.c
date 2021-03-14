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

unsigned char p1 = 0x00;
unsigned char r1 = 0x00;
unsigned char arr3[3] = {0x01,0x01,0x01};
unsigned char arr2[3] = {0xFD,0xFB,0xF7};




enum dis{a,b,c}state;
enum dis1{a1,a2,a3}s;
enum joy{start,wait,up,down,but1,but2}move;
enum but{up1,down1,wait1}m;
unsigned short vak = 0x00;
unsigned char flag = 0x01;
unsigned char flag1 = 0x01;
unsigned char d = 0x00;
unsigned char u = 0x00;
//unsigned char ball = 0x08;
void joystick(){
//	TimerSet(1);
	d = ~PINA & 0x01;
	u = ~PINA & 0x02;
	switch(move){
		case start:
			move = wait;
			break;
		case wait:
			if (u){
				move = up;
			}
			else if (d){
				move = down;
			}
			else{
				move = wait;
			}
			break;
		case up:

		if (flag != 0){
			for (unsigned int i = 0; i < 3; i++){
			arr[i] = arr[i] >> 1;
			}
			flag--;
			flag1++;
			}
		move = but1;

			break;
		case down:

		if (flag1 != 0){
		for (unsigned int i = 0; i < 3; i++){
		arr[i] = arr[i] << 1 | 0x01;
		}
		flag1--;
		flag++;
		}
		move = but2;
			break;

		case but1:
			if (u){
			move = but1;
			}
			else if (!u){
			move = wait;
			}
			break;
		case but2:
			if (d){
			move = but2;
			}	
			else if (!d){
			move = wait;
			}
		default:
			
			move = start;
	}
	switch(move){
		case start:
			break;
		case wait:
			break;
		case up:
			

			break;

		case down:

			break;
		case but1:
			break;
		case but2:
			break;
	}
		
}
void led(){
//	TimerSet(1);
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

void show(){
//	TimerSet(1);
	switch(s){

		case a1:
			p1 = arr3[0];
			r1 = arr2[0];
			s = a2;
			break;
		case a2:
			p1 = arr3[1];
			r1 = arr2[1];
			s = a3;
			break;
		case a3:
			p1 = arr3[2];
			r1 = arr2[2];
			s = a1;
			break;
		default:
			s = a1;
			break;
	}
	PORTC = p1;
	PORTD = r1;
}
unsigned char ballrow = 0xFB;//middlerow
unsigned char ballchecker = 0x04;
unsigned char ball = 0x08;
unsigned char test = 0x10;
unsigned char test1 = 0xBF;
//unsigned long balltime = 0x00;
unsigned char ballrowarr[1] = {0xFB};
unsigned char ballcol[2] = {0x08,0x10,0x20,0x40};
unsigned char col = 0x00;
unsigned char r = 0x00;
enum balls{startball,bounce,bounce1,bounce2,bounce3,reset} bal;
void ballm(){
	//TimerSet(1000);
	switch(bal){
	case startball:
		bal = bounce;
		break;
	case bounce:
			r = 0xFB;
			col = 0x08;
			bal = bounce1;

		break;

	case bounce1:
		r = 0xFB;
		col = 0x10;
		bal = bounce2;
		break;
	case bounce2:
		r = 0xFB;
		col = 0x20;
		bal = bounce3;
		break;
	case bounce3:
		r = 0xFB;
		col = 0x40;
		bal = bounce;
		break;
	case reset:
		break;
	default:
		bal = startball;
		break;
	}
	switch(bal){
	case startball:
		break;
	case bounce:

		break;

	case bounce1:
		break;

	case bounce2:
	break;
	case bounce3:
	break;
	case reset:
	break;
	}


	
	

	PORTC = col;
	PORTD = r;
}





int main(void) {
    /* Insert DDR and PORT initializations */
DDRA = 0x00; PORTA = 0xFF;
DDRB = 0x00; PORTB = 0xFF;
DDRC = 0xFF; PORTC = 0x00;
DDRD = 0xFF; PORTD = 0x00;
unsigned long balltime = 0;
ADC_init();
TimerOn();
TimerSet(1);
    while (1) {
	show();
	led();
	joystick();
	if(balltime >= 8000){
	ballm();
	balltime = 0;
	}
   while(!TimerFlag);
    TimerFlag = 0;
    balltime+=10;
    }
    return 1;
}
