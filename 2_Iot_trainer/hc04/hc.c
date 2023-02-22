#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <wiringPi.h>

#define Trig 	27
#define Echo	28

int main(void)
{
  float distance=0;
  long starttime=0;
  long traveltime=0;

  printf("***** Raspberry pi UltraSonic Test ******\n");


  if (wiringPiSetup() == -1) exit(1);

  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);

  while (1)
  {
    digitalWrite(Trig, LOW);
    delay(50);

    digitalWrite(Trig, HIGH);
    delay(20);

    digitalWrite(Trig, LOW);

    while(digitalRead(Echo) == LOW);

    starttime = micros();

    while(digitalRead(Echo) == HIGH);

    traveltime = micros() - starttime;

    distance = traveltime / 5.8;
    printf("Distance : %.2fmm\r\n", distance);
    delay(1000);
  }
  return 0;
}
