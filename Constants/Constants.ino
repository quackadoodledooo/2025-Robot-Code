#include <PestoLink-Receive.h>
#include <Alfredo_NoU3.h>
#include <Keys.h>
#include <FastLED.h>
#define pi 3.14159265359
#define FASTLED_ALLOW_INTERRUPTS 0  //FASTLED SERIAL INTERRUPTS ALLOWED TO ZERO
#define NUM_LEDS 8                  //NUM OF LEDS IN CHAIN
#define DATA_PIN 7                  //DATA PIN 5 (GPIO D5)
CRGB leds[NUM_LEDS];

const double Kp = 0.0;
const double Ki = 0.0;
const double Kd = 0.0;
const int SampleTime = 100; 
double error = 0.0;
double integral = 0.0;
double derivative = 0.0;
double previousError = 0.0;
double previousTime = 0.0;
double previousInput = 0.0;

NoU_Motor frontLeftMotor(1);
NoU_Motor frontRightMotor(8);
NoU_Motor rearLeftMotor(3);
NoU_Motor rearRightMotor(4);

NoU_Drivetrain drivetrain(&frontLeftMotor, &frontRightMotor, &rearLeftMotor, &rearRightMotor);
