#include "Ina260.h"

Ina260::Ina260(uint8_t adr, uint8_t sclp, uint8_t sdap):
    ina260(),
    adress{adr},
    sclPin{sclp},
    sdaPin(sdap),
    data(),
    Pmpp{0.0f},
    tension{0.0f},
    current{0.0f}
{
    Serial.println("Adafruit INA260 Init");

    if (!ina260.begin(adress)) {
    Serial.println("Couldn't find INA260 chip");
    while (1);
    }
     // set the number of samples to average
    ina260.setAveragingCount(INA260_COUNT_1);
    // set the time over which to measure the current and bus voltage (DEfault 1.1ms)
    ina260.setVoltageConversionTime(INA260_TIME_140_us);
    ina260.setCurrentConversionTime(INA260_TIME_140_us);

    Serial.println("INA260 init done");

}

Ina260::~Ina260()
{
    data.clear();
    data.~vector();
}

void Ina260::readPower(uint intervalMillis, uint timeOutMillis)
{
    // Preallocation to save time
    data.clear();
    data.resize((timeOutMillis/intervalMillis)+1u);

    Pmpp = 0;
    float v{0.0f}, a{0.0f}, p{0.0f};
    uint32_t timer{millis()};

    while(timer < timeOutMillis)
    {
        v = min(v, ina260.readBusVoltage());
        a = max(a, ina260.readCurrent());
        p = max(p, ina260.readPower());
        if((millis() - timer) < intervalMillis)
        {
            data.emplace_back(Data{(millis()-timer),v,a,p});
            Pmpp = max(p,Pmpp);
            tension = min(v,tension);
            current = max(a,current);
            v = a = p = 0.0f;
            timeOutMillis -= intervalMillis;
            timer = millis();
        }
    }
}

float Ina260::getPmpp()
{
    return Pmpp;
}

float Ina260::getCurrent()
{
    return current;
}

float Ina260::getTension()
{
    return tension;
}

std::vector<Data> &Ina260::getDataRef()
{
    return data;
}
