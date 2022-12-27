#include "stopwatch.h"

int32_t counter_sec =0;
int32_t counter_min =0;

unsigned char* stopwatch_display [60] = {
"00","01","02","03","04","05","06","07","08","09","10","11","12","13","14","15","16",
"17","18","19","20","21","22","23","24","25","26","27","28","29","30","31","32","33","34","35",
"36","37","38","39","40","41","42","43","44","45","46","47","48","49","50","51","52","53","54",
"55","56","57","58","59"};

void IntStopwatch(void){
  //MAP_TimerIntClear(TIMER0_BASE, TIMER_RTC_MATCH);
  TimerIntClear(TIMER0_BASE, TIMER_TIMA_TIMEOUT);

  counter_sec++;

  if (counter_sec==60){
    counter_sec=0;
    counter_min++;
  }
    lcd_write_cmd(0xc0);
    lcd_string("     ");
    lcd_string(stopwatch_display[counter_min]);
    lcd_string(":");
    lcd_string(stopwatch_display[counter_sec]);
  
  
}

void Timer0_init(){
  SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER0);
  //TimerConfigure(TIMER0_BASE, TIMER_CFG_RTC);
  TimerConfigure(TIMER0_BASE, TIMER_CFG_PERIODIC);
  TimerLoadSet(TIMER0_BASE , TIMER_A, (1*16e6) -1);
  //TimerMatchSet(TIMER0_BASE,TIMER_A,1);
  //TimerEnable(TIMER0_BASE , TIMER_A);
  //TimerIntEnable(TIMER0_BASE, TIMER_RTC_MATCH);
  TimerIntEnable(TIMER0_BASE, TIMER_TIMA_TIMEOUT);
  TimerIntRegister(TIMER0_BASE, TIMER_A, IntStopwatch);
  counter_sec=0;
  counter_min=0;
        //IntPrioritySet(INT_TIMER0A , 0xe0);

  //TimerRTCEnable(TIMER0_BASE);
}


