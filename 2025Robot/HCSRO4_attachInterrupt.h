/*
      v1.0 initial version by joshua-8 2024-12-11
*/
#ifndef H_HCSRO4_ATTACH_INTERRUPT_LIB_H
#define H_HCSRO4_ATTACH_INTERRUPT_LIB_H
#define NUM_HCSRO4_ISRS 8
// a workaround to let a class use attachInterrupts()
class Interruptable {
  public:
    virtual void callback() = 0;
};
void isrManager(byte i);
void ISR0() {
  isrManager(0);
}
void ISR1() {
  isrManager(1);
}
void ISR2() {
  isrManager(2);
}
void ISR3() {
  isrManager(3);
}
void ISR4() {
  isrManager(4);
}
void ISR5() {
  isrManager(5);
}
void ISR6() {
  isrManager(6);
}
void ISR7() {
  isrManager(7);
}
void ISR8() {
  isrManager(8);
}

void (*isrArrayVoids[NUM_HCSRO4_ISRS])(void) = { &ISR0, &ISR1, &ISR2, &ISR3, &ISR4, &ISR5, &ISR6, &ISR7};
boolean isrArrayElementAssigned[NUM_HCSRO4_ISRS] = { false, false, false, false, false, false, false, false};
Interruptable* isrArrayComponents[NUM_HCSRO4_ISRS] = { nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};

void isrManager(byte i)
{
  if (isrArrayElementAssigned[i] == true && isrArrayComponents[i] != nullptr) {
    isrArrayComponents[i]->callback();
  }
}

boolean isrManagerClaimPointer(void (**func)(void), int* pointerIndex, Interruptable* component)
{
  for (int i = 0; i < NUM_HCSRO4_ISRS; i++) {
    if (isrArrayElementAssigned[i] == false) {
      isrArrayElementAssigned[i] = true;
      *func = isrArrayVoids[i];
      *pointerIndex = i;
      isrArrayComponents[i] = component;
      return true;
    }
  }
  return false;
}

void isrManagerFreePointer(int num)
{
  isrArrayElementAssigned[num] = false;
  isrArrayComponents[num] = nullptr;
}


class HCSRO4 : public Interruptable {
  protected:
    void (*isra)(void);
    int isrAI;
    const byte trigPin;
    const byte echoPin;
    float distanceScalar;
    boolean justGotReading;
    volatile unsigned long highTime;
    volatile unsigned long microsWhenRise;
    volatile unsigned long microsWhenFall;
    unsigned long microsWhenTrig;
    unsigned long maxTimeToWaitMicros;
    unsigned long delayBetweenReadingsMicros;
    boolean waitingAfterPulse;
    volatile boolean echoPulseHigh;
    volatile boolean waitingForEcho;
    void trigPulse() {
      digitalWrite(trigPin, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigPin, LOW);
      microsWhenTrig = micros();
    }
  public:
    HCSRO4(byte _trigPin, byte _echoPin, float scalar = 1.0 / 58, unsigned long _maxTimeToWaitMicros = 20, unsigned long _delayBetweenReadingsMicros = 60):
      trigPin(_trigPin),
      echoPin(_echoPin)
    {
      distanceScalar = scalar;
      justGotReading = false;
      highTime = 0;
      microsWhenRise = 0;
      microsWhenFall = 0;
      microsWhenTrig = 0;
      waitingAfterPulse = true;
      echoPulseHigh = false;
      maxTimeToWaitMicros = _maxTimeToWaitMicros * 1000;
      delayBetweenReadingsMicros = _delayBetweenReadingsMicros * 1000;
      waitingForEcho = false;
    }
    boolean begin() {
      if (isrManagerClaimPointer(&isra, &isrAI, this) == false) {
        return false;
      }
      pinMode(trigPin, OUTPUT);
      digitalWrite(trigPin, LOW);
      pinMode(echoPin, INPUT);
      attachInterrupt(digitalPinToInterrupt(echoPin), isra, CHANGE);
      waitingAfterPulse = true;
      waitingForEcho = true;
      trigPulse();
      return true;
    }
    unsigned long getRawTime() {
      return highTime;
    }
    float getDistance() {
      return highTime * distanceScalar;
    }
    void setDistanceScalar(float scalar) {
      distanceScalar = scalar;
    }
    // returns true if there's a new distance or if the distance has been updated to 0 to indicate no echo received
    boolean run() {
      boolean returnVal = false;
      if (justGotReading) {
        justGotReading = false;
        returnVal = true;
        waitingAfterPulse = true; // for delay between readings
      }
      if (echoPulseHigh == false && waitingAfterPulse == true && (micros() - microsWhenFall) > delayBetweenReadingsMicros) {
        // delay between readings
        waitingAfterPulse = false;
        waitingForEcho = true;
        trigPulse();
      } else if (echoPulseHigh == false && waitingAfterPulse == false && (micros() - microsWhenTrig) > maxTimeToWaitMicros) {
        // it's been too long without a pulse, trig again
        highTime = 0; // distance will be zero
        returnVal = true;
        waitingForEcho = true;
        trigPulse();
      }
      return returnVal;
    }
    void callback() { // ISR
      if (waitingForEcho) {
        if (digitalRead(echoPin) == HIGH) {
          microsWhenRise = micros();
          echoPulseHigh = true;
        } else {
          microsWhenFall = micros();
          highTime = microsWhenFall - microsWhenRise;
          echoPulseHigh = false;
          justGotReading = true;
          waitingForEcho = false;
        }
      }
    }
    ~HCSRO4() {
      isrManagerFreePointer(isrAI);
    }
};
#endif
