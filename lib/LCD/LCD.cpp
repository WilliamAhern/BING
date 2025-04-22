#include "LCD.h"

int ligne = 4;
int colonne = 20;
LiquidCrystal_PCF8574 lcd(0x27);

void initLcd() {
    Wire1.begin(SDA_PIN, SCL_PIN);
    delay(500);
    lcd.begin(colonne, ligne, Wire1);           //Format du LCD
    lcd.setBacklight(HIGH);
    lcd.clear();
    lcd.setCursor(0, 0); 
}

