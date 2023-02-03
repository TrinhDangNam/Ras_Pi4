#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <softPwm.h>

#include <mysql/mysql.h>

#define DBHOST "localhost"
#define DBUSER "pi"
#define DBPASS "raspberry"
#define DBNAME "rpidb"

#define CS_MCP 0

#define MAX_COUNT      255
#define MAXTIMINGS     85

#define DHTPIN 22

int dht11_dat[5] = {0,};


#define SPI_CHANNEL 0
#define SPI_SPEED 1000000

MYSQL *connector;
MYSQL_RES *result;
MYSQL_ROW row;

int read_mcp3208_adc(unsigned char adcChannel)
{
	unsigned char buff[3];
	int adcValue = 0;

	buff[0] = 0x06|((adcChannel&0x07) >> 7);
	buff[1] = ((adcChannel & 0x07) << 6);
	buff[2] = 0x00;

	digitalWrite(CS_MCP,0);

	wiringPiSPIDataRW(SPI_CHANNEL,buff,3);

	buff[1] = 0x0f &buff[1];
	adcValue = (buff[1]<<8)|buff[2];

	digitalWrite(CS_MCP,1);
	//adcValue = (float)(adcValue*3.3)/1024;
	return adcValue;
}

/*float cho(){
	int start_time, end_time ;
	float distance;
	digitalWrite(trig, LOW) ;

	delay(500) ;

	digitalWrite(trig, HIGH) ;

	delayMicroseconds(10) ;

	digitalWrite(trig, LOW) ;

	while (digitalRead(echo) == 0) ;

	start_time = micros() ;

	while (digitalRead(echo) == 1) ;

	end_time = micros() ;

	distance = (end_time - start_time) / 29. / 2. ;

    	return distance;
}*/

int main(int argc, char const *argv[])
{
	int resi = 0;
	int resivalue = 0;
	float choum = 0;



	pinMode(CS_MCP,OUTPUT);
	wiringPiSetup();		/* initialize wiringPi setup */
	pinMode(MOTOR,OUTPUT);	/* set GPIO as output */
	softPwmCreate(MOTOR,1,3725);
	
	if(wiringPiSetup() == -1)
	{
		fprintf(stdout, "Unable : %s\n", strerror(errno));
		return 1;
	}

	if(wiringPiSPISetup(SPI_CHANNEL, SPI_SPEED)==-1)
	{
		fprintf(stdout, "%s\n", strerror(errno));
		return 1;
	}

	connector=mysql_init(NULL);
	if(!mysql_real_connect(connector,DBHOST,DBUSER,DBPASS,DBNAME,3306,NULL,0))
	{
		fprintf(stderr,"%s\n",mysql_error(connector));
		return 0;
	}


	while(1)
	{
		char query[1024];
		resivalue = read_mcp3208_adc(resi);

		printf("resivalue = %d\n",resivalue);
		
		  choum = (5.0/4096) * resivalue;
		  		printf("Volt = %f\n",choum);
		softPwmWrite (MOTOR, resivalue);
		delay (1000);
	//	choum=cho();
	//	printf("distance %.2f cm\n", choum);
		sprintf(query,"insert into envdata(VR,Wave) values (%f,%d)",
		choum,resivalue);
		if(mysql_query(connector,query))
		{
			fprintf(stderr,"%s\n",mysql_error(connector));
			printf("Write DB error\n");
		}
		sleep(1);
	}
	mysql_close(connector);
	return 0;
}
