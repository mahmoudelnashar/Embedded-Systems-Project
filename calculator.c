#include "calculator.h"

char numb1_arr[20];
char numb2_arr[20];
char char_read1;
char char_read2;
double numb1;
double numb2;
double result;

bool numb1_not_done = true;



void calculator(){
int i =0;
  char op;
  

  int display;
  while(1)
  {
    i=0;
    lcd_write_cmd(0x0e);
    display = 0;
    bool first_time = true;
    numb1=result;
  while(1)
  {
    display++;
    if (display>15)
      lcd_write_cmd(0x18);
    
      
    char_read1 = KeyPad_Read();
    if (char_read1 =='+' | char_read1 =='-' | char_read1 =='/' | char_read1 =='x')
    {
      if (first_time & char_read1 =='-')
      {
        first_time = false;
        lcd_write_nibble(char_read1);
        numb1_arr[i] = char_read1;
        numb1 = atof(numb1_arr);
        i++;

      }
      else if (first_time & result !=193213.1206)
      {
        lcd_write_cmd(0x01);
        lcd_string("ANS");
        first_time = false;
      }
      else
      {
      lcd_write_nibble(char_read1);
      op = char_read1;
      break;
      }
  }
  else if (char_read1=='=')
  {
    break;
  }
    
  else
  {
    if (first_time)
    {
      lcd_write_cmd(0x01);
      first_time=false;
    }
    lcd_write_nibble(char_read1);
    numb1_arr[i] = char_read1;
    numb1 = atof(numb1_arr);
    i++;
  }
 
  
  }

  i=0;
  
   while(1)
   {
     
    display++;
    if (display>15)
      lcd_write_cmd(0x18);
    
    if (char_read1 =='=')
    {
      break;
    }
    
     char_read2 = KeyPad_Read();
    
     
    if (char_read2 =='=')
      break;
    
 
  else if(char_read2 =='+'  | char_read2 =='/' | char_read2 =='x')
    continue;
    
    else
  {
    lcd_write_nibble(char_read2);
    numb2_arr[i] = char_read2;
    numb2 = atof(numb2_arr);
    i++;
  }
  
   }
   if (char_read1 =='=')
    {
      break;
    }
   
   else
   {
    lcd_write_cmd (0xc0);
   lcd_write_nibble('=');
   result = calc(numb1,numb2,op);
   lcd_float(result);
  lcd_write_cmd(0x0c);
   }
  }
  }

  double calc(double var1, double var2 , char operator)
{
  if (operator == '+')
    return var1+var2;
  
  else if (operator == '-')
    return var1-var2;
  
   else if (operator == 'x')
    return var1*var2;
   
   else
   {
     if(var1 ==0 & var2==0)
     {
       lcd_string("Undefined");
     }
     else if (var2==0)
     {
       lcd_string("Infinity");
     }
     
    return var1/var2;
   }
}