#ifndef AUTOS
#define AUTOS
#include <Arduino.h>

/*void leftL4Barge() {

}



void rightL4Barge() {

}



void centerL4Barge() {

}



void leftL4Load() {

}



void rightL4Load() {
  
  //move forward for 3 seconds while moving elevator and pivot to scoring position;
  Turn4.write(90);
  Turn3.write(90);
  Turn2.write(90);
  Turn1.write(90);
  Drive4.set(-1);
  Drive3.set(-1);
  Drive2.set(-1);
  Drive1.set(-1);
  servoGoal = servoL4;
  delay(1000);
  pivotGoal = pivotL4;
  delay(200);
  Drive4.set(0);
  Drive3.set(0);
  Drive2.set(0);
  Drive1.set(0);

  //score coral;
  coral.set(1);
  delay(500);
  coral.set(0);
  
  //move backward while stowing pivot/elevator
  Drive4.set(1);
  Drive3.set(1);
  Drive2.set(1);
  Drive1.set(1);
  delay(500);
  pivotGoal = pivotSTOW;
  delay(1000);
  servoGoal = servoSTOW;
  Drive4.set(0);
  Drive3.set(0);
  Drive2.set(0);
  Drive1.set(0);

  //turn
  Turn4.write(135);
  Turn3.write(45);
  Turn2.write(135);
  Turn1.write(45);
  delay(500);
  Drive4.set(1);
  Drive3.set(-1);
  Drive2.set(1);
  Drive1.set(-1);
  delay(500);
  Drive4.set(0);
  Drive3.set(0);
  Drive2.set(0);
  Drive1.set(0);

  //move towards coral station with funnel pointing towards station
  Turn4.write(135);
  Turn3.write(45);
  Turn2.write(135);
  Turn1.write(45);
  Drive4.set(1);
  Drive3.set(1);
  Drive2.set(1);
  Drive1.set(1);
  delay(5000);
  Drive4.set(0);
  Drive3.set(0);
  Drive2.set(0);
  Drive1.set(0);

}



void leave() {
  Drive4.set(1);
  Drive3.set(1);
  Drive2.set(1);
  Drive1.set(1);
  delay(2000);
  Drive4.set(0);
  Drive3.set(0);
  Drive2.set(0);
  Drive1.set(0);
}
*/
#endif
