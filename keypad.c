#include "keypad.h"

//MAP
// Port E 0 to 3 Output Yellow Green Blue Purple
// Port C 4 to 7 Input  White Brown Red

unsigned char key44[4][4] = 
{
  {'1', '2', '3', '+'},
  {'4', '5', '6', '-'},
  {'7', '8', '9', '/'},
  {'x', '0','.', '='}
};


void keyPad_Init()
{
  SYSCTL_RCGCGPIO_R |= 0x14;       //activate clock for Port C & E
  while((SYSCTL_PRGPIO_R & 0x14) == 0 ){}
  
  
  GPIO_PORTC_CR_R  |= 0xF0;
  GPIO_PORTC_PDR_R |= 0xF0; 
  GPIO_PORTC_DEN_R |= 0xF0;     // Enable Required pins of PORTC all inputs
  
  GPIO_PORTC_CR_R  |= 0x1F;
  GPIO_PORTE_DIR_R |= 0x1F;
  GPIO_PORTE_DEN_R |= 0x1F;   //Enable Required pins of PORTE all outputs

}



char KeyPad_Read()
{
  while(1)
  {
  for(int i = 0 ; i <4; i++)
  {
     GPIO_PORTE_DATA_R = (1U << i);  
     delay(5);
    for(int j = 0; j < 4; j++)
    {
      if((GPIO_PORTC_DATA_R & 0xF0) & (1U << j+4))
      { 
        while((GPIO_PORTC_DATA_R & 0xF0) & (1U << j+4)){
        delay(50);
        }
      return key44[i][j];
      }
    }


  }   
  }
}


void add(int num1,int num2)
{
  lcd_num(num1+num2);
  
}