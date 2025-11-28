#ifndef PIDH
#define PIDH
#include <Arduino.h>

class PID {
public:
  PID(float kp, float ki, float kd, float min, float max);
  float update(float error);
  void clear();
private: 
  float kp,ki,kd;
  float min, max;
  float previousError, integral;
  unsigned long start, previous;
};

#endif
