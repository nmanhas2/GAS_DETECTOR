// implementation for PCF8574A - Common Port Expander for LCD Backpack in Arduino World
// Simon Walker, NAIT

#include <compiler.h>
#include "I2C.h"
#include "PCF8574A.h"

// this device uses weird bidirectional pins
//  it would make sense to leave the pins as inputs
//  when not actually writing commands out to the device
// (data direction should follow R/W signal)
// write all 1s to make the 'outputs' weak pull-ups
// this device also uses a 4-bit interface (sigh)

// don't forget that E is an active high signal,
//  so it will need to be forced low when the device
//  is not selected

// P7 P6 P5 P4 P3 P2 P1 P0
// D7 D6 D5 D4 BL  E RW RS

union
{
	unsigned char Byte;
	struct
	{
		unsigned char RS	:1;
		unsigned char RW	:1;
		unsigned char E		:1;
		unsigned char BL	:1;
		unsigned char Data	:4;
	} Bits;
} LCD_PORT;

// private helpers
int PCF8574A_Write (unsigned char ucData)
{
	if (I2C_Start(0x3F, false))
	return -1;

	if (I2C_Write8(ucData, true))
	return -2;
	
	return 0;
}

int PCF8574A_Read (unsigned char * Target)
{
	if (I2C_Start(0x3F, true))
	return -1;

	if (I2C_Read8(Target, false, true))
	return -2;
	
	return 0;
}

// replace with correct timing from timer sleep function
void LCD_CmdDelay ()
{
	for (unsigned int i = 0; i < 60000u; i++)
		;
}

int LCD_WritePort ()
{
	if (PCF8574A_Write(LCD_PORT.Byte))
		return -1;
	return 0;
}

int LCD_ReadPort ()
{
	if (PCF8574A_Read(&LCD_PORT.Byte))
		return -1;
	return 0;
}

int LCD_Busy ()
{
	int bBusy = 0;

	// prepare pins for reading
	LCD_PORT.Bits.Data = 0b1111;
	LCD_PORT.Bits.E = 0;
	LCD_PORT.Bits.RW = 1;
	LCD_PORT.Bits.RS = 0;
	if (LCD_WritePort())
		return -1;
	LCD_CmdDelay();

	do 
	{	
		// now read state of busy bit (must read two nibbles)
		LCD_PORT.Bits.E = 1;
		if (LCD_WritePort())
			return -1;
		LCD_CmdDelay();

		if (LCD_ReadPort())
			return -2;

		bBusy = LCD_PORT.Bits.Data & 0b1000;

		LCD_PORT.Bits.Data = 0b1111;
		LCD_PORT.Bits.E = 0;
		LCD_PORT.Bits.BL = 1;
		if (LCD_WritePort())
			return -1;
		LCD_CmdDelay();

		LCD_PORT.Bits.E = 1;
		if (LCD_WritePort())
			return -1;
		LCD_CmdDelay();
	
		if (LCD_ReadPort())
			return -2;				

		LCD_PORT.Bits.Data = 0b1111;
		LCD_PORT.Bits.E = 0;
		LCD_PORT.Bits.BL = 1;
		if (LCD_WritePort())
			return -1;
		LCD_CmdDelay();
	}
	while (bBusy);

	return 0;
}

int LCD_Inst (unsigned char Value)
{
	LCD_Busy();
	
	// switch to 8-bit interface
	LCD_PORT.Bits.Data = Value >> 4;
	LCD_PORT.Bits.E = 1;
	LCD_PORT.Bits.RW = 0;
	LCD_PORT.Bits.RS = 0;
	if (LCD_WritePort())
		return -1;
	LCD_CmdDelay();
	LCD_PORT.Bits.E = 0;
	if (LCD_WritePort())
		return -1;
	LCD_CmdDelay();
	
	LCD_PORT.Bits.Data = Value & 0x0f;
	LCD_PORT.Bits.E = 1;
	if (LCD_WritePort())
		return -1;
	LCD_CmdDelay();
	LCD_PORT.Bits.E = 0;
	if (LCD_WritePort())
		return -1;
	LCD_CmdDelay();

	return 0;
}

