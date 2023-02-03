#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <wiringPi.h>

#define DCKI 25
#define DI 24

#define GLB_CMDMODE 0x00

unsigned char state[10];
unsigned char greenToRed =0;

void latchData()
{
  unsigned char i=0;
  digitalWrite(DI,LOW);
  delayMicroseconds(250);

  for(i=0;i<4;i++)
  {
    digitalWrite(DI,HIGH);
    delayMicroseconds(2);
    digitalWrite(DI,LOW);
    delayMicroseconds(2);
  }
  delayMicroseconds(2);
}

void sendData(unsigned int data)
{
  unsigned int lstate=0;
  unsigned char i =0;
  
  for (i=0;i<16;i++)
  {
    if (data & 0x8000) lstate=HIGH;
    else lstate=LOW;

    digitalWrite(DI,lstate);

    if(digitalRead(DCKI)) lstate = LOW;
    else lstate = HIGH;

    digitalWrite(DCKI,lstate);

    data <<=1;
  }
}

void setData(unsigned char lstate[])
{
  unsigned char i =0;
  sendData(GLB_CMDMODE);
  
  for (i=0;i<10;i++)
  {
    sendData(lstate[i]);
  }
  sendData(0x00);
  sendData(0x00);
  latchData();
}

void setBits(unsigned int bits)
{
  unsigned char i=0;
  for(i=0;i<10;i++)
  {
    if ( i%2 )
    {
      if ( bits%2 == 1 ) state[i]=0xFF;
      else  state[i]=0x00;
    }
    else
    {
      if ( bits%2 == 1 ) state[i]=0x00;
      else  state[i]=0xFF;
    }

  }
  setData(state);
}

int main(void)
{
  unsigned char i=0;
  int j=0;

  printf("**** RASPBERRY PI LED BAT BASIC TEST ******\n");

  if(wiringPiSetup() == -1) exit(1);

  pinMode(DCKI,OUTPUT);
  pinMode(DI,OUTPUT);

  for(i=0;i<10;i++)
  {
    state[i] = 0x00;
  }	

  delay(500);

  while(1)
  {
    setBits(0x155);
    delay(1000);

    setBits(0x2AA);
    delay(1000);

    setBits(0x0);
    delay(1000);
  }
  return 0;
}	
