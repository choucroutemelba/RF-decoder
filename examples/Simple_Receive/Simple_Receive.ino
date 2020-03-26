/*
  Simple example for receiving
  
  https://github.com/sui77/rc-switch/
*/

#include <RFdecoder.h>

RFdecoder RF = RFdecoder(0);		// Receiver on interrupt 0 => that is pin #2

void setup() {
  Serial.begin(57600);
}

void loop() {
  if (RF.available()) {
    
    Serial.print("Received ");
    Serial.println( RF.getValue() );
    RF.reset();
  }
}
