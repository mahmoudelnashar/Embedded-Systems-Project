#include "delay.h"

void delay(unsigned int delay) // delay for 1 msec
{
	volatile unsigned int i, counter;
	counter = delay * 3180; 
	for(i=0;i<counter;i++);
}
