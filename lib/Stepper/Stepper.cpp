#include "Stepper.h"

Stepper::Stepper(uint8_t dPin, uint8_t stPin, uint8_t slPin, double ratio) :
    stepper(motorInterfaceType, stPin, dPin),
    dirPin{dPin},
    stepPin{stPin},
    sleepPin{slPin},
    gearRatio{ratio}
{
    stepperSetup();
    sleep();
}

Stepper::~Stepper()
{
    stepper.~AccelStepper();
}

void Stepper::stepperSetup()
{
    // Setup pind =========================
    pinMode(dirPin, OUTPUT);
    pinMode(stepPin, OUTPUT);
    pinMode(sleepPin, OUTPUT);
    // ===================================

    // Setup Stepper ========================
    stepper.setMaxSpeed(MAXSPEED);
    stepper.setAcceleration(ACCEL);
}

uint8_t Stepper::getDirPin() const
{
    return dirPin;
}

uint8_t Stepper::getStepPin() const
{
    return stepPin;
}

AccelStepper &Stepper::accelStepper()
{
    return stepper;
}

void Stepper::sleep()
{
    digitalWrite(sleepPin, HIGH);
}

void Stepper::wake()
{
    digitalWrite(sleepPin, LOW);
}

bool Stepper::isSleep()
{
    return digitalRead(sleepPin);
}

void Stepper::step(int steps, int speed)
{
    if(isSleep()) { wake();}

    stepper.setCurrentPosition(0);
    stepper.setSpeed(speed);
    stepper.moveTo(steps);
    stepper.run();

    while(stepper.run())
    {
        delay(1);
    }
    
    sleep();
}

void Stepper::step(int steps)
{
    step(steps, MAXSPEED);
}

void Stepper::angle(float ang, int speed)
{
    step((ang*gearRatio/STEPSIZE),speed);
}

void Stepper::angle(float ang)
{
    angle(ang, MAXSPEED);
}