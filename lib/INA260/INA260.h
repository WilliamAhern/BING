#include <Adafruit_INA260.h>
#include <SPI.h>
#include <vector>
//#include "SDCard.h"

typedef struct
{
    uint32_t millisecondes;
    float voltage;
    float current;
    float power;
}   Data;

class Ina260
{
public:
    Ina260(uint8_t adress = 0x40u, uint8_t sclp = 0x16u, uint8_t sdap = 0x15u);
    ~Ina260();

    void readPower(uint intervalMillis, uint timeOut);
    float getPmpp();
    float getCurrent();
    float getTension();

    //Data is ordered as V, I, P
    std::vector<Data>& getDataRef();
private:
    Adafruit_INA260 ina260;
    uint8_t adress;
    uint8_t sclPin;
    uint8_t sdaPin;

    std::vector<Data> data;
    float Pmpp;
    float tension;
    float current;
};