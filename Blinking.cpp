#include "Blinking.h"

Blinking::Blinking(int sensorPin) : lightsensorPin(sensorPin) {}

void Blinking::listen(){
	detectStartCode();
  synchronise();
	getHeader();
	getDatas();
}

void Blinking::detectStartCode(){
  bool start = false;
  long t;
  int lightValue = lightLevel; //Will be changed to adapt to differents brightnesses 

  while(!start){
    if(micros() - time2 >  1000000.0/480.0){
      time2 = micros();     
      sensorValue = analogRead(lightsensorPin);
      //Serial.println(sensorValue);
      if (sensorValue > lightValue){
        begining = micros();        
        time = begining;
        lambdaAcc += lambda;
        int count = 1;
        while (count < 3){
          t = micros();
          if( t - begining >  lambdaAcc){
            time = t;
            bitRead.value = detectBit();  
            lambdaAcc += lambda;
            //Serial.println(count); //For debugging
            //Serial.println(bitRead); //For debugging
            //If we detect a "1" and there should be a "0", we break out of the loop and restart reading start code from the begenning.
            //Needs more work because we can "miss" the start code
            //Maybe store the "startCodeLenght" last values and compare thoses with the start code, to avoid missing it.
            if (startCode[count] == 0 && bitRead.value == 1){
              begining = 0;
              lambdaAcc = 0;
              break;
            }
            else if (startCode[count] == 1 && bitRead.value == 0){
              begining = 0;
              lambdaAcc = 0;
              break;
            }
            //If what we detect is what we should detect, we continue
            else{     
              count++;
            }
          }
        }
    
        if (count == 3) // Will be changed. We need to compare with the size of the Start Code.
          start = true;        
      }
    }
  }
 Serial.println("Start code detected");
 time2 = micros();
}

void Blinking::synchronise(){
  long sum = 0;
  double a = 0;
  double mean;
  long deb = micros();
  long time = millis();
  long t;
  int currentValue;
  int b = 0;
  Bit temp[1];

  readValues(temp, 1, 0);

  while(a < 60){
    currentValue = b;
    while(b == currentValue){
      t = millis();
      if (t - time >  1){
        time = t;
        b = analogRead(lightsensorPin) > lightLevel ? 1 : 0;
      }
    }
    a+=1;
  }  

  sum = (micros()-deb);

  /*mean = sum/(a/(sum/1000.0));
  frequency = 1000.0/mean;*/
  frequency = 60.0 / (sum/1000000.0);
  //frequency = 59.559261;
  lambda = 1000000.0/frequency;

  lambdaAcc = lambda;
  begining = micros();

  /*Serial.println("Elapsed time");
  Serial.println(millis()-deb);
  Serial.println("Sum");
  Serial.println(sum);
  Serial.println("Frequency");
  Serial.println(frequency);*/


  
}

/*
 * Header Composition
 * Data Length = 8 bits; 0000 0000 = 0; 1111 1111 = 255 //Number of groups of data
 * Data Size = 4 bits; 0000 = 0 Byte; 1111 = 15 Bytes // We can change 0000 to be 1 to improve the range
 */
void Blinking::getHeader(){
    Bit buf[8]; 
    //begining = begining + (begining - time2);
    
    readValues(buf, 8, 0);
    dataLen = binToDec(buf);

    readValues(buf, 4, 4);
    dataSize = binToDec2(buf, 4);
    
    Serial.println("Header Read");//For debugging
    Serial.println(dataLen);
    Serial.println(dataSize);
}


/*
 * Get the data transmitted
 */
void Blinking::getDatas(){
  int i = 0;
  int lightValue = lightLevel;

  readValues(datas, dataLen * dataSize * 8, 0);
  Serial.println("Datas Read");

  for (i = 0; i < dataLen*dataSize*8; i++){//For debbugging
    Serial.print(datas[i].value);
  }
}

int Blinking::binToDec(Bit data[8]){
  int number = 0;
  int count = 0;
  for(int i = 7; i >= 0; i--, count++){
    number += data[i].value*power(2, count);
  }
  return number;
}

int Blinking::binToDec2(Bit data[8], int s){
  int number = 0;
  int count = 0;
  for(int i = 7; i >= s; i--, count++){
    number += data[i].value*power(2, count);
  }
  return number;
}

int Blinking::power(int a, int b){
  int result = 1;
  for(int i = 0; i < b; i++){
    result *= a;
  }
  return result;  
}

int Blinking::detectBit(){
  boolean read = false;
  int sum = 0;
  long timer = micros();
  long t;
  float div = 5.0;
  
  for(int i = 0, count = 0, s = 2; i < 3; i++){
	while (!read){
		t = micros();
		if(t - timer > s*lambda/div){
			timer = t;
			sum = analogRead(lightsensorPin) > lightLevel ? sum + 1 : sum;
			read = true; 
			s = 1;			
		} 
	}
	read = false;
  }

  if (sum  > 1)
    return 1;
  else 
    return 0;
}

void Blinking::readValues(Bit* buf, int nb, int dec){
  int count = 0;
  long t;
  long time = 0;
  while (count < nb){
      t = micros();
      if (t - begining >  lambdaAcc){
        buf[count+dec].value = detectBit();
        time = t;
        lambdaAcc += lambda;    
        count++; 
     }
    }  
}
