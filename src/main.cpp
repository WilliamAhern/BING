#include <Arduino.h>
#include "MenuManager.h"
#include "RotaryEncoder.h"
#include "GPS.h"
#include "Stepper.h"
#include "IMU.h"
#include "INA260.h"


// MOTOR1 PINS
#define MOTOR1_DIRPIN 2
#define MOTOR1_STEPPIN 0
#define MOTOR1_SLEEPPIN 15
#define MOTOR1_RATIO 60

// MOTOR2 PINS
#define MOTOR2_DIRPIN 16
#define MOTOR2_STEPPIN 17
#define MOTOR2_SLEEPPIN 4
#define MOTOR2_RATIO 60


// I2C PINS
#define IMU_SDA 13
#define IMU_SCL 26
#define IMU_DEFAULT_ADRESS 75u

#define IMU_MAX_ITERATION 10
#define AZIMUT_ANGLE_TOL 1.0f


void psaTest();
void imuTest();
void motorTest(uint8_t m, float command);
void stop();
void normalMode();
void showMode();

// =====================================================
#define SHOWTIME    // Ignore GPS because inside statement
#define DEBUG true  // Debug mode active
// =====================================================



void setup()
{
  // Start serial comm
  Serial.begin(115200);
  while (!Serial) delay(10); 
  if (!SPIFFS.begin()) {
    Serial.println("SPIFFS Mount Failed");
    return;
  }

  File file = SPIFFS.open("/WMM.COF", "r");
  if (!file) {
    Serial.println("Failed to open coefficient file");
    return;
  }
  file.close();

  initRotaryEncoder();
  initLcd();
}

void loop()
{
  MenuManager menumanager{};
  menumanager.initLCD();

  motorTest(1,10);
  motorTest(1,-10);
  motorTest(2,-30);
  motorTest(2,30);
  while(1)
  {
    menumanager.encoderManager();
  }
  stop();
  //showMode();
}

void showMode()
{
  // Local variables declaration =======================================
  float angleToMagNorth{0.0f};
  float angleToFlat{0.0f};

  float adjustedAzimut{0.0f};
  float adjustedElevation{0.0f};
  uint8_t it{};

  double azimutSmallestIncrement{(double)(STEPSIZE/MOTOR1_RATIO)};
  double elevationSmallestIncrement{(double)(STEPSIZE/MOTOR2_RATIO)};

  MenuManager::BingMenuOptions state{MenuManager::BingMenuOptions::MAIN};
  std::vector<std::pair<uint8_t,float>> showTime{{1,90},{2,30},{2,-5}, {2,-5}, {2,-5}, {2,-5}, {2,-5}, {2,-5}, {2,30}, {1,-5}, {2,-5}, {2,-5}, {2,-5}, {2,-5}, {2,-5}, {2,-5}};
  // ===================================================================


  // Setup--------------------------------------------------------------------------
  // Setup LCD
  MenuManager menumanager{};
  menumanager.initLCD();

  // Setup GPS
  GPS myGPS{};
  myGPS.read();

  // Set up solar angle calculator
  PSA psaObject{};
  //if(myGPS.isLatched()) { psaObject.setAll(myGPS.getDateTime(), myGPS.getLoc());}

  //Setup steppers
  Stepper stepper1{MOTOR1_DIRPIN, MOTOR1_STEPPIN, MOTOR1_SLEEPPIN, MOTOR1_RATIO};
  Stepper stepper2{MOTOR2_DIRPIN, MOTOR2_STEPPIN, MOTOR2_SLEEPPIN,MOTOR2_RATIO};

  // Setup IMU
  //IMU imu{IMU_DEFAULT_ADRESS, IMU_SCL, IMU_SDA};


  //Calibrating IMU ==============================================================
  // While we wait fo gps to latch, we will position ourself parallel to the ground
  // Move first axis until less than 1 degrees from perfect positionning

  // Read and init psa object to calculate angles
  psaObject.setAll({{2025, 4, 12}, {13, 20, 5.4}}, {45.493611, -73.5625});

  // calculate solar angles
  psaObject.updateSolarAngle();

  // The prototype is now facing magnetic north, but azimut is referenced
  // to geographic north, therefore we adjust with the magnetic declination
  adjustedAzimut = psaObject.getAzimut()+psaObject.getDeclination();
  adjustedElevation = psaObject.getAltitude();

  // Setup Current Sensor
  Ina260 sensor{};
  
  // ------------------------------------------------------------------------------

  // Main loop
  while(true)

  {
    psaObject.updateSolarAngle(myGPS.timeElapsed());
    state = menumanager.encoderManager();

    if(state == MenuManager::BingMenuOptions::START)
    {
      for(size_t i=0;i<showTime.size();i++)
      {
        if(menumanager.encoderManager()!=MenuManager::BingMenuOptions::START){break;}
        showTime[i].first==1?stepper1.angle(showTime[i].second):stepper2.angle(showTime[i].second);
      }
    }

    // Update time for angle calculations
    psaObject.updateSolarAngle(myGPS.timeElapsed());
    // Move if necessary
    if(psaObject.getAngles().first>=azimutSmallestIncrement){stepper1.angle(-psaObject.getAngles().first);}
    if(psaObject.getAngles().second>=elevationSmallestIncrement){stepper2.angle(-psaObject.getAngles().second);}

    // Read power
    sensor.readPower(100,1000);
    menumanager.updateData(psaObject.getAltitude(), psaObject.getAzimut(), sensor.getPmpp(),sensor.getTension(), sensor.getCurrent());
    delay(100);
  }
}

