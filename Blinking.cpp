#include "Blinking.h"

Blinking::Blinking(int sensorPin, double freq, int lightThreshold) : lightsensorPin(sensorPin), threshold(lightThreshold), timer(1000000/60), frequency(freq), lambda(1000000.0/freq) {}

void Blinking::listen(){
	detectStartCode();
  synchronize();
	receiveHeader();
	receiveDatas();

}

void Blinking::clear(){
  for(int i = 0; i < MAX_DATA_LENGTH; i++){
    datas[i].value = 0;
  }
}

void Blinking::detectStartCode(){
  bool start = false;
  int count;

  while(!start){   
      if (analogRead(lightsensorPin) > threshold){
        timer.restart(false);
        count = 1;
        while (count < STARTCODE_LENGTH){
          if(timer.nextStep()){
            timer.update();
            bitRead.value = readBit();
            //If we detect a "1" and there should be a "0", we break out of the loop and restart reading start code from the beginning
            if (startCode[count] == 0 && bitRead.value == 1){
              break;
            }
            else if (startCode[count] == 1 && bitRead.value == 0){
              break;
            }
            //If what we detect is what we should detect, we continue
            else{     
              count++;
            }
          }
        }
    
        if (count == STARTCODE_LENGTH){
          start = true; 
        }       
      }
  }
  if(DEBUG){
    Serial.println("Start code detected");
  }
}

void Blinking::synchronize(){
  int sampleSize = 100;
  long elapsedTime = 0;
  double count = 1;
  long beginning;
  long time = micros();
  int currentValue = 0;
  int bitRead = 1;
  int sensorValue;

  //Wait to enter synchronization sequence
  while (bitRead != currentValue){
     bitRead = analogRead(lightsensorPin) > threshold ? 1 : 0;
  }

  //Start the frequence mesuring
  beginning = micros();

  //Count the number of "periods"
  while(count <= sampleSize){
    currentValue = bitRead; 
    while(bitRead == currentValue){
        sensorValue = analogRead(lightsensorPin); 
        bitRead = sensorValue > threshold ? 1 : 0;

        //Mesure minimum and maximum brightness
        if (bitRead == 0)
          minBrightness = min(minBrightness, sensorValue);
        else
          maxBrightness = max(maxBrightness, sensorValue);
    }
    count++;
  }

  //Initialize variables for reading
  elapsedTime = (micros()-beginning);
  frequency = (double)sampleSize / (elapsedTime/1000000.0);
  lambda = 1000000.0/frequency;
  threshold = (minBrightness + maxBrightness) / 2;

  timer.setPeriod(lambda);
  timer.restart(false);

  if(DEBUG){
    Serial.print("Frequency : ");
    Serial.println(frequency);
    Serial.print("Min Brightness : ");
    Serial.println(minBrightness);
    Serial.print("Max Brightness : ");
    Serial.println(maxBrightness);
  }
  
}

/*
 * Header Composition
 * Data Length = 8 bits; 0000 0000 = 0; 1111 1111 = 255 //Number of groups of data
 * Data Size = 4 bits; 0000 = 0 Byte; 1111 = 15 Bytes // We can change 0000 to be 1 to improve the range
 */
void Blinking::receiveHeader(){
    Bit buf[12]; 
    
    readValues(buf, 12);
    dataLen = Utils::binToDec(buf, 12);

    if(DEBUG){
      Serial.println("Header Read");//For debugging
      Serial.println(dataLen);
    }
}


/*
 * Get the data transmitted
 */
void Blinking::receiveDatas(){
  readValues(datas, dataLen*8);
}

int Blinking::readBit(){
  boolean read = false;
  int value;
  long lastTime = micros();
  long newTime;

  //Wait to read the value in the middle of the period
  while (!read){
      newTime = micros();
      if(newTime - lastTime > 2*lambda/5.0){
        lastTime = newTime;
        value = analogRead(lightsensorPin) > threshold ? 1 : 0;
        read = true;   
      } 
  }
  
  return value;
}

void Blinking::readValues(Bit* buf, int number){
  int count = 0;
  while (count < number){
      if (timer.nextStep()){
        timer.update();
        buf[count].value = readBit();
        count++; 
     }
  }  
}

Bit* Blinking::getDatas(){
  return datas;
}

void Blinking::serialPrintDatasAsStrings(){
  int index = 0;
  while (index < dataLen*8){
    int size = 0;
    while(Utils::charOfBin(datas, index+size*8) != '\0'){
      size++;
    }
  
    char* s = (char*)malloc((size+1)*sizeof(char));
    for(int i = 0; i < size; i++){
      s[i] = Utils::charOfBin(datas,index+i*8);
    }
    s[size] = '\0';

    
    Serial.print(s);
    Serial.print('\t');
    free(s);
  
    index += (size+1)*8;
  }

  Serial.println();
}

void Blinking::serialPrintDatasAsBits(){
    for (int i = 0; i < dataLen*8; i++){
      Serial.print(datas[i].value);
    }
    Serial.println();
}

