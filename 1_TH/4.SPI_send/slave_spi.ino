#include <SPI.h>
 
void setup (void){
    Serial.begin (9600);
    // chon che do slave
    SPCR |= bit (SPE);
    // chuyen MISO sang master in, *slave out*
    pinMode(MISO, OUTPUT);
    // bat che do interrupts
    SPI.attachInterrupt();
    Serial.println("Receive via SPI: ");
 
}
 
// SPI interrupt routine
 
ISR (SPI_STC_vect){
    byte c = SPDR;
    Serial.write(c);
}
 
void loop (void){
    delay(10);
}
/*
- Chọn chế độ slave cho arduino

SPCR |= bit (SPE);

pinMode(MISO, OUTPUT);

- Gắn hàm interrupt cho SPI. Khi Master bắt đầu gửi dữ liệu, Arduino phát hiện interrupt và gọi hàm xử lý.

- Mỗi byte từ Master gửi tới sẽ được gửi lên màn hình qua serial.

Arduino uno sử dụng thạch anh 16Mhz thì SPI có thể đạt tới 16,000,000Hz
*/
