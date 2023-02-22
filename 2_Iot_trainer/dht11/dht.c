#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#define MAX_COUNT      255
#define MAXTIMINGS     85

#define DHTPIN 22

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
    f=dht11_dat[2]*9./5.+32;
    //printf("humidity = %d.%d %% Temperature = %d.%d *C (%.1f F)\n",
    //dht11_dat[0], dht11_dat[1], dht11_dat[2], dht11_dat[3], f) ;
    
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
    
     printf("Date = %d/%d/%d\n",ngay,thang,nam);
     printf("Time = %d:%d:%d\n",gio,phut,1+giay);
    humi =  dht11_dat[0] + dht11_dat[1] * 0.1;
    printf("humidity = %.1f %%\n",humi);
    temp =  dht11_dat[2] +  dht11_dat[2]*0.1;
    printf("Temperature = %.1f *C\n",temp);
    f=temp*9./5.+32;
     printf("Temperature F = %.1f *F\n",f);
  }
  else printf("Data get failed\r");
}



int main(void)
{
  printf("dht11 Raspberry pi\n") ;
  if (wiringPiSetup() == -1) exit(1) ;
  
  float temp=0;
  float humi = 0;
  
  while (1)
  {
    read_dht11_dat(temp,humi);
    delay(1000);
  }
  return 0;
}