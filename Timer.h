#ifndef TIMER_H
#define TIMER_H
#include <arduino.h>

class Timer{
  public:
    Timer(double period);
    void restart(bool instant);
    bool nextStep(); //Did we wait long enough ?
    void update(); //Go to the next step. Have to be call after entering the current step.
    void setPeriod(double period);

  private:
    long time;
    long beginning;
    double lambdaAcc;
    double lambda;
};



#endif
