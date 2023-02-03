#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define SWITCH 26 

int main(void)
{
  if( wiringPiSetup() == -1 )  return -1;

  pinMode(SWITCH, INPUT);

  while(1)
    {
      if (digitalRead(SWITCH) == 1)
            {
		   printf("HIGH \r\n");
           //delay(10); // 1�� delay
	     }
	  else
  	    {
		   printf("LOW \r\n");
		   //delay(1000);
      	    }
    }

   return 0;
}
