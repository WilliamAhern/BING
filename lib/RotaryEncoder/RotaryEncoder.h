#ifndef ROTARY_ENCODER_H
#define ROTARY_ENCODER_H

#include "AiEsp32RotaryEncoder.h"

//Définit les GPIO du ESP32 pour l'encodeur
#define ROTARY_ENCODER_A_PIN 25
#define ROTARY_ENCODER_B_PIN 33
#define ROTARY_ENCODER_BUTTON_PIN 32
#define ROTARY_ENCODER_STEPS 4
#define LONG_PRESS_DURATION 800

// Initialisation de l'encodeur
extern AiEsp32RotaryEncoder rotaryEncoder;

//Routine d'interruption de service pour lire un changement dans l'encodeur en temps réel
//sans interuptions des autres fonctions
void IRAM_ATTR readEncoderISR();

//Initialise le Rotary Encoder
void initRotaryEncoder();


#endif