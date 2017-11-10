// Code for LCD Interfacing with ATmega328P AVR microcontroller

#include <avr/io.h>
#define F_CPU 8000000UL	// internal clock 8mhz no /8
#include <util/delay.h>
#include <string.h>

// Define registers
#define D_OUT PORTC
#define DATA PORTD
#define setDATA DDRD
#define setD_OUT DDRC

// define lcd control pins
#define RS 4
#define E  5

// constants

/************************************************************************/
/* LCD Delay                                                                     */
/************************************************************************/
uint8_t kDelay = 50;

/************************************************************************/
/* Limit base 60                                                                     */
/************************************************************************/
uint8_t kBase60 = 59;

/************************************************************************/
/* Limit base 12                                                                    */
/************************************************************************/
uint8_t kBase12 = 12;

/************************************************************************/
/* ASCII offset value to digit                                                                    */
/************************************************************************/
uint8_t kOffset = 48;

/************************************************************************/
/* ASCII colon                                                                    */
/************************************************************************/
uint8_t kColon = 58;

/************************************************************************/
/* Base 10 divisor                                                                    */
/************************************************************************/
uint8_t kDivisor = 10;

/************************************************************************/
/* Characters for time display                                                                     */
/************************************************************************/
uint8_t kDisplayCharacters = 8;


// declare functions
void send_command (unsigned char command);
void send_data (unsigned char character);

int main(void)
{
	setDATA = 0xFF;		// set DDR register for data as outputs
	setD_OUT = 0xFF;	// set DDR register for digital outputs
	_delay_ms(50);
	send_command(0x01);// sending all clear command
	send_command(0x38);// 16*2 line LCD
	send_command(0x0E);// screen and cursor ON
	
	// declare time counters and set start time
	uint8_t hrs = 11;
	uint8_t mins = 20;
	uint8_t secs = 0;
	// initialize display arrays
	char hours[2] = {0,0};
	char minutes[2] = {0,0};
	char seconds[2] = {0,0};
	// initialize display string
	char time[8] = {hours[0], hours[1], kColon, minutes[0], minutes[1], kColon, seconds[0], seconds[1]};
	// send first line to the LCD display
	char message1[] = "Current Time";
	uint8_t length1 = strlen(message1);
	//First line
	for(uint8_t index = 0; index < length1; index++)
	{
		send_data(message1[index]);
	}
		
	send_command(0xC0); // Move to second line
	
	
	while(1)
	{
		_delay_ms(550); // one second - delays embedded in functions
	
		secs++;
		
		if (secs > kBase60)
		{
			secs = 0;
			mins++;
		}
		else
		{
			//DoNothing();
		}
		if (mins > kBase60)
		{
			mins = 0;
			hrs++;
		}
		else
		{
			//DoNothing();
		}

		if (hrs > kBase12)
		{
			hrs = 1;
		}
		else
		{
			//DoNothing();
		}

		
		if ((secs / kDivisor)>0)
		{
			seconds[0] = ((secs/kDivisor)+kOffset);
			seconds[1] = ((secs%kDivisor)+kOffset);
		}
		else
		{
			seconds[0] = (0+kOffset);
			seconds[1] = (secs+kOffset);
		}
		
		if ((mins / kDivisor)>0)
		{
			minutes[0] = ((mins/kDivisor)+kOffset);
			minutes[1] = ((mins%kDivisor)+kOffset);
		}
		else
		{
			minutes[0] = (0+kOffset);
			minutes[1] = (mins+kOffset);
		}
		
		if ((hrs / kDivisor)>0)
		{
			hours[0] = ((hrs/kDivisor)+kOffset);
			hours[1] = ((hrs%kDivisor)+kOffset);
		}
		else
		{
			hours[0] = (0+kOffset);
			hours[1] = (hrs+kOffset);
		}
		
		time[0] = hours[0];
		time[1] = hours[1];
		time[2] = kColon;
		time[3] = minutes[0];
		time[4] = minutes[1];
		time[5] = kColon;
		time[6] = seconds[0];
		time[7] = seconds[1];
			
		uint8_t length2 = kDisplayCharacters;
		
		send_command(0xC0); // Move to beginning of second line  //adds 50ms delay

		//Second line
		for(uint8_t index = 0; index < length2; index++)
		{
			send_data(time[index]);  //8*50ms delay
		}
		

	}
	
}

// implement functions

void send_command (unsigned char command)
{
	DATA=command;		// command to lcd
	D_OUT &= ~(1<<RS);	// command mode
	D_OUT|= (1<<E);		// enable
	_delay_ms(50);		// allow time to receive
	D_OUT&= ~(1<<E);	// disable
	DATA =0;			// command self destructs
}

void send_data (unsigned char character)
{
	DATA=character;	// 8 bit ascii character
	D_OUT|= (1<<RS);	// character mode
	D_OUT|= (1<<E);		// enable
	_delay_ms(50);		// allow time to receive
	D_OUT&= ~(1<<E);	// disable
	DATA =0;			// message self destructs
}
