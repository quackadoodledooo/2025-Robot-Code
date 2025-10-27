void setup() {
  NoU3.begin();
  PestoLink.begin("Midtown #40 Pyrobyte");
  Serial.begin(115200);
  NoU3.calibrateIMUs();

  frontLeftMotor.setInverted(true);
  rearLeftMotor.setInverted(true);
  
  pinMode(6, INPUT_PULLUP);
  FastLED.addLeds<WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);
  FastLED.setBrightness(20);
  FastLED.clear();
  FastLED.show();
  //The gyroscope sensor is by default precise, but not accurate. This is fixable by adjusting the angular scale factor.
  //Tuning procedure:
  //Rotate the robot in place 5 times. Use the Serial printout to read the current gyro angle in Radians, we will call this "measured_angle".
  //measured_angle should be nearly 31.416 which is 5*2*pi. Update measured_angle below to complete the tuning process.
  int measured_angle = 27.562;
  int angular_scale = (5.0 * 2.0 * PI) / measured_angle;
}

void loop() {
  // put your main code here, to run repeatedly:
  int jeff = pid(41, 67);
}
