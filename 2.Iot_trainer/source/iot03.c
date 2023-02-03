#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define MAX_COUNT      255
#define MAXTIMINGS     85

#define DHTPIN         25
#define SIREN		        24

#define THRESHOLD      30

int dht11_dat[5] = {0,};

void read_dht11_dat()
{
  uint8_t laststate = HIGH;
  uint8_t counter = 0;
  uint8_t j = 0, i;
  float f;

  dht11_dat[0] = dht11_dat[1] = dht11_dat[2] = dht11_dat[3] = dht11_dat[4] = 0;

  pinMode(SIREN, OUTPUT);

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

    if (counter == MAX_COUNT) break ; // if while breaked by timer, break for

    if ( (i>=4) && ((i%2) == 0) )	// even 
    {
      dht11_dat[j/8] <<= 1;					//bit '0'
      if (counter > 18) dht11_dat[j/8] |= 1;	//bit 	
      j++ ;
    }
  }

  if ( (j>=40) && (dht11_dat[4]==((dht11_dat[0]+dht11_dat[1]+dht11_dat[2]+dht11_dat[3])&0xFF)))
  {
    f=dht11_dat[2]*9./5.+32;
    printf("humidity = %d.%d %% Temperature = %d.%d *C (%.1f F)\n",
    dht11_dat[0], dht11_dat[1], dht11_dat[2], dht11_dat[3], f) ;

    if (dht11_dat[2] >= THRESHOLD)
    {
      digitalWrite(SIREN, HIGH);
      delay(1000);
    }
    digitalWrite(SIREN, LOW);
  }
  else printf("Data get failed\r");
}



int main(void)
{
  printf("dht11 Raspberry pi\n") ;
  if (wiringPiSetup() == -1) exit(1) ;

  while (1)
  {
    read_dht11_dat();
    delay(1000);
  }
  return 0;
}
