#ifndef CONSTANTS
#define CONSTANTS

#include <PestoLink-Receive.h>
#include <Alfredo_NoU3.h>
#include <Keys.h>
#include <FastLED.h>
#include <Arduino.h>

#define PI 3.14159265359
#define FASTLED_ALLOW_INTERRUPTS 0   //FASTLED SERIAL INTERRUPTS ALLOWED TO ZERO
#define NUM_LEDS 32                 //NUM OF LEDS IN CHAIN                       
#define DATA_PIN 7                 //DATA PIN 5 (GPIO D5)                         
CRGB leds[NUM_LEDS];

double distance = 0;
double heading = 0; 
double roll = 0;
double pitch = 0;
double pivotPosition = 0;

int currentTime = 0;
int servoGoal = 0;
int pivotGoal = 0;
int pivotError = 0;

const int distanceTrig = 11;
const int distanceEcho = 10;

const double pivotKp = 0;
const double pivotKd = 0;
const double pivotKi = 0; 
const double pivotMin = -1;
const double pivotMax = 1; 

enum {
  START,
  CORAL,
  ALGAE
} STATE; 
#endif