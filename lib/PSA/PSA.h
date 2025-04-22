#include <AccelStepper.h>

class GPS;  // Forward declaration

class PSA
{
public:
/// @brief IN UT1
typedef struct
{
    int year;
    uint8_t month;
    uint8_t day;
}   Date;

/// @brief In time zone UT1
typedef struct
{
    uint8_t hour;
    uint8_t minutes;
    double seconds;
}   Time;

typedef struct
{
    Date date;
    Time time;  // In time zone UT1
}   DateTime;

typedef struct
{
    float latitude;     // In degrees
    float longitude;    // In degrees
}   Position;

typedef struct
{
    double azimuth; // In degrees
    double zenith;  // In degrees
}   SolarAngle;

// Constructors and destructors
PSA(DateTime dt, Position pos);
PSA(void);
~PSA();

// Getters
DateTime getDateTime() const;
Position getPosition() const;

void setDateTime(DateTime dt);
void setPosition(Position position);
void setLatitude(float latitude);
void setLongitude(float longitude);
void setAll(DateTime dt, Position position);

void updateSolarAngle();
void updateSolarAngle(Time time);

// Add update solar angle from gps object

private:
const double psaCoef[15] = {2.267127827,
                            -0.0009300339267,
                            4.895036035,
                            0.017202796,
                            6.239468336,
                            0.01720200135,
                            0.03338320972,
                            0.0003497596876,
                            -0.0001544353226,
                            -0.000008689729360,
                            0.4090904909,
                            -0.000000006213605399,
                            0.00004418094944,
                            6.697096103,
                            0.06570984737};
DateTime dateTime;
double julianDay;
Position pos;
SolarAngle results;

double dtToJulianDay(DateTime dt);

void setTime(Time time);
void setDate(Date date);
};