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

double distance = 0;
double heading = 0; 
double roll = 0;
double pitch = 0;
double pivotPosition = 0;

int currentTime = 0;
int previousTime = 0;
int servoGoal = 0;
int pivotGoal = 0;
int pivotError = 0;
int lastModeSwitch = 0;

const int distanceTrig = 11;
const int distanceEcho = 10;

//diff variables to be used in controls to control delay
double diffSTOW = 0;
double diffL2 = 0;
double diffL3 = 0;
double diffL4 = 0;
double diffAL2 = 0;
double diffAL3 = 0;
double diffBarge = 0;

//pivotPID constants
const double pivotKp = 0.2;
const double pivotKd = 0.1;
const double pivotKi = 0.005; 
const double pivotMin = -1;
const double pivotMax = 1; 

//Preset values for elevator
const double servoSTOW = 0;
const double servoReady = 20;
const double servoL2 = 0;
const double servoL3 = 0;
const double servoL4 = 140;
const double servoAL2 = 120;
const double servoAL3 = 0;
const double servoBarge = 180;

//Preset values for pivot
const double pivotSTOW = 0;
const double pivotReady = 40;
const double pivotL2 = 360;
const double pivotL3 = 500;
const double pivotL4 = 500;
const double pivotAL2 = 0;
const double pivotAL3 = 600;
const double pivotBarge = 700;

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

//swerve vars
double turnMag = 0.0;
double driveAngle = 0.0;
double driveMag = 0.0;
double drivetrainVectors[4][2] = { { 0, 0 },
                                   { 0, 0 },
                                   { 0, 0 },
                                   { 0, 0 } };

double theta;
double headingOffset = 0.0;
int mod1Offset = 0;
int mod2Offset = 0;
int mod3Offset = 0;
int mod4Offset = 0;
int lastOffsetTime = millis();

const bool AM_DEBUGGING = false;

enum {
  START,
  CORAL,
  ALGAE
} STATE;

#endif