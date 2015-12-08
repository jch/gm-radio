#include "MBus.h"
#define mBusIn 10 //input port
#define mBusOut 12 //output port

MBus mBus(mBusIn, mBusOut); //make an object of MBus to work with
uint64_t receivedMessage=0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  if(mBus.receive(&receivedMessage)) {
    char buffer[50];
    sprintf(buffer, "%u", receivedMessage);

    Serial.println(buffer);
  }
}
