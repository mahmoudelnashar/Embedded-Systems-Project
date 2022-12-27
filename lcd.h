#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include "tm4c123gh6pm.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"
#include "inc/hw_memmap.h"
#include "driverlib/interrupt.h"
#include "inc/hw_gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/timer.h"
#include "delay.h"



void lcd_string(char *str);
void lcd_string1(char *str);
void lcd_num(int num);
void lcd_float(float num);

void lcd_modes(void);
void lcd_write_nibble(unsigned char data);
void lcd_write_cmd(unsigned char command);
void printdata(unsigned char data);

