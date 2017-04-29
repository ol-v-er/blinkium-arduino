#ifndef TIMER_H
#define TIMER_H
#include <arduino.h>

class Timer{
  public:
    Timer(double period);
    void restart(bool instant);
    bool nextStep();
    void update();
    void setPeriod(double period);

  private:
    long time;
    long beginning;
    double lambdaAcc;
    double lambda;
};



#endif
