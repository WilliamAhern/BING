#include <Wire.h>
#include <SparkFun_BNO080_Arduino_Library.h>
#include <Adafruit_BNO08x.h>

class IMU
{
public:
  enum READING_T{
    NONE,
    MAG,
    GRAVITY,
    BOTH
  };

    IMU(uint8_t adr=0x4Bu, uint8_t sclp = 0x16u, uint8_t sdap = 0x15u);
    ~IMU();

    bool readData();

    float getMagneticNorthAngle() const;
    float getGravityAngle() const;

    void setReadingMode(READING_T r);

private:
  BNO080 bno080;
  uint8_t adress;
  uint8_t sclPin;
  uint8_t sdaPin;

  READING_T currentReading;

  float magneticNorthAngle;
  float gravityAngle;
};