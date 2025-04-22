#include <Arduino.h>
#include "GPS.h"
#include "Stepper.h"

// GPS UART
#define GPSSerial Serial1
#define GPS_RXPIN 9
#define GPS_TXPIN 10

// MOTOR1 PINS
#define MOTOR1_DIRPIN 12
#define MOTOR1_STEPPIN 13
#define MOTOR1_SLEEPPIN 14
#define MOTOR1_RATIO 42

// MOTOR1 PINS
#define MOTOR2_DIRPIN 15
#define MOTOR2_STEPPIN 16
#define MOTOR2_SLEEPPIN 17
#define MOTOR2_RATIO 42

HardwareSerial Serial;
HardwareSerial GPSSerial;
GPS myGPS;

int main()
{
  init();

  // Setup--------------------------------------------------------------------------

  // Setup GPS
  GPSSerial.setPins(GPS_RXPIN, GPS_TXPIN);
  GPS myGPS{GPSSerial};

  // Set up solar angle calculator
  PSA psaObject{};
  if(myGPS.isLatched()) { psaObject.setAll(myGPS.getDateTime(), myGPS.getLoc());}

  //Setup steppers
  Stepper stepper1{MOTOR1_DIRPIN, MOTOR1_STEPPIN, MOTOR1_SLEEPPIN, MOTOR1_RATIO};
  Stepper stepper2{MOTOR2_DIRPIN, MOTOR2_STEPPIN, MOTOR2_SLEEPPIN,MOTOR2_RATIO};

  // Setup IMU


  // Setup LCD


  // ------------------------------------------------------------------------------

  // Main loop
  while(true)

  {

  }

  return 0;
}