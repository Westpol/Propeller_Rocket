/*
  Library for the MMA8452Q
  By: Jim Lindblom and Andrea DeVore
  SparkFun Electronics

  Do you like this library? Help support SparkFun. Buy a board!
  https://www.sparkfun.com/products/14587

  This sketch uses the SparkFun_MMA8452Q library to initialize
  the accelerometer and stream calcuated x, y, z, acceleration
  values from it (in g units).

  Hardware hookup:
  Arduino --------------- MMA8452Q Breakout
    3.3V  ---------------     3.3V
    GND   ---------------     GND
  SDA (A4) --\/330 Ohm\/--    SDA
  SCL (A5) --\/330 Ohm\/--    SCL

  The MMA8452Q is a 3.3V max sensor, so you'll need to do some
  level-shifting between the Arduino and the breakout. Series
  resistors on the SDA and SCL lines should do the trick.

  License: This code is public domain, but if you see me
  (or any other SparkFun employee) at the local, and you've
  found our code helpful, please buy us a round (Beerware
  license).

  Distributed as is; no warrenty given.
*/

/*
Help for the low pass filter here:
https://www.norwegiancreations.com/2016/03/arduino-tutorial-simple-high-pass-band-pass-and-band-stop-filtering/
*/

#include <Wire.h>                 // Must include Wire library for I2C
#include "SparkFun_MMA8452Q.h"    // Click here to get the library: http://librarymanager/All#SparkFun_MMA8452Q

MMA8452Q accel;                   // create instance of the MMA8452 class


unsigned long micro = 0;    // used for adding t into the Equations
unsigned long milli = 0;    // used for slowing down print functions (10 per Second right now)
float speed = 0;        // Speed in m/s
float position = 0;       // Position in Meters
float g = 9.81;       // Acceleration in Germany
float acc;

//lowpass Filter
float EMA_a = 0.05;    //initialization of EMA alpha
float EMA_S = 0;        //initialization of EMA S
int highpass = 0;


void setup() {
  Serial.begin(115200);
  Serial.println("MMA8452Q Basic Reading Code!");
  Wire.begin();

  if (accel.begin() == false) {
    Serial.println("Not Connected. Please check connections and read the hookup guide.");
    while (1);
  }
  accel.setDataRate(ODR_800);
  accel.setScale(SCALE_4G);
}

void loop() {
  if (accel.available()) {      // Wait for new data from accelerometer

    acc = accel.getCalculatedX() * 2.0;
    EMA_S = (EMA_a*acc) + ((1.0-EMA_a)*EMA_S);

    speed += EMA_S * g * ((micros() - micro) / 1000000.0);
    position += speed * ((micros() - micro) / 1000000.0);

    micro = micros();

  }

  if(milli <= millis()){
    Serial.print(speed);
    Serial.print("\t");
    Serial.print(position);
    Serial.println();
    milli += 100;
  }

}
