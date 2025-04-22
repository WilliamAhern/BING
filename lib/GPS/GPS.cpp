#include "GPS.h"

// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to debug and listen to the raw GPS sentences
#define GPSECHO false

/// @brief Constructor
/// @param gpsUart Reference to hardware serial used (Need to set pins before)
GPS::GPS(HardwareSerial& gpsUart)  :
    myGPS(&gpsUart)
{
    // 9600 baud is the default rate for the Ultimate GPS
    myGPS.begin(9600);

    myGPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
    // uncomment this line to turn on only the "minimum recommended" data
    // GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCONLY);
    // For parsing data, we don't suggest using anything but either RMC only or RMC+GGA since
    // the parser doesn't care about other sentences at this time
    // Set the update rate
    myGPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ); // 1 Hz update rate
    // For the parsing code to work nicely and have time to sort thru the data, and
    // print it out we don't suggest using anything higher than 1 Hz

    // Request updates on antenna status, comment out to keep quiet
    myGPS.sendCommand(PGCMD_ANTENNA);

    delay(1000);
}

/// @brief Destructor
GPS::~GPS()
{
    myGPS.~Adafruit_GPS();
}

bool GPS::isLatched()
{
    return (myGPS.read());
}

/// @brief Reads, parses and updates GPS data, NEEDS TO BE CALLED BEFORE GETTING INFOS
/// @return True if reading operation was successfull
bool GPS::read()
{
    // Manque ptetre logique de multiple read
    // read data from the GPS
    char c = myGPS.read();

    // Debug statement to print out what was received by the gps
    if (GPSECHO){ if (c) { Serial.print(c); }; }

    // if a sentence is received, we can check the checksum, parse it...
    if (myGPS.newNMEAreceived()) 
    {
        Serial.print(myGPS.lastNMEA());
        // Check if able to parse
        return (!myGPS.parse(myGPS.lastNMEA()) ? false : true);
    }
    else { return false;}
}

float GPS::timeElapsed()
{
    return myGPS.secondsSinceTime();
}

/// @brief Gets date and time from satellite
/// @return UTC date and time
PSA::DateTime GPS::getDateTime() const
{
    return {{myGPS.year, myGPS.month, myGPS.day},{myGPS.hour, myGPS.minute, static_cast<double>(myGPS.seconds) + (static_cast<double>(myGPS.milliseconds)/1000.0)}};
}

/// @brief Gets time from satellite
/// @return UTC time 
PSA::Time GPS::getTime() const
{
    return {myGPS.hour, myGPS.minute, static_cast<double>(myGPS.seconds) + (static_cast<double>(myGPS.milliseconds)/1000.0)};
}

/// @brief Gets latitude from satellite
/// @return Latitude in degrees
float GPS::getLatitude() const
{
    return static_cast<double>(myGPS.latitudeDegrees);
}

/// @brief Gets longitude from satellite
/// @return Longitude in degrees
float GPS::getLongitude() const
{
    return static_cast<double>(myGPS.longitudeDegrees);
}

/// @brief Gets latitude and longitude from stallite
/// @return Latitude, Longitude in degrees
PSA::Position GPS::getLoc() const
{
    return {getLatitude(), getLongitude()};
}
