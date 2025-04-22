#ifndef LCD_MANAGER_H
#define LCD_MANAGER_H

#include <LiquidCrystal_PCF8574.h>

// LCD Screen Pins
#define SDA_PIN 14
#define SCL_PIN 27

//Definit resolution LCD
extern int colonne;
extern int ligne;

//Adresse du LCD
extern LiquidCrystal_PCF8574 lcd; //Adresse du LCD = 0x27

//Fonction d'initialisation du LCD
void initLcd();

#endif