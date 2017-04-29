#include "Utils.h"

static int Utils::binToDec(Bit* data, int size){
  return Utils::binToDec(data, size, 0);
}

static int Utils::binToDec(Bit* data, int size, int start){
  int number = 0;
  int count = 0;
  for(int i = start+size-1; i >= start; i--, count++){
    number += data[i].value * Utils::power(2, count);
  }
  return number;
}

static int Utils::power(int a, int b){
  int result = 1;
  for(int i = 0; i < b; i++){
    result *= a;
  }
  return result;  
}

static char Utils::charOfBin(Bit* data, int start){
  return (char)Utils::binToDec(data, 8, start);
}

/*static int Utils::max(int a, int b){
  return a >= b ? a : b;
}

static int Utils::min(int a, int b){
  return a < b ? a : b;
}*/

