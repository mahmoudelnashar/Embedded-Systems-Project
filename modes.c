#include "modes.h"


unsigned char stopwatch_read;  // reads the start command in stopwatch
int start_stop=0;             // used to toggle start and stop in stopwatch
char timer_val[2];








void lcd_init()
{

  SYSCTL_RCGCGPIO_R |= 0x03;       //activate clock for Port B
  while((SYSCTL_PRGPIO_R & 0x03) == 0 ){}
  GPIO_PORTB_DEN_R = 0xF7;     // Enable all pins of PORTB Except Pin 3 unused
  GPIO_PORTB_DIR_R = 0xF7;    // Make all enabled pins of PORTB as ouptut pin    						
  
  
  GPIO_PORTA_DEN_R =0xE0;
  GPIO_PORTA_PUR_R |= 0xE0; 

  GPIOIntRegister(GPIO_PORTA_BASE,IntSwitchMode);
  GPIOIntTypeSet( GPIO_PORTA_BASE,GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7,GPIO_FALLING_EDGE);
  GPIOIntEnable(GPIO_PORTA_BASE,GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);
    IntPrioritySet(INT_GPIOA , 0xe0);

  IntMasterEnable();
  
  delay(25);
  lcd_write_cmd(0x28);
  delay(5);
  lcd_write_cmd(0x28);
  delay(1);
  lcd_write_cmd(0x28);
  lcd_write_cmd(0x06);  // auto increment cursor
  lcd_write_cmd(0x0c);  // cursor off display on
  lcd_write_cmd(0x01);  // clearscreen
  
    delay(25);
  lcd_write_cmd(0x28);
  delay(5);
  lcd_write_cmd(0x28);
  delay(1);
  lcd_write_cmd(0x28);
  lcd_write_cmd(0x06);  // auto increment cursor
  lcd_write_cmd(0x0c);  // cursor off display on
  lcd_write_cmd(0x01);  // clearscreen
  
  lcd_string1("Embedded Project");
  lcd_write_cmd(0xc0);
  lcd_string1("Team 7");
  delay(500);

  
}

void lcd_modes(void){
  
  lcd_write_cmd(0x01);
  lcd_write_cmd(0x80);
  lcd_string("Different Modes");
  lcd_write_cmd(0xc0);
  lcd_string1("1. Calculator");
  delay(1000);
  
  lcd_write_cmd(0x01);
  lcd_write_cmd(0x80);
  lcd_string("Different Modes"); 
  lcd_write_cmd(0xc0);  
  lcd_string1("2. Timer");
  delay(1000);

  lcd_write_cmd(0x01);
  lcd_write_cmd(0x80);
  lcd_string("Different Modes"); 
  lcd_write_cmd(0xc0);  
  lcd_string1("3. Stopwatch");
  delay(1000);
  
  lcd_write_cmd(0x01);
  lcd_write_cmd(0x80);
  lcd_string("Use Buttons to"); 
  lcd_write_cmd(0xc0);  
  lcd_string1("Select a Mode");
  delay(1000);

  
}
void calculator_mode()
{
  lcd_write_cmd(0x01);
  lcd_string1("Calculator Mode");
  delay(2000);
  lcd_write_cmd(0x01);
  
  calculator();
  
  lcd_write_cmd(0x01);
  lcd_string1("Calculator off");
  delay(2000);
}


  
  




void stopwatch_mode()
{
  lcd_write_cmd(0x0c);
  lcd_write_cmd(0x01);
  lcd_write_cmd(0x80);
  lcd_string1("Stopwatch Mode"); 
  Timer0_init();
  delay(2000);
  
  lcd_write_cmd(0x01);
  lcd_write_cmd(0x80);
  lcd_string1("# to Start/Stop"); 

  lcd_write_cmd(0xc0);
  lcd_string("     ");
  lcd_string("00");
  lcd_string(":");
  lcd_string("00");
    
    
    while(1){
    stopwatch_read=KeyPad_Read();  
     if(stopwatch_read!='.')
    {
          TimerDisable(TIMER0_BASE, TIMER_A);
        lcd_write_cmd(0x01);
      lcd_string1("Stopwatch off"); 
      delay(1000);
      break;
    }
    if((stopwatch_read=='.') & (start_stop==0)) { 
      TimerEnable(TIMER0_BASE , TIMER_A);
      start_stop=1;
      continue;
    }
    if((stopwatch_read=='.') & (start_stop==1)) {
      TimerDisable(TIMER0_BASE, TIMER_A);
      start_stop=0;
      continue;
    }
          }

}


void timer_mode()
{
  lcd_write_cmd(0x0c);
  lcd_write_cmd(0x01);
  lcd_string1("Timer Mode");
  delay(2000);
  
  lcd_write_cmd(0x01);
  lcd_string1("Enter Minutes:");
    for (int i =0;i<2;i++)
    {
      timer_val[i]=KeyPad_Read();
      lcd_write_nibble(timer_val[i]);
      delay(250);
    }
    int min = atoi(timer_val);
    
   lcd_write_cmd(0xc0);
   lcd_string1("Enter Seconds:");
   
   for (int i =0;i<2;i++)
    {
      timer_val[i]=KeyPad_Read();
      lcd_write_nibble(timer_val[i]);
      delay(250);
    }
   int sec = atoi(timer_val);
    
  timer(min,sec);
  
}


void IntSwitchMode(void)
{
    GPIOIntClear( GPIO_PORTA_BASE,GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);
    TimerIntUnregister(TIMER0_BASE, TIMER_A);
    TimerIntUnregister(TIMER1_BASE, TIMER_A);

     
if(GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_5)==0)
{
  while(GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_5)==0){}
  calculator_mode();
}
  
else if(GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_6)==0)
{
  while(GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_6)==0){}
  timer_mode();

}
else if(GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_7)==0)
{
  while(GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_7)==0){}
  stopwatch_mode();

}
}
