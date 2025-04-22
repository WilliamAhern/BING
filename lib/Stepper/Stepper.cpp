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
    Serial.println("init complete");
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
    delay(10);
}

void Stepper::wake()
{
    digitalWrite(sleepPin, LOW);
    delay(10);
}

bool Stepper::isSleep()
{
    return digitalRead(sleepPin);
}

void Stepper::step(int steps, int speed)
{
    int polarizedSpeed = steps<0?-abs(speed):speed;
    int polarizedSteps = limitSteps(speed<0?-abs(steps):steps);
    Serial.print("Speed, Steps: "); Serial.print(polarizedSpeed); Serial.print(",  "); Serial.println(polarizedSteps);

    if(isSleep()) { wake();}

    Serial.print("Setting to zero motor, current position: ");
    Serial.println(stepper.currentPosition());
    
    stepper.setCurrentPosition(0);
    
    while(abs(stepper.currentPosition())<abs(polarizedSteps))
    {
        stepper.setSpeed(polarizedSpeed);
        stepper.moveTo(polarizedSteps);
        stepper.run();
    }
    Serial.print("Done, current position: ");
    Serial.println(stepper.currentPosition());
    delay(10);
    sleep();
}

void Stepper::step(int steps)
{
    step(steps, MAXSPEED);
}

void Stepper::angle(float ang, int speed)
{
    step(ang*gearRatio/STEPSIZE,speed);
}

void Stepper::angle(float ang)
{
    angle(ang, MAXSPEED);
}

int Stepper::limitSteps(int steps)
{
    int max{(int)floor(360*gearRatio/STEPSIZE)};
    return (steps<(-max) || steps>max ? 0 : steps);
}