#include "Timer.h"

Timer::Timer(double period) : lambda(period){
  restart(true);
}

void Timer::restart(bool instant){
  beginning = micros();
  
  if (instant)
    lambdaAcc = 0;
  else
    lambdaAcc = lambda;
}

bool Timer::nextStep(){
  time = micros();
  return (time - beginning) > lambdaAcc;
}

void Timer::update(){
  lambdaAcc += lambda;
}

void Timer::setPeriod(double period){
  lambda = period;
}

