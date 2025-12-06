#ifndef VARS
#define VARS

#include <PestoLink-Receive.h>
#include <Alfredo_NoU3.h>
#include <Keys.h>
#include <FastLED.h>
#include <Arduino.h>
#include <cmath>
#include <tgmath.h>

#define PI 3.14159265359
#define FASTLED_ALLOW_INTERRUPTS 0   //FASTLED SERIAL INTERRUPTS ALLOWED TO ZERO
#define NUM_LEDS 32                 //NUM OF LEDS IN CHAIN                       
#define DATA_PIN 39                 //DATA PIN 5 (GPIO D5)                         
CRGB leds[NUM_LEDS];

//General robot variables
  double distance = 0;
  double heading = 0; 
  double roll = 0;
  double pitch = 0;
  int currentTime = 0;
  int previousTime = millis();
  bool autoRan = false;
  enum {
    START,
    CORAL,
    ALGAE
  } STATE;

//diff variables to be used in controls to control delay
  int diffSTOW = 0;
  int diffL2 = 0;
  int diffL3 = 0;
  int diffL4 = 0;
  int diffAL2 = 0;
  int diffAL3 = 0;
  int diffBarge = 0;
  int lastModeSwitch = 0;
  int diffAuto = 0; 

//Pivot variables
  int pivotGoal = 0;
  int pivotError = 0;
  double pivotPosition = 0;
  //pivotPID constants
    const float pivotKp = 0.2;
    const float pivotKd = 0.1;
    const float pivotKi = 0.005; 
    const float pivotMin = -1;
    const float pivotMax = 1; 

  //Preset values for pivot
    const int pivotSTOW = 0;
    const int pivotReady = 40;
    const int pivotL2 = 360;
    const int pivotL3 = 500;
    const int pivotL4 = 500;
    const int pivotAL2 = 0;
    const int pivotAL3 = 600;
    const int pivotBarge = 700;

//Variables for elevator
  int servoGoal = 0;
  //Preset values for elevator
    const int servoSTOW = 0;
    const int servoReady = 20;
    const int servoL2 = 0;
    const int servoL3 = 0;
    const int servoL4 = 120;
    const int servoAL2 = 100;
    const int servoAL3 = 0;
    const int servoBarge = 130;

//Bindings for Xbox controller
  const int buttonA = 0;
  const int buttonB = 1;
  const int buttonX = 2;
  const int buttonY = 3;
  const int leftBumper = 4;
  const int rightBumper = 5; 
  const int leftTrigger = 6;
  const int rightTrigger = 7;
  const int leftMain = 8;
  const int rightMain = 9;
  const int leftStick = 10;
  const int rightStick = 11;
  const int upDPad = 12;
  const int downDPad = 13;
  const int leftDPad = 14;
  const int rightDPad = 15;

//Swerve vars
  double turnMag = 0.0;
  double driveAngle = 0.0;
  double driveMag = 0.0;
  double drivetrainVectors[4][2] = { { 0, 0 },
                                    { 0, 0 },
                                    { 0, 0 },
                                    { 0, 0 } };

  double theta;
  double headingOffset = 0.0;
  int mod1Offset = 405;
  int mod2Offset = 135;
  int mod3Offset = 315;
  int mod4Offset = 225;
  int lastOffsetTime = millis();
  const bool AM_DEBUGGING = false;

#endif