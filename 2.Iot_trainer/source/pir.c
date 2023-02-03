#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define PIR 		25

int main(void)
{
  unsigned char cnt=0;
  int value = 0;

  printf("***** Raspberry pi PIR Test ******\n") ;

  if (wiringPiSetup() == -1) exit(1) ;

  pinMode(PIR, INPUT);

  while(1)
  {
    value = digitalRead(PIR);
    if(value == HIGH)
    {
      printf(" %4d PIR sensed \r\n",cnt++);

      printf(" wait for next sensing ..\r\n");
      printf(" ------------------------\r\n\r\n\r\n");
      delay(5000);
      printf(" ready to sensing ... \r\n");
    }
    else
    {
      printf("waiting..... \n");
      delay(100);
    }
  }
  return 0 ;
}
