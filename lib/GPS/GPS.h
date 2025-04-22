#include "PSA.h"
#include <HardwareSerial.h>
#include <Adafruit_GPS.h>

// GPS class is a wrapper fort the Adafruit_GPS class, must create a HardareSerial 
// instance and set pins before instanciating this class
class GPS
{
public:
    GPS(HardwareSerial& gpsUart);       // Constructor
    ~GPS();                             // Destructor

    bool isLatched();                   // Check if connected to a satellite

    bool read();                        // Function to read and parse GPS infos, call every 2 seconds max

    float timeElapsed();       // Time elapsed since time was obtained from satellite
    PSA::DateTime getDateTime() const;  //UTC
    PSA::Time getTime() const;          //UTC
    
    float getLatitude() const;          // In degrees
    float getLongitude() const;         // in degrees
    PSA::Position getLoc() const;       // In degrees

private:
    Adafruit_GPS myGPS;                 // GPS object that contains all the info from last readout
};