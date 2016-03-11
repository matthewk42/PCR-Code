/*
 * H_C_Test.c
 *
 * Created: 3/7/2016 3:59:38 PM
 *  Author: martijus
 */ 


//#Defines:
#define CPU_PRESCALE(n) (CLKPR = 0x80, CLKPR = (n))
#define CPU_16MHz 0x00
#define F_CPU CPU_16MHz 

// Libraries:
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "usb_serial.h"
#include "temp.h"
#include "Timer.h"
#include "ADC.h"

volatile unsigned int count_ms = 0;
volatile unsigned int printFlag = 0;
void Counter_init (void);

ISR(TIMER1_COMPA_vect){
	count_ms ++;
	printFlag = 1;
}

// Testing:
#define LED_ON		(PORTD	|= (1<< PD6))	// Toggle Teensy LED on.
#define LED_OFF		(PORTD	&= ~(1<<PD6))	// Toggle Teensy LED off.

// Heating:
#define Heat_ON		(PORTB	|= (1<< PB2))	// Toggle H-bridge channel 1 on.
#define Heat_OFF	(PORTB	&= ~(1<<PB2))	// Toggle H-bridge channel 1 off.

// Fan:
#define Fan_ON		(PORTB	|= (1<< PB3))	// Toggle H-bridge channel 2 on.
#define Fan_OFF		(PORTB	&= ~(1<<PB3))	// Toggle H-bridge channel 2 off.

//Function declarations:
//void init(void);
void port_init(void);
void init(void);

// Global Variables:
uint8_t input;
uint8_t targetTime=30;
uint8_t targetTemp=96;
uint8_t annTime;
uint8_t annTemp;
uint8_t eloTime;
uint8_t eloTemp;
char passData[19];

void send_serial (uint16_t, uint16_t);
void send_strB(char *s);
double convertTemp(double);


int main(void)
{
	int isTime = 0;
	int next = 0;
	double readTemp;
	double lowerRange = 0;
	double upperRange = 0;
	init();
	
	lowerRange = targetTemp;// + 0.1;
	upperRange = targetTemp;// + 0.1;
				
	while(next != 1){
		
		while(count_ms < 4*targetTime){
			
			readTemp = getTemp();
			/*
			//send_serial("Denaturing temp is "); //send App
			uart_putchar(denTemp);
			uart_putchar('\n');
			_delay_ms(1000);
			//send_serial("readTemp is "); //send App
			uart_putchar(readTemp);
			uart_putchar('\n');
			_delay_ms(1000);
			*/

			if (printFlag == 1){
				send_serial(readTemp*10, count_ms);	// count_ms is incremented with a timer interrupt every quarter second
				printFlag = 0;				// the effect is that the read temp is printed every quarter second
			
	
				_delay_ms(1);// don't forget about the delay in the adc_read function!
			
				if (readTemp < lowerRange){
					OCR0A = (82 - (targetTemp-55)*2);//normalized to PWM @ 82 at 55C and increase duty cycle by 2 PWM values for each degree
					PORTB = Fan_OFF;
					PORTD = LED_OFF;
				}
				else{
					OCR0A = 255; // 0% duty cycle
					PORTB = Fan_OFF;
					PORTD = LED_ON;
				}
				/*
				if (readTemp > lowerRange && readTemp < upperRange)
				{
					OCR0A = 128; //PWM duty cycle set to 0% PORTB = Heat_OFF; //
					PORTB = Fan_OFF;
					PORTD = LED_ON;
				}
				*/
		
				if (readTemp > (upperRange + 0.9)){
					OCR0A = 255; //PWM duty cycle set to 0% PORTB = Heat_OFF; //
					PORTB = Fan_ON;
					PORTD = LED_OFF;
				}
		
				if (((readTemp*10)/10 >= targetTemp+1) || ((readTemp*10)/10 <= targetTemp-1)){ // if current temperature is outside of +/- 1C range
					count_ms = 0; // reset the counter if not in temperature range			
				}
			}			
		}		
		
		while(1){
			if (printFlag == 1){
				send_serial(1010, 1010);
				printFlag = 0;		// printed every quarter second
			}
		}
		
	}	
	
}


void init(void){
	
	CPU_PRESCALE(CPU_16MHz);
	adc_init();
	Counter_init();
	port_init();
	usb_init();
	pwm_init();
	sei();
}



void port_init(void){
	
	DDRB |= ((1<<PB1)|(1<<PB2)|(1<<PB3)) ; // Sets the data direction for Port B PB1 (Relay Control), PB2 is Heating PB3 is fan as an output.
	DDRF |= (0<<PF0) | (0<<PF1); // Sets the data direction for port F, PF0 (ADC in), PF1 (Low Battery Signal) as an input.
	DDRD |= (1<<6); // Sets the data direction for port D , PD6 (Teensy LED) as an output.
	
	PORTB = 0b00000000; // Sets all PB pin values low.
	PORTF = 0b00000000; // Sets all PF pins low.
	PORTD = 0b00000000; // Sets all PD pins low.

}

void pwm_init(void){
	
	TCCR0A = (1<<COM0A1)|(1<<COM0A0)|(1<<WGM01)|(1<<WGM00); //Fast PWM mode, clear on compare match
	TCCR0B = (0<<WGM02)|(1<<CS02)|(0<<CS01)|(1<<CS00); // pre-scaler 1024
	DDRB |= (DDRB |= (1<<PB7)); //sets PORTB Pin 7 (OCOA -- Timer/Counter0 output) as an output pin for PWM
	OCR0A = 255; //default output to 0% duty cycle
}

void send_serial (uint16_t out, uint16_t count){
	char temp_string[32];
	usb_serial_flush_input();	// we empty the data line before sending.
	
	// sprintf will take multiple data types and turn the whole line into a string.
	sprintf(temp_string, "Temp: %u | Count: %u \n", out, count/4);
	send_strB(temp_string);
}
// this function takes all the characters and feeds them into the usb_serial data stream.
void send_strB(char *s){
	int i = 0;
	while (s[i] != '\0') {
		usb_serial_putchar(s[i]);
		i++;
	}
}