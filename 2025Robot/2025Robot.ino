#include "PID.h"
#include "Constants.h"

PID pivotPID(pivotKp, pivotKi, pivotKd, pivotMin, pivotMax);

NoU_Motor frontLeftMotor(1);
NoU_Motor frontRightMotor(2);
NoU_Motor rearLeftMotor(3);
NoU_Motor rearRightMotor(4);
NoU_Motor pivot(5);

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

  if (PestoLink.keyHeld(Key::Numpad9)) {
      PestoLink.rumble();
  }

  FastLED.show();

  if(PestoLink.keyHeld(Key::ArrowUp)) {
    if (servoGoal > 0 && servoGoal < 180) {
      servoGoal++;
    }
  }
  if(PestoLink.keyHeld(Key::ArrowDown)) {
    if (servoGoal > 0 && servoGoal < 180) {
      servoGoal--;
    }
  }
  if(PestoLink.keyHeld(Key::ArrowLeft)) {
    if (pivotGoal > 0 && pivotGoal < 180) {
      pivotGoal--;
    }
  }
  if(PestoLink.keyHeld(Key::ArrowRight)) {
    if (pivotGoal > 0 && pivotGoal < 180) {
      pivotGoal++;
    }
  }
  

  elevatorLeft.write(servoGoal);
  elevatorRight.write(servoGoal);
  
  pivotError = pivotPosition - pivotGoal;
  pivot.set(pivotPID.update(pivotError));
}