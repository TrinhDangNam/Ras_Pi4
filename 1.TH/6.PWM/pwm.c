#include <stdio.h>
#include <wiringPi.h>

int main(void)
{
	wiringPiSetup();
// pwmFrequency in Hz = 19.2e6 Hz / pwmClock / pwmRange.
//put PWM in mark-space mode, which will give you 
//the traditional (and easily predictable) PWM    
	pwmSetMode(PWM_MODE_MS);
//setting ping GPIO 18 as a pwm output
	pinMode(18,PWM_OUTPUT);
//Set clock divisor to 4000
	pwmSetClock(384); //clock at 50kHz (20us tick)
	pwmSetRange(1000); //range at 1000 ticks (20ms)
	while(1)
	{
	pwmWrite(1, 75);  // độ rộng xung 
	//theretically 50 (1ms) to 100 (2ms) on my servo 30-130 works ok
	}
	return 0 ;
}
