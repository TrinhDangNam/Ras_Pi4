#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <wiringSerial.h>
#include <string.h>

#define MCP3208_CS		10
#define SPI_CH			0
#define SPI_SPEED		1000000

int readADC_MCP3208(unsigned char adc_ch)
{
  int AdcValue=0;
  unsigned char adcbuff[3];

  adcbuff[0] = 0x06 | ((adc_ch&0x07)>>2);
  adcbuff[1] = (adc_ch&0x07)<<6;
  adcbuff[2] = 0;

  digitalWrite(MCP3208_CS, 0);
  wiringPiSPIDataRW(adc_ch, adcbuff, 3);

  AdcValue = (adcbuff[1]&0x0f)<<8 | adcbuff[2];

  digitalWrite(MCP3208_CS, 1);

  return AdcValue;
}

int main(void)
{
  int ADC_CH=4;
  int ADC_Value=0;
  float Sound_Volt=0;
  int Sound_Value=0;
  int cnt=0;

  int fd;
  int data;
  unsigned char z[256];

  if ((fd = serialOpen ("/dev/ttyS0", 115200)) < 0)
  {
    fprintf (stderr, "Unable to open serial device: %s\n", strerror(errno));
    return 1;
  }

  if(wiringPiSetup() == -1)
  {	
    fprintf(stdout, "Unable o start wiringPi: %s\n", strerror(errno));
    return 1 ;
  }

  if(wiringPiSPISetup(SPI_CH, SPI_SPEED) == -1)
  {
    fprintf(stdout, "wiringPiSPISetup Failed: %s\n\r", strerror(errno));
    return 1;
  }

  pinMode(MCP3208_CS, OUTPUT);

  printf("***** Raspberry Pi Sound Sensor Test ******\n") ;

  while (1)
  {

    Sound_Value = readADC_MCP3208(ADC_CH);

    Sound_Volt = (5.0/4096)*Sound_Value;
    printf("Sound Volt = %.3fV\n\r", Sound_Volt);

    sprintf(z,"%.3f, ", Sound_Volt);
    serialPuts(fd, z);

    sprintf(z,"%.3f", (0-Sound_Volt));
    serialPuts(fd, z);

    serialPuts(fd, "\n");
    fflush(stdout);

    delay(10);
  }

  serialClose(fd);
  return 0 ;
}
