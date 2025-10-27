float pid(float currentMotorPosition, float goalMotorPosition) {
  unsigned long now = millis();  
  double timeChange = (double)(now - previousTime);
  if (timeChange >= SampleTime) {
    error = (goalMotorPosition - currentMotorPosition);// calculate error
    integral += error * Ki;//calculate error over time
    derivative = (currentMotorPosition - previousInput);// calculate how much error is changing

    float output = error * Kp + integral - derivative * Kd;//calculate error

    if (abs(error) < 0.005) {
      output = 0; 
      integral = 0;
    } else {
      if (output > 0 && output < 0.1) output = 0.1;
      if (output > -0.1 && output < 0) output = -0.1;
    }

    if (output > 1) output = 1;
    if (output < -1) output = -1;

    previousError = error;
    previousTime = now;
    previousInput = currentMotorPosition;

    return output;
  }
}