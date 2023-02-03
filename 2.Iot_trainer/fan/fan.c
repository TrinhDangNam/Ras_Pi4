#include <wiringPi.h>
#include <stdio.h>

int main()
{
    wiringPiSetup();  
    pinMode(21, OUTPUT);
    
    while(1)
{
	    digitalWrite(21, 1); 
	    //delay(200); 
}
return 0;
}