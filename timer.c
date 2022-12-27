#include "timer.h"



int32_t timer_sec =0;
int32_t timer_min =0;

unsigned char* timer_display [60] = {
"00","01","02","03","04","05","06","07","08","09","10","11","12","13","14","15","16",
"17","18","19","20","21","22","23","24","25","26","27","28","29","30","31","32","33","34","35",
"36","37","38","39","40","41","42","43","44","45","46","47","48","49","50","51","52","53","54",
"55","56","57","58","59"};


unsigned char timer_read;
int start=0;

void IntTimer()
{
  TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);

  timer_sec--;

  if (timer_sec==-1){
    timer_sec=59;
    timer_min--;
  }
  if(timer_min ==-1)
  {
  lcd_write_cmd(0x01);
  lcd_string("Buzzer");
  lcd_write_cmd(0xc0);
  lcd_string("Press any key");
  GPIO_PORTE_DATA_R |= (1 << 4);
    delay(1250);

  TimerIntDisable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
  }
  else{
    lcd_write_cmd(0xc0);
    lcd_string("     ");
    lcd_string(timer_display[timer_min]);
    lcd_string(":");
    lcd_string(timer_display[timer_sec]);
  }
  
}

void Timer1_init(){
  SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);
  TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);
  TimerLoadSet(TIMER1_BASE , TIMER_A, (1*16e6) -1);
  TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
  TimerIntRegister(TIMER1_BASE, TIMER_A, IntTimer);
      //IntPrioritySet(INT_TIMER1A , 0xe0);

  lcd_write_cmd(0x01);
  
}

void timer(int min,int sec)
{
  timer_min=min;
  timer_sec=sec;
  Timer1_init();
  lcd_write_cmd(0x01);
  lcd_write_cmd(0x80);
  lcd_string1("# to Start/Stop"); 

  lcd_write_cmd(0xc0);
  lcd_string("     ");
  lcd_string(timer_display[min]);
  lcd_string(":");
  lcd_string(timer_display[sec]);
    
    
    while(1){ 
    timer_read=KeyPad_Read();  
    if(timer_read!='.')
    {
      TimerDisable(TIMER1_BASE, TIMER_A);
      lcd_write_cmd(0x01);
      lcd_string1("Timer off"); 
      delay(1000);
      break;
    }
    if((timer_read=='.') & (start==0)) { 
      TimerEnable(TIMER1_BASE , TIMER_A);
      start=1;
      continue;
    }
    if((timer_read=='.') & (start==1)) {
      TimerDisable(TIMER1_BASE, TIMER_A);
      start=0;
      continue;
    }
          }

}