#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define PIR 		25
#define SIREN		24

int main(void)
{
  if (wiringPiSetup() == -1) exit(1) ;

  unsigned char cnt=0;
  int value = 0;

  printf("***** Raspberry pi PIR Test ******\n") ;
  if (wiringPiSetup() == -1) exit(1) ;

  pinMode(PIR, INPUT);
  pinMode(SIREN, OUTPUT);

  digitalWrite(SIREN, 0);

  while(1)
  {
    value = digitalRead(PIR);

    if(value == HIGH)
    {
      printf(" %4d PIR sensed \r\n",cnt++);

      printf(" wait for next sensing ..\r\n");
      printf(" ------------------------\r\n\r\n\r\n");

      digitalWrite(SIREN, HIGH);
      delay(500);
      digitalWrite(SIREN, LOW);
      delay(4500);

      printf(" ready to sensing ... \r\n");
    }
  }
  return 0 ;
}
