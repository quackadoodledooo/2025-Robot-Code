#include "PID.h"
#include "Constants.h"

PID pivotPID(pivotKp, pivotKi, pivotKd, pivotMin, pivotMax);

NoU_Motor frontLeftMotor(1);
NoU_Motor frontRightMotor(2);
NoU_Motor rearLeftMotor(3);
NoU_Motor rearRightMotor(4);
NoU_Motor pivot(7);

NoU_Servo elevatorLeft(1);
NoU_Servo elevatorRight(3);

NoU_Drivetrain drivetrain(&frontLeftMotor, &frontRightMotor, &rearLeftMotor, &rearRightMotor);

//The gyroscope sensor is by default precise, but not accurate. This is fixable by adjusting the angular scale factor.
//Tuning procedure:
//Rotate the robot in place 5 times. Use the Serial printout to read the current gyro angle in Radians, we will call this "measured_angle".
//measured_angle should be nearly 31.416 which is 5*2*pi. Update measured_angle below to complete the tuning process.
int measured_angle = 27.562;
int angular_scale = (5.0 * 2.0 * PI) / measured_angle;

void setup() {
  NoU3.begin();
  PestoLink.begin("Midtown #40 Pyrobyte");
  Serial.begin(115200);
  NoU3.calibrateIMUs();
  pivot.beginEncoder();

  frontLeftMotor.setInverted(true);
  rearLeftMotor.setInverted(true);
  
  pinMode(6, INPUT_PULLUP);
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);
  FastLED.setBrightness(20);
  FastLED.clear();
  FastLED.show();

}

void setLEDS() {
  if (STATE == START) {
    leds[0] = CRGB::Yellow;
  } else if (STATE == CORAL) {
    leds[0] = CRGB::White;
  } else if (STATE == ALGAE) {
    leds[0] = CRGB::Blue;
  }

}

void loop() {
  
  NoU3.updateIMUs();
  NoU3.updateServiceLight();
  heading = NoU3.yaw * angular_scale;
  roll = NoU3.roll * angular_scale;
  pitch = NoU3.pitch * angular_scale;
  currentTime = millis();
  pivotPosition = pivot.getPosition();
  setLEDS();

  float batteryVoltage = NoU3.getBatteryVoltage();
  PestoLink.printBatteryVoltage(batteryVoltage);

  if (PestoLink.buttonHeld(leftMain)) {
      PestoLink.rumble();
  }

  FastLED.show();

  if(PestoLink.keyHeld(Key::ArrowUp)) {
    servoGoal++;
  }
  if(PestoLink.keyHeld(Key::ArrowDown)) {
    servoGoal--;
  }
  if(PestoLink.keyHeld(Key::ArrowLeft)) {
    Serial.println("left");
    pivotGoal--;
    Serial.println("elevator up");
    Serial.println(pivotGoal);
    Serial.println(pivotPosition);
  }
  if(PestoLink.keyHeld(Key::ArrowRight)) {
    pivotGoal++;
    Serial.println("elevator down");
    Serial.println(pivotGoal);
    Serial.println(pivotPosition);
  }
  //switch modes
  if(PestoLink.buttonHeld(leftBumper)) {
    if(millis() - lastModeSwitch >= 200){
    if(STATE == CORAL) STATE = ALGAE;
    if(STATE == ALGAE) STATE = CORAL; 
    }
  }

  if(STATE == CORAL) { // CORAL MODE PRESETS
    if(PestoLink.buttonHeld(buttonA)) { //L1
      servoGoal = servoL1;
      diffL1 += currentTime - previousTime;
      if(diffL1 > 1000){
        pivotGoal = pivotL1;
        diffL1 = 0; 
      }
    }
    if(PestoLink.buttonHeld(buttonB)) { //L2
      servoGoal = servoL2;
      diffL2 += currentTime - previousTime;
      if(diffL2 > 1000){
        pivotGoal = pivotL2;
        diffL2 = 0; 
      }
    }
    if(PestoLink.buttonHeld(buttonX)) { //L3
      servoGoal = servoL3;
      diffL3 += currentTime - previousTime;
      if(diffL3 > 1000){
        pivotGoal = pivotL3;
        diffL3 = 0; 
      }
    }
    if(PestoLink.buttonHeld(buttonY)) { //L4
      servoGoal = servoL4;
      diffL4 += currentTime - previousTime;
      if(diffL4 > 1000){
        pivotGoal = pivotL4;
        diffL4 = 0; 
      }
    }
    if(PestoLink.buttonHeld(leftTrigger)) { //STOW/INTAKE
      servoGoal = servoSTOW;
      diffSTOW += currentTime - previousTime;
      if(diffSTOW > 1000){
        pivotGoal = pivotSTOW;
        diffSTOW = 0; 
      }
    }
  } else if (STATE == ALGAE) { // ALGAE MODE PRESETS
    if(PestoLink.buttonHeld(buttonB)) { //L2 algae
      servoGoal = servoAL2;
      diffAL2 += currentTime - previousTime;
      if(diffAL2 > 1000){
        pivotGoal = pivotAL2;
        diffAL2 = 0; 
      }
    }
    if(PestoLink.buttonHeld(buttonB)) { //L3 algae
      servoGoal = servoAL3;
      diffAL3 += currentTime - previousTime;
      if(diffAL3 > 1000){
        pivotGoal = pivotAL3;
        diffAL3 = 0; 
      }
    }
    if(PestoLink.buttonHeld(buttonB)) { //Barge
      servoGoal = servoBarge;
      diffBarge += currentTime - previousTime;
      if(diffBarge > 1000){
        pivotGoal = pivotBarge;
        diffBarge = 0; //hello
      }
    }
  }

  if(pivotGoal>180) pivotGoal = 360;
  if(pivotGoal<0) pivotGoal = 0;
  if(servoGoal>180) servoGoal = 360;
  if(servoGoal<0) servoGoal = 0;

  elevatorLeft.write(servoGoal);
  elevatorRight.write((-1 * servoGoal) + 180);
  
  pivotError = pivotGoal - pivotPosition;
  pivot.set(pivotPID.update(pivotError));
  
  previousTime = currentTime; 
}