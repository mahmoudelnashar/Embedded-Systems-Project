#include "modes.h"


unsigned char stopwatch_read;  // reads the start command in stopwatch
int start_stop=0;             // used to toggle start and stop in stopwatch
char timer_val[2];


// Push buttons on port A





void lcd_init()
{

  SYSCTL_RCGCGPIO_R |= 0x03;       // activate clock for Port B and Port A
  while((SYSCTL_PRGPIO_R & 0x03) == 0 ){} // wait for clock
  GPIO_PORTB_DEN_R = 0xF7;     // Enable all pins of PORTB Except Pin 3 unused
  GPIO_PORTB_DIR_R = 0xF7;    // Make all enabled pins of PORTB as ouptut pin    						
  
  
  GPIO_PORTA_DEN_R =0xE0; // Enable Pin 5-7 on Port A
  GPIO_PORTA_PUR_R |= 0xE0;  // Make the push buttons pull up resistance

  GPIOIntRegister(GPIO_PORTA_BASE,IntSwitchMode); // Specify the interrupt handler for port A interrupts
  GPIOIntTypeSet( GPIO_PORTA_BASE,GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7,GPIO_FALLING_EDGE); // make the interrupt on the falling edge of pins 5,6,7
  GPIOIntEnable(GPIO_PORTA_BASE,GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7); // Enable interrupt
    IntPrioritySet(INT_GPIOA , 0xe0); // Set the priority lower than all of the other interrupts in the project

  IntMasterEnable(); //Enable interrupt
  
  delay(25);
  lcd_write_cmd(0x28); // Enable 4 bit mode in lcd
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
  
  lcd_string1("Embedded Project"); //startup message
  lcd_write_cmd(0xc0);
  lcd_string1("Team 7");
  delay(500);

  
}

void lcd_modes(void) // Main screen
{
  
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
void calculator_mode() // Calculator
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


  
  




void stopwatch_mode() // Stopwatch
{
  lcd_write_cmd(0x0c);
  lcd_write_cmd(0x01);
  lcd_write_cmd(0x80);
  lcd_string1("Stopwatch Mode"); 
  Timer0_init();
  delay(2000);
  
  lcd_write_cmd(0x01);
  lcd_write_cmd(0x80);
  lcd_string1("D to Start/Stop"); 

  lcd_write_cmd(0xc0);
  lcd_string("     ");
  lcd_string("00");
  lcd_string(":");
  lcd_string("00");
    
    
    while(1){
    stopwatch_read=KeyPad_Read(); 
    
         if(stopwatch_read=='.') // if the input is # Reset the stopwatch
    {
          TimerDisable(TIMER0_BASE, TIMER_A);
          Timer0_init();
  
      lcd_write_cmd(0x01);
      lcd_write_cmd(0x80);
    lcd_string("D to Start/Stop"); 

    lcd_write_cmd(0xc0);
    lcd_string("     ");
    lcd_string("00");
    lcd_string(":");
    lcd_string("00");
      continue;
    }
    
     if(stopwatch_read!='=') // if the input is not D. Exit the mode
    {
          TimerDisable(TIMER0_BASE, TIMER_A);
        lcd_write_cmd(0x01);
      lcd_string1("Stopwatch off"); 
      delay(1000);
      break;
    }
    if((stopwatch_read=='=') & (start_stop==0)) { // if the input is D Start the stopwatch 
      TimerEnable(TIMER0_BASE , TIMER_A);
      start_stop=1;
      continue;
    }
    if((stopwatch_read=='=') & (start_stop==1)) { // if the input is D Pause the stopwatch
      TimerDisable(TIMER0_BASE, TIMER_A);
      start_stop=0;
      continue;
    }
          }

}


void timer_mode() // Timer
{
  lcd_write_cmd(0x0c);
  lcd_write_cmd(0x01);
  lcd_string1("Timer Mode");
  delay(2000);
  
  lcd_write_cmd(0x01);
  lcd_string1("Enter Minutes:");
    for (int i =0;i<2;i++) // get values of minutes as in format 00
    {
      timer_val[i]=KeyPad_Read();
      lcd_write_nibble(timer_val[i]);
      delay(250);
    }
    int min = atoi(timer_val); // convert string value from input to integer for calculations
    
   lcd_write_cmd(0xc0);
   lcd_string1("Enter Seconds:");
   
   for (int i =0;i<2;i++) // get values of seconds as in format 00
    {
      timer_val[i]=KeyPad_Read();
      lcd_write_nibble(timer_val[i]); 
      delay(250);
    }
   int sec = atoi(timer_val); // convert string value from input to integer for calculations
    
  timer(min,sec); // send min and seconds entered values
  
}


void IntSwitchMode(void) // interrupt on the push buttons to enter different modes from the main screen
{
    GPIOIntClear(GPIO_PORTA_BASE,GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7); // clear the interrupt flag
    TimerIntUnregister(TIMER0_BASE, TIMER_A);
    TimerIntUnregister(TIMER1_BASE, TIMER_A);

     
if(GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_5)==0) // Check if button 1 is pressed
{
  while(GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_5)==0){} // wait for button release
  calculator_mode(); // Enter Calculator mode
}
  
else if(GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_6)==0) // Check if button 2 is pressed
{
  while(GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_6)==0){} // wait for button release
  timer_mode(); // Enter Timer mode

}
else if(GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_7)==0) // Check if button 3 is pressed
{
  while(GPIOPinRead(GPIO_PORTA_BASE, GPIO_PIN_7)==0){} // wait for button release
  stopwatch_mode(); // Enter Stopwatch mode

}
}
