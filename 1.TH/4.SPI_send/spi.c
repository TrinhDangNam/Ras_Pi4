#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
 
#define chanel 0
#define speed 500000
#define	SS0	8       // GPIO 8
 
int main(){
	unsigned char buff[4] = "abc\n";
	pinMode(SS0, OUTPUT);
	digitalWrite(SS0, 1);
	int fd;
	if( (wiringPiSPISetup(chanel, speed)) <0){
		fprintf (stderr, "SPI Setup failed: %s\n", strerror (errno));
		return 0;
	}
 
	fd = wiringPiSPIGetFd (0) ;
 
	while(1){
		printf("%s",buff);
		fflush(stdout);
		digitalWrite(SS0, 0);
		write (fd, buff, 4) ;
		digitalWrite(SS0, 1);
		delay(1000);
	}
	return 0;
}
/*
 * - Thiết lập SPI-0 :

wiringPiSPISetup(chanel, speed): chanel là 0 chọn port 0, chanel 1 chọn port 1. Speed là tốc độ của SPI (500,000 - 32,000,000). Nó trả về file-descriptor với giá trị -1 nghĩa là bị lỗi.

Vi xung nhịp chỉ được tạo bởi master nên tốc độ cũng do master quyết định. Chú ý rằng tốc độ này không được vượt quá tốc độ của arduino (Các bạn xem bên dưới )

- Gắn file-descriptor:

fd = wiringPiSPIGetFd (chanel) : file-descriptor được dùng để đại diện cho input/output của linux.Ta sẽ dùng file này để dùng các hàm gửi/nhận thông qua nó.

- Gửi và nhận :

write(int fd, const void *buf, size_t count);

read(int fd, void *buf, size_t count);

fd : là file-descriptor, buf là mảng kí tự, count là số byte gửi/ nhận.

-  Trong quá trình gửi và nhận phải đưa chân SS xuống thấp, sau đó được đưa lên mức cao để kết thúc.

Các hàm được hỗ trợ : wirinpi - spi
* /
