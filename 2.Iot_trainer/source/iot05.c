#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <wiringSerial.h>
#include <string.h>

#define MCP3208_CS			10
#define LED_INDICATOR		1

#define ECG					1
#define BEAT				0

#define DCKI 				25
#define DI 					24

#define SPI_CH			0
#define SPI_SPEED		1000000

unsigned char state[10] = {0,};
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
    if (data & 0x8000)		lstate=HIGH;
    else					lstate=LOW;

    digitalWrite(DI,lstate);

    if(digitalRead(DCKI))	lstate = LOW;
    else					lstate = HIGH;

    digitalWrite(DCKI,lstate);

    data <<=1;
  }
}


void setData(unsigned char lstate[])
{
  unsigned char i =0;

  for (i=0;i<10;i++)
  {
    sendData(lstate[i]);	
  }
  sendData(0x00);
  sendData(0x00);
  latchData();
}



int readADC_MCP3208(unsigned char adc_ch)
{
  int AdcValue=0;
  unsigned char adcbuff[3];

  adcbuff[0] = 0x06 | ((adc_ch&0x07)>>2);
  adcbuff[1] = (adc_ch&0x07)<<6;
  adcbuff[2] = 0;

  digitalWrite(MCP3208_CS, 0);
  wiringPiSPIDataRW(SPI_CH, adcbuff, 3);

  AdcValue = (adcbuff[1]&0x0f)<<8 | adcbuff[2];

  digitalWrite(MCP3208_CS, 1);

  return AdcValue;
}


int main(void)
{
  float ECG_ANA=0;
  float ECG_BEAT=0;
  int ECG_Value=0;
  int cnt=0;
  int fd;
  int data;
  int set;
  unsigned char z[256];

  if ((fd = serialOpen ("/dev/ttyS0", 115200)) < 0)
  {
    fprintf (stderr, "Unable to open serial device: %s\n", strerror (errno));
    return 1;
  }	

  if(wiringPiSetup() == -1)
  {	
    fprintf(stdout, "Unable to start wiringPi: %s\n", strerror(errno));
    return 1 ;
  }

  if(wiringPiSPISetup(SPI_CH, SPI_SPEED) == -1)
  {
    fprintf(stdout, "wiringPiSPISetup Failed: %s\n\r", strerror(errno));
    return 1;
  }

  pinMode(MCP3208_CS, OUTPUT);
  pinMode(DCKI,OUTPUT);
  pinMode(DI,OUTPUT);	

  printf("***** Raspberry Pi ECG and LED Test ******\n") ;

  printf("---------------------------------\r\n");

  delay(2000);

  while (1)
  {
    ECG_Value = readADC_MCP3208(ECG);
    ECG_ANA = (5.0/4096)*ECG_Value;

    delay(10);

    ECG_Value = readADC_MCP3208(BEAT);
    ECG_BEAT = (5.0/4096)*ECG_Value;

    printf("ECG ANA = %.3fV  ::  ", ECG_ANA);
    printf("ECG BEAT = %.3f\n\r", ECG_BEAT);

    if(ECG_BEAT > 2 )
    {
      for(set = 0 ; set < 6 ; set++)
      {				
        state[set]=0xFF;
      }	
      setData(state);
    }
    else if(ECG_BEAT < 2)
    {
      unsigned char state[10] = {0,};
      for(set = 0 ; set < 3 ; set++)
      {				
        state[set]=0xFF;
      }
      setData(state);	
    }
    sprintf(z,"%.3f,%.3f", ECG_ANA,ECG_BEAT);
    serialPuts(fd, z);

    serialPuts(fd, "\n");
    fflush(stdout);

    delay(10);
  }
  serialClose(fd);
  return 0 ;
}