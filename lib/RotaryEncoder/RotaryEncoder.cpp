#include "RotaryEncoder.h"

AiEsp32RotaryEncoder rotaryEncoder(ROTARY_ENCODER_A_PIN, ROTARY_ENCODER_B_PIN, ROTARY_ENCODER_BUTTON_PIN, -1, ROTARY_ENCODER_STEPS, false);


void IRAM_ATTR readEncoderISR() {
    rotaryEncoder.readEncoder_ISR();
}
void initRotaryEncoder() {
    rotaryEncoder.begin();
    rotaryEncoder.setup(readEncoderISR);
    rotaryEncoder.setBoundaries(0, 1, true);  
    rotaryEncoder.setAcceleration(100); 
}


