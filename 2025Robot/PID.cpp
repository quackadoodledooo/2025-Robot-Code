#include "PID.h"
#include "Constants.h"

PID:PID(float kp, float ki, float kd, float min, float max) {
  this->kp = kp;
  this->ki = ki;
  this->kd = kd;
  this->min = min;
  this->max = max;
  this->previousError = 0;
  this->integral = 0; 
  this->start = 0;
  this->previousTime = 0;
}

PID:update(float error, float velocity) {
  unsigned long current_time = millis(); // get the current time in milliseconds
  float timestep; 

  if (this->previousTime == 0) {
    this->start = current_time;
    timestep = 0.001; //set an initial timestep of 1ms
  } else {
    timestep = (current_time - this->previousTime) / 1000; // gets timestep in seconds
  }

  this->previousTime = current_time;

  float derivative = 0;
  if (timestep != 0) 
    derivative = (error - this->previousError) / timestep; // calculate derivative part

  this->integral += error*timestep; // calulate integral part

  integral = constrain(this->integral, -max_integral, max_integral); //clap integral to prevent it from winding up

  //calulae ouput
  float output = this->kp * error + this->ki * this->integral + this->kd * derivative;
  this->previousError = error;

  //constrain the ouput value to make sure its within limits
  output = constrain(output, this->min, this->max);
  
  return output;
}

void PID::clear() {
  this->previousError = 0;
  this->previousTime = 0; 
}