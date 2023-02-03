#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>

#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <softPwm.h>

#include <mysql/mysql.h>

#define DBHOST "localhost"
#define DBUSER "pi"
#define DBPASS "raspberry"
#define DBNAME "home"

#define CS_MCP 0

#define MAX_COUNT      255
#define MAXTIMINGS     85

#define DHTPIN 22



#define SPI_CHANNEL 0
#define SPI_SPEED 1000000

MYSQL *connector;
MYSQL_RES *result;
MYSQL_ROW row;


int dht11_dat[5] = {0,};

void read_dht11_dat(float temp,  float humi )
{
  uint8_t laststate = HIGH;
  uint8_t counter = 0;
  uint8_t j = 0, i;
  float f;

  dht11_dat[0]=dht11_dat[1]=dht11_dat[2]=dht11_dat[3]=dht11_dat[4] = 0;

  pinMode(DHTPIN, OUTPUT);
  digitalWrite(DHTPIN, LOW);
  delay(18);
  digitalWrite(DHTPIN, HIGH);
  delayMicroseconds(30);

  pinMode(DHTPIN, INPUT);

  for (i = 0; i < MAXTIMINGS; i++)
  {
    counter = 0;
    while ( digitalRead(DHTPIN) == laststate )	//255us delay
    { 
      counter++;
      delayMicroseconds(1);
      if (counter == MAX_COUNT) break;
    }

    printf("laststate %s -> ", (laststate==HIGH?"HIGH":"LOW"));
    laststate = digitalRead(DHTPIN);
    printf("%s \r\n", (laststate==HIGH?"HIGH":"LOW"));

    if (counter == MAX_COUNT) break;

    if ( (i>=4) && ((i%2) == 0) )
    {
      dht11_dat[j/8] <<= 1;
      if (counter > 18) dht11_dat[j/8] |= 1;
      j++ ;
    }
  }

  if((j>=40) && (dht11_dat[4]==((dht11_dat[0]+dht11_dat[1]+dht11_dat[2]+dht11_dat[3])&0xFF)))
  {
  	char query[1024];
  	
    f=dht11_dat[2]*9./5.+32;
    //printf("humidity = %d.%d %% Temperature = %d.%d *C (%.1f F)\n",
    //dht11_dat[0], dht11_dat[1], dht11_dat[2], dht11_dat[3], f) ;
    
    humi =  dht11_dat[0] + dht11_dat[1] * 0.1;
    printf("humidity = %.1f %%\n",humi);
    temp =  dht11_dat[2] +  dht11_dat[2]*0.1;
    printf("Temperature = %.1f *C\n",temp);
    f=temp*9./5.+32;
     printf("Temperature F = %.1f *F\n",f);
       //timer
     struct tm *gmt;
    time_t raw;

    time(&raw);
    gmt = gmtime(&raw);
    
    int ngay, thang, nam, gio, phut, giay =0;
    
    ngay = gmt->tm_mday;
    thang = 1+gmt->tm_mon;
    nam = 1900+gmt->tm_year;
    gio = 7+gmt->tm_hour;
    phut = gmt->tm_min;
    giay = 1+gmt->tm_sec;
    
 
     
		sprintf(query,"insert into temp_at_interrupt(Date,Time,Temperature,Humidity) values ('%d/%d/%d','%d:%d:%d','%f','%f')",nam,thang,ngay,gio,phut
		,giay,temp,humi);
		if(mysql_query(connector,query))
		{
			fprintf(stderr,"%s\n",mysql_error(connector));
			printf("Write DB error\n");
		}
    	sleep(10);
	
  }
  else printf("Data get failed\r");
}



int main(void)
{
  printf("dht11 Raspberry pi\n") ;
  if (wiringPiSetup() == -1) exit(1) ;
  
  float temp=0;
  float humi = 0;
  
  	connector=mysql_init(NULL);
	if(!mysql_real_connect(connector,DBHOST,DBUSER,DBPASS,DBNAME,3306,NULL,0))
	{
		fprintf(stderr,"%s\n",mysql_error(connector));
		return 0;
	}

  
  while (1)
  {
  	
  		
    read_dht11_dat(temp,humi);
     delay(1000);
   
  }
  mysql_close(connector);
  return 0;
}
