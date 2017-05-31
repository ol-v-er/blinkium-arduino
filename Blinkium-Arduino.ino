#include "Blinking.h"

#define BAUDS 9600

bool testBrightness = false;


Blinking blink(A0, 60, 40);

void setup() {
  Serial.begin(BAUDS);//For debugging
}

void loop() {
  if (testBrightness){
     Serial.println(analogRead(A0));
  } 
  if (!testBrightness){
    blink.listen();
    blink.serialPrintDatasAsBits();
    blink.serialPrintDatasAsStrings(); 
    blink.clear();
  }
}


