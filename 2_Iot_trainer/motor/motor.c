#include<stdio.h>
#include<wiringPi.h>

int main(void)

{

    wiringPiSetup();  //đánh số theo Broadcom /GPIO( nếu là lệnh wiringPiSetupGpio ) – tương ứng với chân.. lệnh gpio readall nhé

    pinMode(1, OUTPUT);  //Chọn và thiết lập Output chân LED ( chân số 1 trên phần cứng)

    //pinMode(2, INPUT);  //Thiết lập input cho chân button

    //pullUpDnControl (2, PUD_UP);  //Đọc tín hiệu từ button

    //digitalWrite(17, 0);

    while(1)
{
	    digitalWrite(1, 1); 
	    delay(200); 
	     
	    //digitalWrite(1, 0);
	    //delay(200);
	
}
    return 0;

}
