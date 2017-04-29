#include "Blinking.h"

bool testBrightness = false;


Blinking blink(A0, 60, 50);

void setup() {
  Serial.begin(9600);//For debugging

  if (!testBrightness)
    blink.listen();
}

void loop() {
  if (testBrightness){
     Serial.println(analogRead(A0));
  } 
}


