#include "PID.h"
#include "Constants.h"

PID pivotPID(pivotKp, pivotKi, pivotKd, pivotMin, pivotMax);

NoU_Motor Drive1(1);
NoU_Servo Turn1(1, 500, 2500);
NoU_Motor Drive2(2);
NoU_Servo Turn2(2, 500, 2500);
NoU_Motor Drive3(3);
NoU_Servo Turn3(3, 500, 2500);
NoU_Motor Drive4(4);
NoU_Servo Turn4(4, 500, 2500);

NoU_Motor algae1(5);
NoU_Motor algae2(6);
NoU_Motor coral(8);
NoU_Motor pivot(7);

NoU_Servo elevatorLeft(5);
NoU_Servo elevatorRight(6);

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
  xTaskCreatePinnedToCore(taskUpdateSwerve, "taskUpdateSwerve", 4096, NULL, 2, NULL, 1);
  
  pinMode(6, INPUT_PULLUP);
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);
  FastLED.setBrightness(20);
  FastLED.clear();
  FastLED.show();

  //Motor Configs
  Drive1.setMinimumOutput(0.6);
  Drive2.setMinimumOutput(0.6);
  Drive3.setMinimumOutput(0.6);
  Drive3.setMinimumOutput(0.6);
  Drive1.setExponent(2);
  Drive2.setExponent(2);
  Drive3.setExponent(2);
  Drive4.setExponent(2);

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
  
  /**
  CONTROLS
  Up/Down arrows - Manual elevator control
  Left/Right arrows - Manual pivot control
  Left bumper - Switch modes
  Right bumper - Prepare to score
  CORAL MODE:
    A - L2
    B - L3
    Y - L4
    X - Stow
    Left trigger - Intake
    Right trigger - Outtake
  ALGAE MODE:
    Y - L2
    A - L3
    B - Barge
    Left trigger - intake
    Right trigger - outtake
  */

  if(PestoLink.keyHeld(Key::ArrowUp)) {
    servoGoal++;
  }
  if(PestoLink.keyHeld(Key::ArrowDown)) {
    servoGoal--;
  }
  if(PestoLink.keyHeld(Key::ArrowLeft)) {
    pivotGoal--;
  }
  if(PestoLink.keyHeld(Key::ArrowRight)) {
    pivotGoal++;
  }
  //switch modes
  if(PestoLink.buttonHeld(leftBumper)) {
    if(millis() - lastModeSwitch >= 200){
      if(STATE == CORAL) {
         STATE = ALGAE;
         lastModeSwitch = millis();
         coral.setBrakeMode(true);
         algae1.setBrakeMode(false);
         algae2.setBrakeMode(false);
      }
      if(STATE == ALGAE){
         STATE = CORAL;
         lastModeSwitch = millis();
         coral.setBrakeMode(false);
         algae1.setBrakeMode(true);
         algae2.setBrakeMode(true);
      } 
    }
  }

  if(PestoLink.buttonHeld(rightBumper)) { //Prepare to score
      servoGoal = servoReady+20;
      diffL2 += currentTime - previousTime;
      if(diffL2 > 1000){
        pivotGoal = pivotReady;
        servoGoal = servoReady;
        diffL2 = 0; 
      }
    }

  if(STATE == CORAL) { // CORAL MODE PRESETS
    if(PestoLink.buttonHeld(buttonA)) { //L2
      servoGoal = servoL2+20;
      diffL2 += currentTime - previousTime;
      if(diffL2 > 1000){
        pivotGoal = pivotL2;
        servoGoal = servoL2;
        diffL2 = 0; 
      }
    }
    if(PestoLink.buttonHeld(buttonB)) { //L3
      servoGoal = servoL3+20;
      diffL3 += currentTime - previousTime;
      if(diffL3 > 1000){
        pivotGoal = pivotL3;
        servoGoal = servoL3;
        diffL3 = 0; 
      }
    }
    if(PestoLink.buttonHeld(buttonX)) { //Stow
      pivotGoal = pivotSTOW;
      servoGoal = servoSTOW+20;
      diffSTOW += currentTime - previousTime;
      if(diffSTOW > 1000){
        servoGoal = servoSTOW;
        diffSTOW = 0; 
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
    if(PestoLink.buttonHeld(leftTrigger)) { //INTAKE
       coral.set(1);
    }
    if(PestoLink.buttonHeld(rightTrigger)){ //OUTTAKE
      coral.set(-1);
    }

  } else if (STATE == ALGAE) { // ALGAE MODE PRESETS
    if(PestoLink.buttonHeld(buttonY)) { //L2 algae
      servoGoal = servoAL2+20;
      diffAL2 += currentTime - previousTime;
      if(diffAL2 > 1000){
        pivotGoal = pivotAL2;
        servoGoal = servoAL2;
        diffAL2 = 0; 
      }
    }
    if(PestoLink.buttonHeld(buttonA)) { //L3 algae
      servoGoal = servoAL3 + 20;
      diffAL3 += currentTime - previousTime;
      if(diffAL3 > 1000){
        pivotGoal = pivotAL3;
        servoGoal = servoAL3;
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
    if(PestoLink.buttonHeld(leftTrigger)){ //INTAKE
      algae1.set(1);
      algae2.set(-1);
    }
    if(PestoLink.buttonHeld(rightTrigger)){// OUTTAKE
      algae1.set(-1);
      algae2.set(1);
    }
  }

  if(pivotGoal>720) pivotGoal = 720;
  if(pivotGoal<0) pivotGoal = 0;
  if(servoGoal>180) servoGoal = 180;
  if(servoGoal<0) servoGoal = 0;

  elevatorLeft.write(servoGoal);
  elevatorRight.write((-1 * servoGoal) + 180);
  
  pivotError = pivotGoal - pivotPosition;
  pivot.set(pivotPID.update(pivotError));
  Serial.println(pivotPosition);
  Serial.println(pivotGoal);
  
  previousTime = currentTime; 
}

void taskUpdateSwerve(void* pvParameters){ // written by Julien
  while(true) {
    // Set up Gyro and its variables
    theta = NoU3.yaw - headingOffset;

    // get magnitude and direction and assign to drivetrainVectors array, add offsets
    // set turn vector magnitude
    // set RSL based on whether a gamepad is connected
    // ANGLES IN RADIANS
    if (PestoLink.isConnected()) {

      driveAngle = atan2(PestoLink.getAxis(1), PestoLink.getAxis(0));
      driveMag = sqrt(pow(PestoLink.getAxis(1), 2) + pow(PestoLink.getAxis(0), 2));
      turnMag = PestoLink.getAxis(2);

      drivetrainVectors[0][0] = driveMag;
      drivetrainVectors[0][1] = driveAngle + ((mod1Offset + theta) * (PI / 180));

      drivetrainVectors[1][0] = driveMag;
      drivetrainVectors[1][1] = driveAngle + ((mod2Offset + theta) * (PI / 180));

      drivetrainVectors[2][0] = driveMag;
      drivetrainVectors[2][1] = driveAngle + ((mod3Offset + theta) * (PI / 180));

      drivetrainVectors[3][0] = driveMag;
      drivetrainVectors[3][1] = driveAngle + ((mod4Offset + theta) * (PI / 180));

      NoU3.setServiceLight(LIGHT_ENABLED);
    } else {
      NoU3.setServiceLight(LIGHT_DISABLED);
    }

    //Heading Offset Control

    if (PestoLink.isConnected() && PestoLink.buttonHeld(10) && PestoLink.buttonHeld(11)) {
      headingOffset = NoU3.yaw;
    }



    //Vector Addition
    //Finds the component form of the current drive vector on the unit circle for each individual module
    //Uses the fact that turn vector is always 0 degrees, adds it to x coordinate.
    //reconverts back into magnitude and directon form
    //ANGLES IN DEGREES

    double xCord1 = drivetrainVectors[0][0] * cos(drivetrainVectors[0][1]) + turnMag;
    double yCord1 = drivetrainVectors[0][0] * sin(drivetrainVectors[0][1]);

    double xCord2 = drivetrainVectors[1][0] * cos(drivetrainVectors[1][1]) + turnMag;
    double yCord2 = drivetrainVectors[1][0] * sin(drivetrainVectors[1][1]);

    double xCord3 = drivetrainVectors[2][0] * cos(drivetrainVectors[2][1]) + turnMag;
    double yCord3 = drivetrainVectors[2][0] * sin(drivetrainVectors[2][1]);

    double xCord4 = drivetrainVectors[3][0] * cos(drivetrainVectors[3][1]) + turnMag;
    double yCord4 = drivetrainVectors[3][0] * sin(drivetrainVectors[3][1]);

    double cordArray[4] = { abs(xCord1), abs(xCord2), abs(xCord3), abs(xCord4) };

    //Find max x coordinate (since only adding to x)
    double massiveCord = 0.0;
    for (int i = 0; i < 3; i++) {
      if (cordArray[i] > massiveCord) {
        massiveCord = cordArray[i];
      }
    }
    //scales all components to the one with the largest magnitude
    if (massiveCord > 1 && massiveCord != 0) {
      xCord1 /= massiveCord;
      yCord1 /= massiveCord;
      xCord2 /= massiveCord;
      yCord2 /= massiveCord;
      xCord3 /= massiveCord;
      yCord3 /= massiveCord;
      xCord4 /= massiveCord;
      yCord4 /= massiveCord;
    }

    //component form --> magnitude and direction form as semifinal values
    drivetrainVectors[0][1] = atan2(yCord1, xCord1) * (180 / PI);
    drivetrainVectors[0][0] = sqrt(pow(xCord1, 2) + pow(yCord1, 2));
    drivetrainVectors[1][1] = atan2(yCord2, xCord2) * (180 / PI);
    drivetrainVectors[1][0] = sqrt(pow(xCord2, 2) + pow(yCord2, 2));
    drivetrainVectors[2][1] = atan2(yCord3, xCord3) * (180 / PI);
    drivetrainVectors[2][0] = sqrt(pow(xCord3, 2) + pow(yCord3, 2));
    drivetrainVectors[3][1] = atan2(yCord4, xCord4) * (180 / PI);
    drivetrainVectors[3][0] = sqrt(pow(xCord4, 2) + pow(yCord4, 2));

    //Wrapping functions for 360 degree motion with 180 degree servos
    if (drivetrainVectors[0][1] < -1) {
      drivetrainVectors[0][1] += 181;
      drivetrainVectors[0][0] *= -1;
    }
    if (drivetrainVectors[0][1] > 181) {
      drivetrainVectors[0][1] -= 181;
      drivetrainVectors[0][0] *= -1;
    }

    if (drivetrainVectors[1][1] < -1) {
      drivetrainVectors[1][1] += 181;
      drivetrainVectors[1][0] *= -1;
    }
    if (drivetrainVectors[1][1] > 181) {
      drivetrainVectors[1][1] -= 181;
      drivetrainVectors[1][0] *= -1;
    }

    if (drivetrainVectors[2][1] < -1) {
      drivetrainVectors[2][1] += 181;
      drivetrainVectors[2][0] *= -1;
    }
    if (drivetrainVectors[2][1] > 181) {
      drivetrainVectors[2][1] -= 181;
      drivetrainVectors[2][0] *= -1;
    }

    if (drivetrainVectors[3][1] < -1) {
      drivetrainVectors[3][1] += 181;
      drivetrainVectors[3][0] *= -1;
    }
    if (drivetrainVectors[3][1] > 181) {
      drivetrainVectors[3][1] -= 181;
      drivetrainVectors[3][0] *= -1;
    }


    //write to drivetrain + deadzone
    if (PestoLink.isConnected() && (abs(PestoLink.getAxis(0)) + abs(PestoLink.getAxis(1)) + abs(PestoLink.getAxis(2))) > 0.02) {
      Turn1.write(int(drivetrainVectors[0][1]));
      Drive1.set(drivetrainVectors[0][0]);
      Turn2.write(int(drivetrainVectors[1][1]));
      Drive2.set(drivetrainVectors[1][0]);
      Turn3.write(int(drivetrainVectors[2][1]));
      Drive3.set(drivetrainVectors[2][0]);
      Turn4.write(int(drivetrainVectors[3][1]));
      Drive4.set(drivetrainVectors[3][0]);
    } else {
      Turn1.write(0);
      Drive1.setBrakeMode(true);
      Turn2.write(0);
      Drive2.setBrakeMode(true);
      Turn3.write(0);
      Drive3.setBrakeMode(true);
      Turn4.write(0);
      Drive4.setBrakeMode(true);
    }

    vTaskDelay(pdMS_TO_TICKS(10));  //this line is like arduino delay() but for rtos tasks
  }
}