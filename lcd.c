#include "lcd.h"


/*
  Color Code
  White jumper:  vbus
  Black jumper:  Ground
  Brown jumper:  PB0  RS
  Red jumper:    PB1  R/W
  Orange jumper: PB2  EN
  Yellow jumper: PB4  D4
  Green jumper:  PB5  D5
  Blue jumper:   PB6  D6
  Purple jumper: PB7  D7 
*/




void lcd_string1(char *str) // Print on screen with animation
{
  for (int i=0;str[i]!='\0';i++)
  {
    lcd_write_nibble(str[i]);
    if(i>=16)
	{
          delay(50);
          lcd_write_cmd(0x18); //Scrolling text to Right
        }
    delay(25);
  }
  
}

void lcd_string(char *str) // Normal String Printing on screen
{
  for (int i=0;str[i]!='\0';i++)
  {
    lcd_write_nibble(str[i]);
  }
  
}
void lcd_num(int num) // Prints integer
{
  char str[10];
  sprintf(str, "%d", num);
  int i = 0;
  for ( i=0;str[i]!='\0';i++)
  {
    lcd_write_nibble(str[i]);
  }
}

void lcd_float(float num) // Prints floating point numbers with precision of 3
{
   int len = snprintf('\0', 0, "%f", num);
    char flt[10];
  sprintf(flt,"%f", num);
  
for (int  i=0;i<len-3;i++)  { // solves the 0 after decimal point as precision = 3
  if (flt[i]=='.' & flt[i+1]=='0' & flt[i+2] == '0' & flt[i+3] =='0')
    break;
    lcd_write_nibble(flt[i]);
   if (flt[i-1]=='.' & flt[i+1]=='0' & flt[i+2] =='0')
    break;
   else if (flt[i-2] =='.' & flt[i+1]=='0')
     break;
 
 }

}

void lcd_write_nibble(unsigned char data) // sends character to be printed
{

  printdata(data); //  4 digits due to 4 bit mode
  GPIO_PORTB_DATA_R &= (~(1<<1)); // turn off R/W    R/W=0
  GPIO_PORTB_DATA_R |= (1<<0);   // turn on RS   Register select=1
  GPIO_PORTB_DATA_R |= (1<<2); // Enable Writing Enable =1
  delay(2);
  GPIO_PORTB_DATA_R &= (~(1<<2)); // Disable Writing Enable =0
  
  printdata(data<<4);  // Repeat for other 4 digits
  GPIO_PORTB_DATA_R &= (~(1<<1));
  GPIO_PORTB_DATA_R |= (1<<0);
  GPIO_PORTB_DATA_R |= (1<<2);
  delay(2);
  GPIO_PORTB_DATA_R &= (~(1<<2));
}

void lcd_write_cmd(unsigned char command) // sends commands that control lcd not printed
{
  printdata(command); //  4 digits due to 4 bit mode
  GPIO_PORTB_DATA_R &= (~(1<<1)); // turn off R/W    R/W=0
  GPIO_PORTB_DATA_R &= (~(1<<0)); // turn off RS   Register select=0
  GPIO_PORTB_DATA_R |= (1<<2); // Enable Writing Enable =1
  delay(2);
  GPIO_PORTB_DATA_R &= (~(1<<2)); // Disable Writing Enable =0

  printdata(command<<4); // Repeat for other 4 digits
  GPIO_PORTB_DATA_R &= (~(1<<1));
  GPIO_PORTB_DATA_R &= (~(1<<0));
  GPIO_PORTB_DATA_R |= (1<<2);
  delay(2);
  GPIO_PORTB_DATA_R &= (~(1<<2));
}

void printdata(unsigned char data)
{
  if((data&0x10)==0x10) // check if to signal send on d4
  {
  GPIO_PORTB_DATA_R |=(1<<4);
  }
  else
  {
  GPIO_PORTB_DATA_R &=~(1<<4);
  }
  if((data&0x20)==0x20) // check if to send signal on d5
  {
  GPIO_PORTB_DATA_R |=(1<<5);
  }
  else
  {
  GPIO_PORTB_DATA_R &=~(1<<5);
  }  
  if((data&0x40)==0x40) // check if to send signal on d6
  {
  GPIO_PORTB_DATA_R |= (1<<6);
  }
  else
  {
  GPIO_PORTB_DATA_R &=~(1<<6);
  }  
  if((data&0x80)==0x80) // check if to send signal on d7
  {
  GPIO_PORTB_DATA_R |=(1<<7);
  }
  else
  {
  GPIO_PORTB_DATA_R &=~(1<<7);
  }
}



	