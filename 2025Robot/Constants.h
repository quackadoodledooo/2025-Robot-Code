#include <PestoLink-Receive.h>
#include <Alfredo_NoU3.h>
#include <HCSRO4_attachInterrupt.h>
#include <Keys.h>
#include <FastLED.h>
#include <Arduino.h>

#define pi 3.14159265359
#define FASTLED_ALLOW_INTERRUPTS 0  //FASTLED SERIAL INTERRUPTS ALLOWED TO ZERO
#define NUM_LEDS 8                  //NUM OF LEDS IN CHAIN
#define DATA_PIN 7                  //DATA PIN 5 (GPIO D5)
CRGB leds[NUM_LEDS];

NoU_Motor frontLeftMotor(1);
NoU_Motor frontRightMotor(8);
NoU_Motor rearLeftMotor(3);
NoU_Motor rearRightMotor(4);

NoU_Drivetrain drivetrain(&frontLeftMotor, &frontRightMotor, &rearLeftMotor, &rearRightMotor);
HCSRO4 distanceSensor = HCSRO4(distanceTrig, distanceEcho, 1.0 / 148);

typedef enum {
  START = 0,
  CORAL = 1,
  ALGAE = 2,
  CLIMB = 3
} STATE; 