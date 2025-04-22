#include "IMU.h"
#define DELAYM 100
#define DELAYG 100

IMU::IMU(uint8_t adr, uint8_t sclp, uint8_t sdap):
    bno080(),
    adress{adr},
    sclPin{sclp},
    sdaPin{sdap},
    magneticNorthAngle{0.0f},
    gravityAngle{0.0f},
    currentReading{READING_T::NONE}
{
  //  //=================================
  //  delay(100); //  Wait for BNO to boot
  //  // Start i2c and BNO080
  //  Wire.flush();   // Reset I2C
  //  IMU.begin(BNO080_DEFAULT_ADDRESS, Wire);
  //  Wire.begin(SDA_PIN, SCL_PIN);
  //  Wire.setClockStretchLimit(4000);
  //  //=================================

    Serial.println("Adafruit bno080 init");
    //Wire1.begin(sdaPin,sclPin);
    Wire.begin();
    //Wire1.setClock(400000); //Increase I2C data rate to 400kHz
    Wire.setClock(400000); //Increase I2C data rate to 400kHz
    if(!bno080.begin()){
      Serial.println("Failed to find bno080 chip");
      while (1) { delay(10); }
    }

    bno080.enableMagnetometer(DELAYM);
    //bno080.enableGravity(DELAYG);

    Serial.println(F("Gyro integrated rotation vector enabled"));
    Serial.println(F("Output in form i, j, k, real, gyroX, gyroY, gyroZ"));
    Serial.println(F("Output in form x, y, z, in uTesla"));
}

IMU::~IMU()
{
}

bool IMU::readData()
{
  if(!bno080.dataAvailable()){Serial.print("No data available...");Serial.println(bno080.getMagAccuracy());}
  switch (currentReading)
  {
  case MAG:
  {
    // Will average over a period of time for precision
    size_t it{0};
    if(bno080.dataAvailable())
    {
      magneticNorthAngle = 0;
      for (size_t i = 0; i < 10; i++)
      {
        float mX = bno080.getMagX();
        float mY = bno080.getMagY();
        float mZ = bno080.getMagZ();

        if(bno080.getMagAccuracy()>1)
        {
          magneticNorthAngle += degrees(atan(mY/mX));
          it++;
        }
        Serial.print(degrees(atan(mY/mX)));
        Serial.print(F(","));
        Serial.print(bno080.getMagAccuracy());

        delay(DELAYM);
      }
      magneticNorthAngle/=it;
      if(bno080.getMagAccuracy()>1){return true;}
    }
    break;
  }
  case GRAVITY:
  {
    // Will average over a period of time for precision
    size_t it{0};
    if(bno080.dataAvailable())
    {
      gravityAngle = 0;
      for (size_t i = 0; i < 10; i++)
      {
        float gX = bno080.getGravityX();
        float gY = bno080.getGravityY();
        float gZ = bno080.getGravityZ();

        if(bno080.getGravityAccuracy()>1)
        {
          gravityAngle += degrees(atan(gZ/gX));
          it++;
        }

        Serial.print(degrees(atan(gX/gZ)));
        Serial.print(F(","));
        Serial.print(bno080.getGravityAccuracy());

        delay(DELAYG);
      }
      gravityAngle/=it;
      if(bno080.getGravityAccuracy()>1){return true;}
    }
    break;
  }
  default:
  {
    return false;
  }
  }
  return false;
}

float IMU::getMagneticNorthAngle() const
{
    return {magneticNorthAngle};
}

float IMU::getGravityAngle() const
{
    return {gravityAngle};
}

void IMU::setReadingMode(READING_T r)
{
  bno080.softReset();
  currentReading = r;
  switch(currentReading)
  {
    case MAG:
      bno080.enableMagnetometer(DELAYM);
      break;
    case GRAVITY:
      bno080.enableGravity(DELAYG);
      break;
    case BOTH:
      bno080.enableMagnetometer(DELAYM);
      bno080.enableGravity(DELAYG);
      break;
    default:
      break;
  }
}
