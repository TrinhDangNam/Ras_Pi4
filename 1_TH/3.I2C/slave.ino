#include <Wire.h> 
 
void setup() { 
  Wire.begin(0x08);                // dia chi #0x08 
  Wire.onReceive(receiveEvent); // ham goi event I2C 
  Serial.begin(9600);            
} 
 
void loop() { 
  delay(100); 
} 
 
void receiveEvent(int n) { 
  while(Wire.available())    // chay lien tuc den khi het data 
  {  
    byte c = Wire.read();    // Doc 1 byte du lieu 
    Serial.print(c);         // In len man hinh qua uart 
  } 
}
