/*
 * time.c
 *
 *  Created on: May 6, 2020
 *      Author: MyPC
 *      konfigurisemo tajmer da pravi prekide trajanja 1ms
 */
#include <avr/io.h> //definicije registara
#include <interrupt.h> //koristimo prekide

unsigned long sys_time = 0; //vreme krece od 0 i samo se inkrementira pa se stavi unsigned long

ISR(TIMER0_COMPA_vector) //timer CTC mod
{
	sys_time++;
}

void initTime()
{
	TCCR0A = 0X02; //ne koristimo izlazne pinove, tamjer 0 CTC mod

#if F_CPU>20000000
#error "Frekvencija takta mora biti manja od 20MHz"
#endif

	unsigned long n = F_CPU/1000;
	unsigned char clksel = 1; //vrednost bey presklairanja

	while ( n > 255 )
	{
		n/=8;
		clksel ++;
	}

	TCCR0B = clksel;
	OCR0A = unsigned char(n & 0xff) - 1;
	TIMSK0 = 0x02;

	sei();
}

unsigned long  milis ()
{
	unsigned long tmp;

	cli();
	tmp = sys_time;
	sei();
	return tmp;
}

void delay(unsigned long d)
{
	unsigned long t0 = millis();
	while( millis() - t0 < d);
}