void psaTest()
{
  PSA psaObj;
  //psaObj.setAll({{2025, 4, 8}, {13, 20, 5.4}}, {45.494035, -73.563299});
  psaObj.setAll({{2025, 4, 8}, {13, 20, 5.4}}, {45.493863, -73.562768});
  psaObj.updateSolarAngle();
  std::pair<double, double> test = psaObj.getAngles();
  Serial.print("Azimut: "); Serial.print(test.first,3); Serial.print("  Zenith: "); Serial.println(test.second,3);
  psaObj.getDeclination(); 
}

void imuTest()
{
  IMU imu{IMU_DEFAULT_ADRESS, IMU_SCL, IMU_SDA};

  imu.setReadingMode(IMU::READING_T::GRAVITY);
  delay(1000);

  while(!imu.readData()){delay(100);}


  imu.setReadingMode(IMU::READING_T::MAG);
  while(!imu.readData()){delay(100);}
  delay(1000);

}

void motorTest(uint8_t m, float command)
{
  Stepper stepper1{MOTOR1_DIRPIN, MOTOR1_STEPPIN, MOTOR1_SLEEPPIN, MOTOR1_RATIO};
  Stepper stepper2{MOTOR2_DIRPIN, MOTOR2_STEPPIN, MOTOR2_SLEEPPIN,MOTOR2_RATIO};

  //Setup steppers
  if(m==1)
  {
    stepper1.angle(command);
  }else
  {
    stepper2.angle(command);
  }
}

void stop()
{
  while(1){delay(1000);}
}

