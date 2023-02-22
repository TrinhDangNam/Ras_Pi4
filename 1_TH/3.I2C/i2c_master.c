#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>
 
#define addrSlave 0x08
 
int val = 0x10;   // DEC = 16
 
 /*Thư viện <wiringPiI2C.h>

-      Chọn địa chỉ slave, wiringPiI2CSetup(addrSlave): địa chỉ là 0x08.

-      Gửi dữ liệu tới Arduino, wiringPiI2CWrite(fd,val) : lệnh gửi 1 byte tới slave và có dữ liệu là val. (val ở dạng hex đổi ra thập phân bằng = 16)

Câu lệnh đọc dữ liệu được thư viện WiringPi hỗ trợ là :

int wiringPiI2CRead (int fd) : dữ liệu trả về dạng int.
* */
int main(){
 
	int fd = wiringPiI2CSetup(addrSlave);
 
	while(1){
		wiringPiI2CWrite(fd,val);
		printf("%d",val);
		fflush(stdout);
		delay(1000);
	}
	return 0;
}
