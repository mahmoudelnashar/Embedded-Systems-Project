#include "lcd.h"


/*
  Color Code
  White jumper:  +3.3 V
  Black jumper:  Ground
  Brown jumper:  PB0  RS
  Red jumper:    PB1  R/W
  Orange jumper: PB2  EN
  Yellow jumper: PB4  D4
  Green jumper:  PB5  D5
  Blue jumper:   PB6  D6
  Purple jumper: PB7  D7 
*/




void lcd_string1(char *str)
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

void lcd_string(char *str)
{
  for (int i=0;str[i]!='\0';i++)
  {
    lcd_write_nibble(str[i]);
  }
  
}
void lcd_num(int num)
{
  char str[10];
  sprintf(str, "%d", num);
  int i = 0;
  for ( i=0;str[i]!='\0';i++)
  {
    lcd_write_nibble(str[i]);
  }
}

void lcd_float(float num)
{
   int len = snprintf('\0', 0, "%f", num);
    char flt[10];
  sprintf(flt,"%f", num);
  
for (int  i=0;i<len-3;i++)  {
  if (flt[i]=='.' & flt[i+1]=='0' & flt[i+2] == '0' & flt[i+3] =='0')
    break;
    lcd_write_nibble(flt[i]);
   if (flt[i-1]=='.' & flt[i+1]=='0' & flt[i+2] =='0')
    break;
   else if (flt[i-2] =='.' & flt[i+1]=='0')
     break;
 
 }

}

void lcd_write_nibble(unsigned char data)
{

  printdata(data);
  GPIO_PORTB_DATA_R &= (~(1<<1)); //turn off R/W
  GPIO_PORTB_DATA_R |= (1<<0);   // turn on RS
  GPIO_PORTB_DATA_R |= (1<<2);
  delay(2);
  GPIO_PORTB_DATA_R &= (~(1<<2));
  
  printdata(data<<4);
  GPIO_PORTB_DATA_R &= (~(1<<1));
  GPIO_PORTB_DATA_R |= (1<<0);
  GPIO_PORTB_DATA_R |= (1<<2);
  delay(2);
  GPIO_PORTB_DATA_R &= (~(1<<2));
}

void lcd_write_cmd(unsigned char command)
{
  printdata(command);
  GPIO_PORTB_DATA_R &= (~(1<<1));
  GPIO_PORTB_DATA_R &= (~(1<<0));
  GPIO_PORTB_DATA_R |= (1<<2);
  delay(2);
  GPIO_PORTB_DATA_R &= (~(1<<2));

  printdata(command<<4);
  GPIO_PORTB_DATA_R &= (~(1<<1));
  GPIO_PORTB_DATA_R &= (~(1<<0));
  GPIO_PORTB_DATA_R |= (1<<2);
  delay(2);
  GPIO_PORTB_DATA_R &= (~(1<<2));
}

void printdata(unsigned char data)
{
  if((data&0x10)==0x10)
  {
  GPIO_PORTB_DATA_R |=(1<<4);
  }
  else
  {
  GPIO_PORTB_DATA_R &=~(1<<4);
  }
  if((data&0x20)==0x20)
  {
  GPIO_PORTB_DATA_R |=(1<<5);
  }
  else
  {
  GPIO_PORTB_DATA_R &=~(1<<5);
  }  
  if((data&0x40)==0x40)
  {
  GPIO_PORTB_DATA_R |= (1<<6);
  }
  else
  {
  GPIO_PORTB_DATA_R &=~(1<<6);
  }  
  if((data&0x80)==0x80)
  {
  GPIO_PORTB_DATA_R |=(1<<7);
  }
  else
  {
  GPIO_PORTB_DATA_R &=~(1<<7);
  }
}



	