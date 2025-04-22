#include "PSA.h"


/// @brief Constructor
/// @param dt Date and time
/// @param position position as latitude and longitude
PSA::PSA(DateTime dt, Position position) :
    dateTime{dt},
    pos{position},
    results{0.0, 0.0}
{
    julianDay = dtToJulianDay(dateTime);
}

/// @brief Default constructor
PSA::PSA(void) : PSA::PSA(DateTime{Date{0,0,0},Time{0,0,0.0}}, Position{0,0})
{
}

/// @brief Default destructor
PSA::~PSA()
{
}

PSA::DateTime PSA::getDateTime() const
{
    return dateTime;
}

PSA::Position PSA::getPosition() const
{
    return pos;
}

void PSA::setDateTime(DateTime dt)
{
    dateTime = dt;
}

void PSA::setPosition(Position position)
{
    pos = position;
}

void PSA::setLatitude(float latitude)
{
    pos.latitude = latitude;
}

void PSA::setLongitude(float longitude)
{
    pos.longitude = longitude;
}

void PSA::setAll(DateTime dt, Position position)
{
    setDateTime(dt);
    setPosition(position);
}

double PSA::dtToJulianDay(DateTime dt)
{
    // First section: acquisition -----------
    uint8_t D = dt.date.day;
    uint8_t M = dt.date.month;
    uint8_t Y = dt.date.year;
    // ---------------------------------------

    // Second section: Adjustment for year and months ------------
    // January and febuary are seen as the 13th and 14th month,
    // therefore we adjust the date in this case
    if (M <= 2) { Y --; M += 12; }
    // -----------------------------------------------------------

    // Third part: computing coefficients ----------------------------------------------------
    // This operation is best done as parts
    // We drop the fraction part of every multiplication and division, hence the int and floor
    int A = std::floor(dt.date.year / 100);
    int B = std::floor(A/4);
    int C = 2 - A + B;
    int E = std::floor(365.25 * (dt.date.year + 4716));
    int F = std::floor(30.6001 * (dt.date.month + 1));
    // ----------------------------------------------------------------------------------------

    // Fourth section: Julian day calculation ----------
    return C+D+E+F-1524.5;
    // -------------------------------------------------
}

void PSA::setTime(Time time)
{
    dateTime.time = time;
}

void PSA::setDate(Date date)
{
    dateTime.date = date;
}

void PSA::updateSolarAngle()
{
    // PSA+ algorithm implemented from this article:
    // https://www.sciencedirect.com/science/article/pii/S0038092X20311488#bfn1 

    // First section: julian day number ----------------------
    // julian day as a number of days n since the day 2451545
    double n = julianDay - 2451545.0;
    // -------------------------------------------------------

    // Second section: ecliptic coordinates ----------------------------------------------------------
    double omega = psaCoef[0] + psaCoef[1] * n;
    double L = psaCoef[2] + psaCoef[3] * n;
    double g = psaCoef[4] + psaCoef[5] * n;
    // Ecliptic longitude
    double lambdaE = L + psaCoef[6]*sin(g) + psaCoef[7]*sin(2*g) + psaCoef[8] + psaCoef[9]*sin(omega);
    // Obliquity of the ecliptic
    double epsilon = psaCoef[10] + psaCoef[11]*n + psaCoef[12]*cos(omega);
    // -----------------------------------------------------------------------------------------------

    // Third section celestial coordinates ------------------
    // right ascension
    double ra = atan(cos(epsilon)*sin(lambdaE)/cos(lambdaE));   
    // declination
    double delta = asin(sin(epsilon)*sin(lambdaE));
    // ------------------------------------------------------

    // Fourth section: local coordinates ------------------------------------------------------------------------------------
    double gmst = psaCoef[13] + psaCoef[14]*n + (dateTime.time.hour + dateTime.time.minutes/60 + dateTime.time.seconds/3600);
    double w = (gmst * PI/12 + pos.longitude);
    // Zenith
    double tethaZ = acos(cos(pos.latitude)*cos(delta)*cos(w) + sin(pos.latitude)*sin(delta));

    // Azimuth
    double gamma = atan(-sin(w)/(tan(delta)*cos(pos.latitude) - sin(pos.latitude)*cos(w)));

    // Parallax correction of zenith using: 
    // Earth Mean Radius = 6371.01 km
    // Astronomical Unit = 149597870.7 km 
    tethaZ = tethaZ + 6371.01 / (149597870.7 * sin(tethaZ));
    // ----------------------------------------------------------------------------------------------------------------------

    // Fifth section: conversion to degrees -----------
    results.azimuth = degrees(tethaZ);
    results.zenith = degrees(tethaZ);
    // -------------------------------------------------
}

void PSA::updateSolarAngle(Time time)
{
    setTime(time);
    updateSolarAngle();
}
