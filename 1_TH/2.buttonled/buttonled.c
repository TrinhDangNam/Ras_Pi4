#include<stdio.h>
#include<wiringPi.h>

int main(void)

{

    wiringPiSetup();  //đánh số theo Broadcom GPIO – tương ứng với chân

    pinMode(17, OUTPUT);  //Chọn và thiết lập Output chân LED

    pinMode(2, INPUT);  //Thiết lập input cho chân button

    pullUpDnControl (2, PUD_UP);  //Đọc tín hiệu từ button

    digitalWrite(17, 0);

    while(1)
    {

//        if(!digitalRead(2))
//        {
//
//	    digitalWrite(17, 1);
//	    printf("led sang");
//	    delay(200);
//            //digitalWrite(17, !digitalRead(17));
//	    
//            
//		}
//            
//         else 
//	    printf("led tat");
//	    delay(200);
//	    digitalWrite(17, 0);
//	    
		digitalWrite(17, 1);
		delay(1000);
		digitalWrite(17, 0);
		delay(1000);
    }

    return 0;

}
