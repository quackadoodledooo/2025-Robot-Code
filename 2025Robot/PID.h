#ifndef PID
#define PID

#include <Arduino.h>

class PID {
  public:
    PID(float kp; float ki; float kd; float min; float max);
    float update(float error, float velocity = 0);
    void clear();
  private: 
    float kp,ki,kd;
    float min, max;
    float previousError, integral;
    unsigned long start, previous
};

#endif
