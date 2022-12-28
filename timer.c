#include "timer.h"

// Buzzer on port E

int timer_sec =0; // variable for seconds
int timer_min =0; // variable for minutes

unsigned char* timer_display [60] = { // Array used to display the status of stopwatch
"00","01","02","03","04","05","06","07","08","09","10","11","12","13","14","15","16",
"17","18","19","20","21","22","23","24","25","26","27","28","29","30","31","32","33","34","35",
"36","37","38","39","40","41","42","43","44","45","46","47","48","49","50","51","52","53","54",
"55","56","57","58","59"};


unsigned char timer_read; // input buffer
int start=0; // will be used to check pause/resume

void IntTimer() // Interrupt handler for timer mode
{
  TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT); // clear interrupt flag

  timer_sec--; // decrement timer

  if (timer_sec==-1)  // if seconds reaches -1 update timer to 59 and decrement minutes as the range is 0-59
  {
    timer_sec=59;
    timer_min--;
  }
  if(timer_min ==-1) // if minutes reaches -1 that means the timer is finished
  {
  lcd_write_cmd(0x01);
  lcd_string("Buzzer");
  lcd_write_cmd(0xc0);
  lcd_string("Press any key");
  GPIO_PORTE_DATA_R |= (1 << 4); // write 1 on buzzer to trigger it
    delay(1250);

  TimerIntDisable(TIMER1_BASE, TIMER_TIMA_TIMEOUT); // disable the interupt as the timer finished
  }
  else{ // normal timer behaviour update screen
    lcd_write_cmd(0xc0);
    lcd_string("     ");
    lcd_string(timer_display[timer_min]); // print current minutes value
    lcd_string(":");
    lcd_string(timer_display[timer_sec]); // print current seconds value
  }
  
}

void Timer1_init(){
  SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1); // Enable peripheral using tivaware
  TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC); // make the timer periodic to repeat every 1 second and update
  TimerLoadSet(TIMER1_BASE , TIMER_A, (1*16e6) -1); // Number of ticks for 1 second
  TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT); // Enable timer
  TimerIntRegister(TIMER1_BASE, TIMER_A, IntTimer); // Specify interrupt handler for timer

  lcd_write_cmd(0x01);
  
}

void timer(int min,int sec) // function that waits for input
{
  timer_min=min;
  timer_sec=sec;
  Timer1_init();
  lcd_write_cmd(0x01);
  lcd_write_cmd(0x80);
  lcd_string1("D to Start/Stop"); 
// display the initial timer configuration
  lcd_write_cmd(0xc0);
  lcd_string("     ");
  lcd_string(timer_display[min]);
  lcd_string(":");
  lcd_string(timer_display[sec]);
    
    
    while(1){ 
    timer_read=KeyPad_Read();  // wait for input from user
    if(timer_read!='=') // if input is not D exit timer
    {
      TimerDisable(TIMER1_BASE, TIMER_A); // disable the timer
      lcd_write_cmd(0x01);
      lcd_string1("Timer off"); 
      delay(1000);
      break;
    }
    if((timer_read=='=') & (start==0)) {  // if input is D and the timer is paused resume it
      TimerEnable(TIMER1_BASE , TIMER_A);
      start=1;
      continue;
    }
    if((timer_read=='=') & (start==1)) { // if input is D and the timer is working pause it
      TimerDisable(TIMER1_BASE, TIMER_A);
      start=0;
      continue;
    }
          }

}