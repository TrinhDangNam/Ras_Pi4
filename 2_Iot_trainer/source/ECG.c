#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <wiringSerial.h>
#include <string.h>


#define MCP3208_CS      10
#define LED_INDICATOR   1

#define ECG             1
#define BEAT            0

#define SPI_CH          0
#define SPI_SPEED       1000000


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

  printf("***** Raspberry Pi ECG Sensor Test ******\n") ;

  while (1)
  {

    ECG_Value = readADC_MCP3208(ECG);
    ECG_ANA = (5.0/4096)*ECG_Value;

    delay(10);

    ECG_Value = readADC_MCP3208(BEAT);
    ECG_BEAT = (5.0/4096)*ECG_Value;

    printf("ECG ANA = %.3fV  ::  ", ECG_ANA);
    printf("ECG BEAT = %.3f\n\r", ECG_BEAT);

    sprintf(z,"%.3f,%.3f", ECG_ANA,ECG_BEAT);
    serialPuts(fd, z);

    serialPuts(fd, "\n");
    fflush(stdout);

    delay(10);
  }

  serialClose(fd);
  return 0 ;
}




