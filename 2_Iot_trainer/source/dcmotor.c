#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <softPwm.h>
#include <wiringPiSPI.h>

#define CS_MCP3208  0

#define SPI_CHANNEL 0
#define SPI_SPEED   1000000  // 1MHz

#define MOTOR 		25


int read_mcp3208_adc(unsigned char adcChannel)
{
  unsigned char buff[3];
  int adcValue = 0;

  buff[0] = 0x06 | ((adcChannel & 0x07) >> 7);
  buff[1] = ((adcChannel & 0x07) << 6);
  buff[2] = 0x00;

  digitalWrite(CS_MCP3208, 0);  // Low : CS Active

  wiringPiSPIDataRW(SPI_CHANNEL, buff, 3);

  buff[1] = 0x0F & buff[1];
  adcValue = ( buff[1] << 8) | buff[2];

  digitalWrite(CS_MCP3208, 1);  // High : CS Inactive

  return adcValue;
}



int main(void)
{
  unsigned char cnt=0;

  printf("***** Raspberry pi Motor Test ******\n") ;
  if (wiringPiSetup() == -1) exit(1) ;
  
  
  int adcChannel = 0;
  int adcValue   = 0;
  float adc_Volt = 0;


  if(wiringPiSPISetup(SPI_CHANNEL, SPI_SPEED) == -1)
  {
    fprintf (stdout, "wiringPiSPISetup Failed: %s\n", strerror(errno));
    return 1 ;
  }

  pinMode(CS_MCP3208, OUTPUT);

  while(1)
  {
    adcValue = read_mcp3208_adc(adcChannel);

    adc_Volt = (5.0/4096) * adcValue;
    printf("adc0 Value = %f\n", adc_Volt);
  }


 // pinMode(MOTOR, OUTPUT);

 // digitalWrite(MOTOR, 1);
  //delay(5000);
 // digitalWrite(MOTOR, 0);
 // delay(5000);
 // digitalWrite(MOTOR, HIGH);
 // delay(5000);
 // digitalWrite(MOTOR, LOW);

	int intensity;
	wiringPiSetup();		/* initialize wiringPi setup */
	pinMode(MOTOR,OUTPUT);	/* set GPIO as output */
	softPwmCreate(MOTOR,1,100);	/* set PWM channel along with range*/
	while (1)
	  {
		
		
		  softPwmWrite (MOTOR, adcValue); /* change the value of PWM */
		  delay (10) ;
		
	

	/*	for (intensity = 100; intensity >= 0; intensity--)
		{
		  softPwmWrite (MOTOR	, intensity);
		  delay (10);
		}
		delay(1);*/
		
	}
 // return 0;

}
