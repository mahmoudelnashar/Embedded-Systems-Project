#include "modes.h"


int main()
{
  lcd_init(); // initializes the lcd and some ports
  keyPad_Init(); // initializes the keypad and some ports
  
  while(1)
  {
    lcd_modes(); // Main screen
    
  }


}



  


	