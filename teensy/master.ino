#include <Arduino.h>
#include <i2c_driver.h>
#include <i2c_driver_wire.h>
#include <stdlib.h>
int readCounter;
int reqCounter;

typedef union bit_to_float
{
  float f;
  char c[4];
  int i;
} btf;

void setup()
{
  Wire.begin();         // join i2c bus
  Serial.begin(9600);   // start serial for output
  readCounter = 0;
  reqCounter = 0;
}

void loop()
{
  Wire.beginTransmission(0xac);   // set adress
  if(reqCounter == 7) reqCounter = 0;
  switch(reqCounter){
    case 0 : Wire.write(0x01); break;
    case 1 : Wire.write(0x02); break;
    case 2 : Wire.write(0x03); break;
    case 3 : Wire.write(0x04); break;
    case 4 : Wire.write(0x05); break;
    case 5 : Wire.write(0x06); break;
    case 6 : Wire.write(0x07); break;
    default : reqCounter = 0; break;
  }
  ++reqCounter;
  delay(1);
  Wire.endTransmission();
  /*---------------------------------*/
  
  Serial.print("read: ");

  Wire.requestFrom(0xac, 4);   // request 4 bytes from slave device 0xac

  btf temp;
  temp.f = 0;
  while(Wire.available()) { // slave may send less than requested
    
    char c = Wire.read();
    temp.c[readCounter] = c;
    ++readCounter;
    if(readCounter == 4){
      Serial.print(temp.f);
      Serial.print(" ");
      readCounter = 0;
    }
    
  }

  Serial.println();
  delay(499);
}