int LCD_Data (unsigned char Value)
{
	LCD_Busy();
	
	// switch to 8-bit interface
	LCD_PORT.Bits.Data = Value >> 4;
	LCD_PORT.Bits.E = 1;
	LCD_PORT.Bits.RW = 0;
	LCD_PORT.Bits.RS = 1;
	if (LCD_WritePort())
		return -1;
	LCD_CmdDelay();
	LCD_PORT.Bits.E = 0;
	if (LCD_WritePort())
		return -1;
	LCD_CmdDelay();
	
	LCD_PORT.Bits.Data = Value & 0x0f;
	LCD_PORT.Bits.E = 1;
	if (LCD_WritePort())
		return -1;
	LCD_CmdDelay();
	LCD_PORT.Bits.E = 0;
	if (LCD_WritePort())
		return -1;
	LCD_CmdDelay();

	return 0;
}

int LCD_Init ()
{
	// all high but E
	LCD_PORT.Byte = 0b11111011;
	if (LCD_WritePort())
		return -1;

	// delay to wait for E to unclog
	LCD_CmdDelay();

	for (int i = 0; i < 2; ++i)
	{
		// switch to 8-bit interface
		LCD_PORT.Bits.Data = 0x03;
		LCD_PORT.Bits.E = 1;
		LCD_PORT.Bits.RW = 0;
		LCD_PORT.Bits.RS = 0;
		if (LCD_WritePort())
			return -1;
		LCD_CmdDelay();
		LCD_PORT.Bits.E = 0;
		if (LCD_WritePort())
			return -1;	
		LCD_CmdDelay();
	}

	// switch to 4-bit interface
	LCD_PORT.Bits.Data = 0x02;
	LCD_PORT.Bits.E = 1;
	if (LCD_WritePort())
	return -1;
	LCD_CmdDelay();
	LCD_PORT.Bits.E = 0;
	if (LCD_WritePort())
	return -1;
	LCD_CmdDelay();

	LCD_Inst(0x28); // 4-bit interface, 2 lines, 5x7 characters
	LCD_Inst(0x0c); // display on, blink and cursor off
	LCD_Inst(0x06); // increment address on write, no shift
	LCD_Inst(0x02); // home
 	LCD_Inst(0x01); // clear

	return 0;
}

// set addr in A to LCD
void LCD_Addr (unsigned char addr)
{
	addr |= 0x80;
	LCD_Inst (addr);
}

// set addr in A to LCD
void LCD_AddrXY (unsigned char ix, unsigned char iy)
{
	unsigned char phoffset = 0;
	
	// address for LCD is
	// 00
	// 40
	// 14
	// 54
	
	// range check
	if (iy > 3)
		return;
	if (ix > 19)
		return;
	
	// calculate address offset
	if (!iy)
		phoffset = 0x00;
	else if (iy == 1)
		phoffset = 0x40;
	else if (iy == 2)
		phoffset = 0x14;
	else
		phoffset = 0x54;
	
	phoffset += ix;
	
	phoffset |= 0x80; // make into dd addr command
	
	LCD_Inst (phoffset);
}

void LCD_String (char * straddr)
{
	for (; *straddr; ++straddr)
		LCD_Data (*straddr);
}

// start the string at X/Y
void LCD_StringXY (unsigned char ix, unsigned char iy, char * straddr)
{
	// range check
	if (iy > 3)
	return;
	if (ix > 19)
	return;

	LCD_AddrXY (ix, iy);
	
	// show the string...
	LCD_String (straddr);
}

// clear the display
void LCD_Clear (void)
{
	LCD_Inst (0x01);
}

void LCD_DispControl (char curon, char blinkon, char dispon)
{
	unsigned char bval = 0b00001000; // command = display control
	
	if (dispon)
		bval |= 0x04;         // add disp on
	if (curon)
		bval |= 0x02;         // add cursor
	if (blinkon)
		bval |= 0x01;         // add blink
	
	LCD_Inst (bval);
}
