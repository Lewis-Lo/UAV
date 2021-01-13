#include <Arduino.h>
#include <stdlib.h>
#include <i2c_driver.h>
#include <i2c_driver_wire.h>

/*--------AOA, AOS--------*/
int read_AOA=A1,read_AOS=A2;
float AOA=0.0,AOS=0.0,avg_AOA=0,avg_AOS=0; 
int led_pin=8;
byte Angle[2];
#define angle1 60
#define angle2 50
#define range 255.75
/*--------AOA, AOS--------*/



void requestEvent();
void receiveEvent(int);
void Readangle();
void updateData();
void logData();
int sendIndex;

float d[7]; /*  d[0]:攻角
                d[1]:側滑角
                d[2]:舵面左
                d[3]:舵面右
                d[4]:舵面水平
                d[5]:舵面垂直
                d[6]:轉速
            */

typedef union bit_to_float
{
  float f;
  char c[4];
} btf;


void setup()
{
    Serial.begin(9600);  
    Wire.begin(0xac);        // join i2c bus with address 0x40
    Wire.onRequest(requestEvent); // register event
    Wire.onReceive(receiveEvent);
    for(int i=0; i<7; ++i){
        d[i] = i;
    }


    /*--------AOA, AOS--------*/
    pinMode(read_AOA,INPUT);
    pinMode(read_AOS,INPUT);
    pinMode(led_pin,OUTPUT);
    for(int i=0;i<3;i++){
        digitalWrite(led_pin,HIGH);
        delay(300);
        digitalWrite(led_pin,LOW);
        delay(200);
    }
    Serial.println("System start!");
    //digitalWrite(led_pin,HIGH);
    Serial.println("Test Initial value:");
    for(int i=0;i<1000;i++){
        avg_AOA+=analogRead(read_AOA);
        //Serial.println(analogRead(read_AOA));
        avg_AOS+=analogRead(read_AOS);
        delay(3);
    }
    avg_AOA/=1000;
    avg_AOS/=1000;
    Serial.println(avg_AOA);
    digitalWrite(led_pin,LOW);
    for(int i=0;i<2;i++){
        digitalWrite(led_pin,HIGH);
        delay(300);
        digitalWrite(led_pin,LOW);
        delay(200);
    }
    /*--------AOA, AOS--------*/
}

void loop()
{
    /*--------AOA, AOS--------*/
    Readangle();
    /*Serial.print(analogRead(read_AOA));
    Serial.print("data1:");
    Serial.print(AOA);
    Serial.print("\tdata2:");
    Serial.println(AOS);*/
    /*--------AOA, AOS--------*/
    updateData();

    delay(1000);
}

// function that executes whenever data is requested by master
// this function is registered as an event, see setup()
void requestEvent()
{
      btf temp;
      temp.f = d[sendIndex];
      for(int j=0; j<4; ++j){
        Wire.write(temp.c[j]);
      }
}

/*--------AOA, AOS--------*/
void Readangle(){
    AOA=(analogRead(read_AOA)- ((float)round(avg_AOA)-range)) * (angle1+angle2) / (((float)round(avg_AOA)+range) - ((float)round(avg_AOA)-range))-(angle1+angle2)/2;
    AOS=(analogRead(read_AOS)- ((float)round(avg_AOS)-range)) * (angle1+angle2) / (((float)round(avg_AOS)+range) - ((float)round(avg_AOS)-range))-(angle1+angle2)/2;
}
/*--------AOA, AOS--------*/


void updateData(){
    d[0] = AOA;
    d[1] = AOS;
    logData();
}
void logData(){
    for(int i=0; i<7; ++i){
        Serial.print(d[i]);
        Serial.print(" ");
    }
    Serial.println();
}
void receiveEvent(int index){
    if (Wire.available()) {
    switch(Wire.read()){
      case 0x01 : sendIndex = 0; break;
      case 0x02 : sendIndex = 1; break;
      case 0x03 : sendIndex = 2; break;
      case 0x04 : sendIndex = 3; break;
      case 0x05 : sendIndex = 4; break;
      case 0x06 : sendIndex = 5; break;
      case 0x07 : sendIndex = 6; break;
    }
    while (Wire.available()) Wire.read();
  }
}
