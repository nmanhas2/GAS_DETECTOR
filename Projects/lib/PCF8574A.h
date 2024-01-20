// header for PCF8574A - Common Port Expander for LCD Backpack in Arduino World
// Simon Walker, NAIT
// Revision History:
// March 22 2022 - Initial Build

int LCD_Init ();
//int PCF8574A_Write (unsigned char ucData);
//int PCF8574A_Read (unsigned char * Target);

void LCD_Clear (void);
void lcd_Addr (unsigned char addr);
void lcd_AddrXY (unsigned char ix, unsigned char iy);
void LCD_String (char * straddr);
void LCD_StringXY (unsigned char ix, unsigned char iy, char * straddr);
void LCD_DispControl (char curon, char blinkon, char dispon);