// PB0  Output Joy2 fire
// PB1  Input Joy2 up                                __________
// PB2  Input Joy2 down                     Reset   |1       28|    PC5  (switch)
// PB3  Input Joy2 left                 /   PD0     |2       27|    PC4   \
// PB4  Input Joy2 right                    PD1     |3   A   26|    PC3     
// PB5  Input Joy2 fire     Joy1 output     PD2     |4   T   25|    PC2     Joy1 Input
// PB6  N.C                                 PD3     |5   m   24|    PC1
// PB7  Output Joy2 up                  \   PD4     |6   e   23|    PC0   /
// PC0  Input Joy1 up                       VCC     |7   g   22|    GND
// PC1  Input Joy1 down                     GNC     |8   a   21|    ARef
// PC2  Input Joy1 left                 N.C PB6     |9   3   20|    AVCC
// PC3  Input Joy1 right                /   PB7     |10  2   19|    PB5   \
// PC4  Input Joy1 fire                     PD5     |11  8   18|    PB4
// PC5  Switch              Joy2 output     PD6     |12  p   17|    PB3     Joy 2 Input
// PD0  Output Joy1 up                      PD7     |13      16|    PB2
// PD1  Output Joy1 down                \   PB0     |14______15|    PB1   /
// PD2  Output Joy1 left
// PD3  Output Joy1 right
// PD4  Output Joy1 fire
// PD5  Output Joy2 down
// PD6  Output Joy2 left
// PD7  Output Joy2 right 

#include <avr/io.h>

int main()
{
	// change system clock prescaler from 1/8 (default) to 1/32
	// drops power consumption from 3.15mA to 2.54mA
	CLKPR = 0x80;
	CLKPR = 0x05;

	PRR = 0b01101011;		// shutdown timers, USART, ADC

	DDRC = 0;
	PORTC = 0b00111111;	// enable pull-ups for PC0 - PC5  
	PORTB = 0b00111110;	// enable pull-ups for PB1 - PB5
	PORTD = 0;

	while(true)
	{
		// read joysticks
		uint8_t joy1 = PINC & 0b00011111;
		uint8_t joy2 = (PINB >> 1) & 0b00011111;

		// swap if switch is on
		if((PINC & 0b00100000) == 0)
		{
			uint8_t t = joy1;
			joy1 = joy2;
			joy2 = t;
		}

		// output joystick state
		// output only LOW or HI-Z, never HIGH as this will cause a short circuit when certain keys on the C64 keyboard are pressed!
		// (joysticks are connected to the keyboard matrix in a C64)

		uint8_t d = joy1 | ((joy2 << 4) & 0b11100000);
		uint8_t b = ((joy2 >> 4) & 1) | ((joy2 << 7) & 0x80);

		DDRD = d ^ 0xff;	// enable output for each low bit 
		DDRB = (b ^ 0xff) & 0b10000001;
	}	

	return 0;
}
