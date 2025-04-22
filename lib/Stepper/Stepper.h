#include <AccelStepper.h>

#define motorInterfaceType 1
#define MAXSPEED 1500
#define ACCEL 300
#define STEPSIZE 1.8

class Stepper
{
public:
    Stepper(uint8_t dPin, uint8_t stPin, uint8_t slPin, double ratio);
    ~Stepper();

    void stepperSetup();

    uint8_t getDirPin() const;
    uint8_t getStepPin() const;
    AccelStepper& accelStepper();

    void step(int steps, int speed);

    void step(int steps);

    void angle(float angle, int speed);

    void angle(float angle);

private:
    AccelStepper stepper;

    uint8_t dirPin;
    uint8_t stepPin;
    uint8_t sleepPin;

    double gearRatio;

    
    void sleep();
    void wake();
    bool isSleep();
};