void normalMode()
{
  // Local variables declaration =======================================
  float angleToMagNorth{0.0f};
  float angleToFlat{0.0f};

  float adjustedAzimut{0.0f};
  float adjustedElevation{0.0f};
  uint8_t it{};

  double azimutSmallestIncrement{(double)(STEPSIZE/MOTOR1_RATIO)};
  double elevationSmallestIncrement{(double)(STEPSIZE/MOTOR2_RATIO)};

  MenuManager::BingMenuOptions state{MenuManager::BingMenuOptions::MAIN};
  // ===================================================================


  // Setup--------------------------------------------------------------------------
  // Setup LCD
  MenuManager menumanager{};
  menumanager.initLCD();

  // Setup GPS
  GPS myGPS{};

  // Set up solar angle calculator
  PSA psaObject{};
  //if(myGPS.isLatched()) { psaObject.setAll(myGPS.getDateTime(), myGPS.getLoc());}

  //Setup steppers
  Stepper stepper1{MOTOR1_DIRPIN, MOTOR1_STEPPIN, MOTOR1_SLEEPPIN, MOTOR1_RATIO};
  Stepper stepper2{MOTOR2_DIRPIN, MOTOR2_STEPPIN, MOTOR2_SLEEPPIN,MOTOR2_RATIO};

  // Setup IMU
  IMU imu{IMU_DEFAULT_ADRESS, IMU_SCL, IMU_SDA};


  //Calibrating IMU ==============================================================
  // While we wait fo gps to latch, we will position ourself parallel to the ground
  // Move first axis until less than 1 degrees from perfect positionning

  // Calibration
  stepper1.angle(180);
  stepper1.angle(-180);

  // Azimut axis calibration -------------------------------------------------------
  do{
  imu.setReadingMode(IMU::READING_T::MAG);
  while(!imu.readData()){delay(100);}
  angleToMagNorth = imu.getMagneticNorthAngle();

  // move motors to obtain position parallel to ground and aligned with true north
  Serial.print("Angle to mag north: "); Serial.print(angleToMagNorth,3);
  stepper1.angle(-angleToMagNorth);
  stop();

  while(!imu.readData()){delay(100);}
  } while ((abs(imu.getMagneticNorthAngle()) > AZIMUT_ANGLE_TOL ) || DEBUG);
  // ---------------------------------------------------------------------------------

  // Elevation axis calibration -------------------------------------------------------
  do{
    imu.setReadingMode(IMU::READING_T::GRAVITY);
  while(!imu.readData()){delay(100);}
  angleToFlat += imu.getGravityAngle();

  // move motors to obtain position parallel to ground and aligned with true north
  Serial.print("Angle to flat: "); Serial.print(angleToFlat,3);
  stepper2.angle(-angleToFlat);

  imu.readData();
  } while ((abs(imu.getGravityAngle()) > AZIMUT_ANGLE_TOL) || DEBUG);
  // ---------------------------------------------------------------------------------

  // Read and init psa object to calculate angles
  #if defined(SHOWTIME)
  psaObject.setAll({{2025, 4, 12}, {13, 20, 5.4}}, {45.493611, -73.5625});
  #elif
  // Wait for GPS to latch
  Serial.println("Waiting on GPS siganl");
  while(!myGPS.isLatched()){ delay(1000);}
  Serial.println("GPS has latched to satellites");

  myGPS.read();
  psaObject.setAll(myGPS.getDateTime(), myGPS.getLoc());
  #endif

  // calculate solar angles
  psaObject.updateSolarAngle();

  // The prototype is now facing magnetic north, but azimut is referenced
  // to geographic north, therefore we adjust with the magnetic declination
  adjustedAzimut = psaObject.getAzimut()+psaObject.getDeclination();
  adjustedElevation = psaObject.getAltitude();

  // move motors to obtain position perfectly perpendicular to sun
  stepper1.angle(-adjustedAzimut);
  stepper2.angle(-adjustedElevation);

  // At the end of this, we should be facing th sun perfectly
  // =============================================================================


  // Setup Current Sensor
  Ina260 sensor{};
  
  // ------------------------------------------------------------------------------

  // Main loop
  while(true)

  {
    state = menumanager.encoderManager();
    // Update time for angle calculations
    psaObject.updateSolarAngle(myGPS.timeElapsed());
    
    // Move if necessary
    if(psaObject.getAngles().first>=azimutSmallestIncrement){stepper1.angle(-psaObject.getAngles().first);}
    if(psaObject.getAngles().second>=elevationSmallestIncrement){stepper2.angle(-psaObject.getAngles().second);}

    // Read power
    sensor.readPower(200,20000);
    delay(100);
  }
}
