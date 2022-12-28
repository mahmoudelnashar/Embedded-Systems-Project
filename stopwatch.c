#include "stopwatch.h"

// variables that hold the value of the seconds and the minutes
int counter_sec =0; // variable for secodns
int counter_min =0; // variable for minutes


// Array used to display the status of stopwatch
unsigned char* stopwatch_display [60] =  {
"00","01","02","03","04","05","06","07","08","09","10","11","12","13","14","15","16",
"17","18","19","20","21","22","23","24","25","26","27","28","29","30","31","32","33","34","35",
"36","37","38","39","40","41","42","43","44","45","46","47","48","49","50","51","52","53","54",
"55","56","57","58","59"};

void IntStopwatch(void) // Interrupt handler for Stopwatch mode
{
  TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT); // clear interrupt flag

  counter_sec++; // Increment seconds by 1

  if (counter_sec==60){ // when seconds reach 60 make 60 = 0  and increment a minute as the value ranges from 0 to 59
    counter_sec=0;
    counter_min++;
  }
  // write the correct time in the middle of the screen with the required format
    lcd_write_cmd(0xc0);
    lcd_string("     ");
    lcd_string(stopwatch_display[counter_min]);
    lcd_string(":");
    lcd_string(stopwatch_display[counter_sec]);
  
  
}




void Timer0_init(){
  SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0); // Enable peripheral using tivaware
  TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC); // make the timer periodic to repeat every 1 second and update
  TimerLoadSet(TIMER0_BASE , TIMER_A, (1*16e6) -1); // Number of ticks for 1 second
  TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT); // Enable timer
  TimerIntRegister(TIMER0_BASE, TIMER_A, IntStopwatch); // Specify interrupt handler for stopwatch
  counter_sec=0; // reset seconds from previous use
  counter_min=0; // reset minutes from previous use

}


