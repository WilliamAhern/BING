#include "PSA.h"

int readMagneticModel(MAGtype_MagneticModel *MagneticModel);


/// @brief Constructor
/// @param dt Date and time
/// @param position position as latitude and longitude
PSA::PSA(DateTime dt, Position position) :
    dateTime{dt},
    pos{position},
    results{0.0, 0.0}
{
    julianDay = dtToJulianDay(dateTime).first;
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

double PSA::getAzimut() const
{
    return results.azimuth;
}

double PSA::getAltitude() const
{
    return results.zenith;
}

/// @brief Returns angles between position and sun
/// @return Azimuth, Altitude
std::pair<double, double> PSA::getAngles() const
{
    return {getAzimut(), getAltitude()};
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

/// @brief Converts a date time struct data into a julian day
/// @param dt has to be in UT1
/// @return julian day, decimal hours
std::pair<double, double> PSA::dtToJulianDay(DateTime dt)
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
    
     // Fourth part: decimal hours ----------------------------------------------------
    double decimalHours{(double)dateTime.time.hour + (double)dateTime.time.minutes/60.0 + (double)dateTime.time.seconds/3600.0};
    // ----------------------------------------------------------------------------------------

    // Fourth section: Julian day calculation ----------
    return {C+D+E+F-1524.5+(decimalHours/24), decimalHours};
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
    // update julian day
    std::pair<double,double> jd{dtToJulianDay(dateTime)};
    julianDay=jd.first;
    Serial.print("julian date: ");   Serial.println(julianDay);
    Serial.print("Decimal hours: ");   Serial.println(jd.second);

    // julian day as a number of days n since the day 2451545
    double n = julianDay - 2451545.0;
    Serial.print("n: ");   Serial.println(n);
    // -------------------------------------------------------

    // Second section: ecliptic coordinates ----------------------------------------------------------
    double omega = psaCoef[0] + psaCoef[1] * n;
    Serial.print("Omega: ");   Serial.println(omega);
    double L = psaCoef[2] + psaCoef[3] * n;
    Serial.print("L: ");   Serial.println(L);
    double g = psaCoef[4] + psaCoef[5] * n;
    Serial.print("g: ");   Serial.println(g);
    // Ecliptic longitude
    double lambdaE = L + psaCoef[6]*sin(g) + psaCoef[7]*sin(2*g) + psaCoef[8] + psaCoef[9]*sin(omega);
    Serial.print("Ecliptic longitude: ");   Serial.println(lambdaE);
    // Obliquity of the ecliptic
    double epsilon = psaCoef[10] + psaCoef[11]*n + psaCoef[12]*cos(omega);
    Serial.print("obliuity of the eclipse: ");   Serial.println(epsilon);
    // -----------------------------------------------------------------------------------------------

    // Third section celestial coordinates ------------------
    // right ascension
    double ra = atan(cos(epsilon)*sin(lambdaE)/cos(lambdaE));
    Serial.print("Right ascension: ");   Serial.println(ra); 
    // make sure interval is 0 <= gamma <= 2pi
    while(ra<0.0){ ra+=2*PI;}
    Serial.print("Right ascension: ");   Serial.println(ra); 
      
    // declination
    double delta = asin(sin(epsilon)*sin(lambdaE));
    Serial.print("Right declination: ");   Serial.println(delta);
    // ------------------------------------------------------

    // Fourth section: local coordinates ------------------------------------------------------------------------------------
    double gmst = psaCoef[13] + psaCoef[14]*n + jd.second;
    Serial.print("gmst: ");   Serial.println(gmst);
    double w = (gmst * PI/12 + (pos.longitude*PI/180.0));
    Serial.print("w: ");   Serial.println(w);
    double HourAngle = w - ra;
    Serial.print("HourAngle: ");   Serial.println(HourAngle);

    // Zenith
    double tethaZ = acos(cos(pos.latitude*PI/180.0)*cos(delta)*cos(HourAngle) + sin(pos.latitude*PI/180.0)*sin(delta));
    Serial.print("zenith avant correction: ");   Serial.println(tethaZ);

    // Azimuth
    double gamma = atan2(-sin(HourAngle),tan(delta)*cos(pos.latitude*PI/180.0) - sin(pos.latitude*PI/180.0)*cos(HourAngle));
     Serial.print("azimut: ");   Serial.println(gamma);
    // make sure interval is 0 <= gamma <= 2pi
    while(gamma<0.0){gamma+=2*PI;}
    Serial.print("azimut: ");   Serial.println(gamma);

    // Parallax correction of zenith using: 
    // Earth Mean Radius = 6371.01 km
    // Astronomical Unit = 149597870.7 km 
    tethaZ = tethaZ + 6371.01 / (149597870.7 * sin(tethaZ));
    Serial.print("zenith apres correction: ");   Serial.println(tethaZ);
    // ----------------------------------------------------------------------------------------------------------------------

    // Fifth section: conversion to degrees -----------
    results.azimuth = degrees(gamma);
    Serial.print("azimut deg: ");   Serial.println(results.azimuth);
    results.zenith = degrees(tethaZ);
    Serial.print("zenith deg: ");   Serial.println(results.zenith);
    // -------------------------------------------------
}

void PSA::updateSolarAngle(Time time)
{
    setTime(time);
    updateSolarAngle();
}

void PSA::updateSolarAngle(float timeElapsed)
{
    dateTime.time.seconds+= (double)timeElapsed;
    updateSolarAngle();
}

double PSA::getDeclination() const
{
    const int nMax = 12;  // Adjust to your WMM.COF's value (e.g., 12 or 13 or 90)
    const int coeff_size = (nMax + 1) * (nMax + 2) / 2;

    MAGtype_MagneticModel *magneticModel= new MAGtype_MagneticModel{};
    MAGtype_MagneticModel *TimedMagneticModel= nullptr;
    MAGtype_Ellipsoid Ellip;
    MAGtype_Geoid Geoid;

    MAG_SetDefaults(&Ellip, &Geoid);

    // Load the WMM model file (WMM2025.COF should be in SPIFFS or LittleFS or statically linked — see note below)
    // Replace with your actual path/method if not using filesystem
    // Call your function here:
    magneticModel->Main_Field_Coeff_G = new double[coeff_size];
    magneticModel->Main_Field_Coeff_H = new double[coeff_size];
    magneticModel->Secular_Var_Coeff_G = new double[coeff_size];
    magneticModel->Secular_Var_Coeff_H = new double[coeff_size];
    magneticModel->nMax=nMax;
    magneticModel->nMaxSecVar=coeff_size;

    if (!readMagneticModel(magneticModel)) {
        
        return 999.0;
    }

       // Date struct
    MAGtype_Date date = {
        .Year = dateTime.date.year,
        .Month = dateTime.date.month,
        .Day = dateTime.date.day,
        .DecimalYear = decimalYear()
    };

    // Apply time modification
    TimedMagneticModel = MAG_AllocateModelMemory(coeff_size);

    // Set location: latitude, longitude, altitude
    MAGtype_CoordGeodetic userCoord = {
        .lambda = pos.longitude, // longitude
        .phi = pos.latitude,     // latitude
        .HeightAboveEllipsoid = 0.0,
        .HeightAboveGeoid = 0.0,
        .UseGeoid = 0
    };

    // Compute spherical coordinates
    MAGtype_CoordSpherical sphericalCoord;
    MAG_GeodeticToSpherical(Ellip, userCoord, &sphericalCoord);

    // Output magnetic elements and errors
    MAGtype_GeoMagneticElements elements, errors;

    // Perform the calculation
    point_calc(Ellip, userCoord, &sphericalCoord, date, magneticModel, TimedMagneticModel, &elements, &errors);

    Serial.println("Test statement ");

    // Print results
    Serial.print("Magnetic Declination: ");
    Serial.print(elements.Decl, 4);
    Serial.println(" degrees");

    // Cleanup
    MAG_FreeMagneticModelMemory(magneticModel);
    MAG_FreeMagneticModelMemory(TimedMagneticModel);

    return elements.Decl;
}

double PSA::decimalHours() const
{
    return {(double)dateTime.time.hour + (double)dateTime.time.minutes/60.0 + (double)dateTime.time.seconds/3600.0};
}

double PSA::decimalYear() const
{
    return dateTime.date.year + (isLeapYear(dateTime.date.year) ?  (CUMULDAYS[dateTime.date.month]+dateTime.date.day+(int)(dateTime.date.month>2))/366 : (CUMULDAYS[dateTime.date.month]+dateTime.date.day)/365);
}

bool PSA::isLeapYear(int year) const
{
    return (year % 4 == 0) && ((year % 100 != 0) || (year % 400 == 0));
}

int readMagneticModel(MAGtype_MagneticModel *MagneticModel)
{
    File file = SPIFFS.open("/WMM.COF", "r");
    if (!file) {
        Serial.println("Failed to open coefficient file");
        return false;
    }
    const int date_size = 20;
    char *edit_date = (char *)malloc(sizeof(char) * date_size);

    MagneticModel->Main_Field_Coeff_H[0] = 0.0;
    MagneticModel->Main_Field_Coeff_G[0] = 0.0;
    MagneticModel->Secular_Var_Coeff_H[0] = 0.0;
    MagneticModel->Secular_Var_Coeff_G[0] = 0.0;

    String line = file.readStringUntil('\n');
    double epoch;
    sscanf(line.c_str(), "%lf %s %s", &epoch, MagneticModel->ModelName, edit_date);

    MagneticModel->min_year = MAG_dtstr_to_dyear(edit_date);
    if (MagneticModel->min_year == -1)
    {
        MagneticModel->min_year = epoch;
    }
    MagneticModel->epoch = epoch;

    while (file.available())
    {
        line = file.readStringUntil('\n');
        if (line.startsWith("9999"))
        {
            break;
        }

        int n, m;
        double gnm, hnm, dgnm, dhnm;
        sscanf(line.c_str(), "%d%d%lf%lf%lf%lf", &n, &m, &gnm, &hnm, &dgnm, &dhnm);
        if (m <= n)
        {
            int index = (n * (n + 1) / 2 + m);
            MagneticModel->Main_Field_Coeff_G[index] = gnm;
            MagneticModel->Secular_Var_Coeff_G[index] = dgnm;
            MagneticModel->Main_Field_Coeff_H[index] = hnm;
            MagneticModel->Secular_Var_Coeff_H[index] = dhnm;
        }
    }

    free(edit_date);
    file.close();
    return true;
}