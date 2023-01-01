#include "calculator.h"

char numb1_arr[20]; // number1 buffer
char numb2_arr[20]; // number2 buffer
char char_read1; // input buffer
char char_read2; // input buffer
double numb1; // number1
double numb2; // number2
double result; // answer




void calculator(){ 
int i =0;
  char op; // operator
  

  int display;
  while(1)
  {
    i=0;
    lcd_write_cmd(0x0e);
    display = 0;
    bool first_time = true;
    numb1=result;  // storing previous result to be used in future calculations
  while(1)
  {
    display++;
    if (display>15) //shifts display if it is not enough to fit input
      lcd_write_cmd(0x18);
    
      
    char_read1 = KeyPad_Read();
    if (char_read1 =='+' | char_read1 =='-' | char_read1 =='/' | char_read1 =='x')
    {
      if (first_time & char_read1 =='-') // handling negative numbers
      {
        first_time = false;
        lcd_write_nibble(char_read1);
        numb1_arr[i] = char_read1;
        numb1 = atof(numb1_arr);
        i++;

      }
      else if (first_time & result !=1932131313.1206) // handling saved result
      {
        lcd_write_cmd(0x01);
        lcd_string("ANS");
        first_time = false;
              lcd_write_nibble(char_read1);
      op = char_read1;
      break;
      }
      else // storing the operator
      {
      lcd_write_nibble(char_read1);
      op = char_read1;
      break;
      }
  }
  else if (char_read1=='=') // exiting the mode
  {
    break;
  }
    
  else // normal number 1 input
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
    if (display>15) //shifts display if it is not enough to fit input
      lcd_write_cmd(0x18);
    
    if (char_read1 =='=') // exiting the mode
    {
      break;
    }
    
     char_read2 = KeyPad_Read();
    
     
    if (char_read2 =='=') // breaking to get the result
      break;
    
 
  else if(char_read2 =='+'  | char_read2 =='/' | char_read2 =='x') // handling error in entering character, simply ignore the input
    continue; 
    
    else // normal number 2 input
  {
    lcd_write_nibble(char_read2);
    numb2_arr[i] = char_read2;
    numb2 = atof(numb2_arr);
    i++;
  }
  
   }
   if (char_read1 =='=') // exiting the mode
    {
      break;
    }
   
   else //print the result
   {
    lcd_write_cmd (0xc0);
   lcd_write_nibble('=');
   result = calc(numb1,numb2,op);
   lcd_float(result);
  lcd_write_cmd(0x0c);
   }
  }
  }

  double calc(double var1, double var2 , char operator) // function used to do calculations
{
  if (operator == '+') // addition
    return var1+var2;
  
  else if (operator == '-') // subtraction
    return var1-var2;
  
   else if (operator == 'x') // multiplication
    return var1*var2;
   
   else // division and different error cases
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