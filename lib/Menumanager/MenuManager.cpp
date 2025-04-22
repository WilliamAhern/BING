#include "MenuManager.h"
#include "LCD.h"
#include "RotaryEncoder.h"
#include <vector> // Ensure vector is included

const char* MenuManager::defaultMenuOptions[] = {"Choisir une option"};
const char* MenuManager::bingMenuOptions[] = {"Run", "Afficher Donnees"}; // Juste pour la démo
const char* MenuManager::showDataMenuOptions[] = {"Hauteur","Azimut","PMPP", "Volt","Amperage"};
const char* MenuManager::startTestMenuOptions[] = {"Complete", "Hauteur","Azimut"};
/*
const char* MenuManager::mainMenuOptions[] = {"Mode Manuel", "Mode Auto"};
const char* MenuManager::manualMenuOptions[] = {"Ajuster Hauteur", "Ajuster Azimut", "Centrer", "Afficher Donnees", "Retour"};
const char* MenuManager::autoMenuOptions[] = {"Configuration", "Commencer Test", "Retour"};
const char* MenuManager::adjustHeightMenuOptions[] = {"Haut. cible", "Haut. reel", "Retour"};
const char* MenuManager::adjustAzimutMenuOptions[] = {"Azi. cible", "Azi. reel", "Retour"};
const char* MenuManager::centerMenuOptions[] = {"Centrage en cours..", "Retour"};
const char* MenuManager::configMenuOptions[] = {"Haut. Min", "Haut. Max","Azi. Min", "Azi. Max", "Retour"};
const char* MenuManager::startTestMenuOptions[] = {"Azimut", "Hauteur","PMPP","Next", "Retour"};
*/

const int MenuManager::showDataMenuSize = (sizeof(showDataMenuOptions) / sizeof(showDataMenuOptions[0]));
const int MenuManager::bingMenuSize = (sizeof(bingMenuOptions) / sizeof(bingMenuOptions[0]));
const int MenuManager::startTestMenuSize = (sizeof(startTestMenuOptions) / sizeof(startTestMenuOptions[0]));
/*
const int MenuManager::mainMenuSize = (sizeof(mainMenuOptions) / sizeof(mainMenuOptions[0]));
const int MenuManager::manualMenuSize = (sizeof(manualMenuOptions) / sizeof(manualMenuOptions[0]));
const int MenuManager::autoMenuSize = (sizeof(autoMenuOptions) / sizeof(autoMenuOptions[0]));
const int MenuManager::adjustHeightMenuSize = sizeof(adjustHeightMenuOptions) / sizeof(adjustHeightMenuOptions[0]);
const int MenuManager::adjustAzimutMenuSize = sizeof(adjustAzimutMenuOptions) / sizeof(adjustAzimutMenuOptions[0]);
const int MenuManager::centerMenuSize = (sizeof(centerMenuOptions) / sizeof(centerMenuOptions[0]));
const int MenuManager::configMenuSize = (sizeof(configMenuOptions) / sizeof(configMenuOptions[0]));
*/
MenuManager::MenuManager(): 
/*mainMenuIndex(0), manualMenuIndex(0), autoMenuIndex(0), 
      adjustHeightMenuIndex(0), adjustAzimutMenuIndex(0), 
      centerMenuIndex(0), showDataMenuIndex(0), 
      configMenuIndex(0), startTestMenuIndex(0),*/ 
      bingMenuIndex(0), variableTest{0}
{

//Initialisation des valeurs
float posCibleHauteur = 50.0;
float posCibleAzimut = 95.0;
float increment = 5.0;
float hauteurMin = 0.0;
float hauteurMax = 0.0;
float azimutMin = 0.0;
float azimutMax = 0.0;

//Initialisation des variables
variableTest[0] = &posCibleHauteur;
variableTest[1] = &posReelleHauteur;
variableTest[2] = &posCibleAzimut;
variableTest[3] = &posReelleAzimut;
variableTest[4] = &PMPP;
variableTest[5] = &tension;
variableTest[6] = &amperage;
variableTest[7] = &increment;
variableTest[8] = &hauteurMin;
variableTest[9] = &hauteurMax;
variableTest[10] = &azimutMin;
variableTest[11] = &azimutMax;
variableTest[12] = 0;
variableTest[13] = 0;
variableTest[14] = 0;
}

MenuManager::~MenuManager(){
}
// Affichage du menu avec un ajustement de valeur
//void MenuManager::displayMenu(const char* titre, float* variableTest[], int rangeMin, int rangeMax, int prevMenuIndex){

/********************************************************
Fonction qui ajuste la valeur d'une variable avec l'encodeur
    - Prend en paramètre le titre, la variable à ajuster, le pas, la valeur minimale et maximale
    - Retourne la nouvelle valeur de la variable
    - Affiche la valeur sur l'écran LCD
    - Gère le clic de l'encodeur pour sortir de la fonction
***********************************************************/
/*float MenuManager::adjustValueWithEncoder(const char* label, float variableTest, float step, float minVal, float maxVal) {
    /*bool activeAdjustment = true;
    float correction = 0;
    // Affichage du titre centré
    centerTitle(titre);

    // Définir les limites de l'encodeur rotatif
    rotaryEncoder.setBoundaries(rangeMin, rangeMax, false);

    // Convertir la valeur de variableTest en long pour setEncoderValue
    long encoderValue = static_cast<long>(*variableTest[0]);
    // Définit la valeur initiale de l'encodeur rotatif avec la valeur de variableTest
    rotaryEncoder.setEncoderValue(encoderValue);

    //while (activeAdjustment) {
        // Affichage du titre centré
        lcd.setCursor(0, 1);
        lcd.print("Val.: ");

        // Lire la valeur de l'encodeur et mettre à jour la correction
        correction = rotaryEncoder.readEncoder();
        correction = constrain(correction, -1, 1);
        //rotaryEncoder.reset();// Réinitialiser la valeur de l'encodeur pour éviter les changements rapides

        // Mettre à jour la valeur de variableTest avec la correction
        *variableTest[0] += correction;

        // Effacer la valeur précédente
        lcd.setCursor(6, 1);
        lcd.print("    ");

        // Afficher la nouvelle valeur
        lcd.setCursor(6, 1);
        lcd.print(*variableTest[0]);

        // Vérifier si le bouton de l'encodeur a été cliqué pour sortir de la boucle
        //if (rotaryEncoder.isEncoderButtonClicked()) {
            //activeAdjustment = false;
            // Réinitialiser les limites de l'encodeur pour le menu précédent
    rotaryEncoder.setBoundaries(0, prevMenuIndex, false);
        //}
   // }
    float value = variableTest;  // Utiliser la première variable pour l'ajustement
    bool adjusting = true;

    // Affiche le titre
    lcd.clear();
    centerTitle(label);
    lcd.setCursor(0, 1);
    lcd.print("Val.: ");  
    lcd.setCursor(6, 1);
    lcd.print(value,1);

    // Initialiser l’encodeur rotatif   
    rotaryEncoder.reset();                                      // Réinitialiser la valeur de l'encodeur
    rotaryEncoder.setBoundaries(minVal, maxVal, false);         // Réinitialiser les limites de l'encodeur
    rotaryEncoder.setEncoderValue(value);                       // Définit la valeur initiale de l'encodeur rotatif avec la valeur de variableTest
    long lastPos = value;;

    while (adjusting) {
        
        long newPos = rotaryEncoder.readEncoder();
        long delta = newPos - lastPos;

        if (delta != 0) {
            value += delta * step;
            value = constrain(value, minVal, maxVal);
            lastPos = newPos;
        
         // Affichage
            lcd.setCursor(0, 1);
            lcd.print("Val.:       ");
            lcd.setCursor(6, 1);
            lcd.print(value, 1);  // Affiche avec 1 décimale

        // Sortie si clic bouton
         if (rotaryEncoder.isEncoderButtonClicked()) {
            adjusting = false;
        } 

        delay(30);  // Anti-rebond / fluidité
        }
        
       
    }

    return value; // Add a return statement at the end of the function
}*/


/*void MenuManager::menuManager(){


switch (currentMenu) {
        case MAIN_MENU:
            mainMenuIndex = rotaryEncoder.readEncoder();
            displayMenu(defaultMenuOptions[0], mainMenuOptions, mainMenuSize, mainMenuIndex);
        break;
        case MANUAL_MENU:
            manualMenuIndex = rotaryEncoder.readEncoder();
            displayMenu(mainMenuOptions[0], manualMenuOptions, manualMenuSize, manualMenuIndex);  
        break;
        case AUTO_MENU:
            autoMenuIndex = rotaryEncoder.readEncoder();
            displayMenu(mainMenuOptions[1], autoMenuOptions, autoMenuSize, autoMenuIndex);   
        break;
        case SUB_MENU_HEIGHT:{
            float* selectionTest[] = { variableTest[0], variableTest[1], variableTest[6]}; 
            adjustHeightMenuIndex = rotaryEncoder.readEncoder();              //Sélection des variables en envoyer. Voir correspondace .h
            displayMenu(manualMenuOptions[0], adjustHeightMenuOptions, selectionTest, adjustHeightMenuSize, adjustHeightMenuIndex);
        break;}
        case SUB_MENU_AZIMUT:
            adjustAzimutMenuIndex = rotaryEncoder.readEncoder();
        break;
        case SUB_MENU_CENTER:
            centerMenuIndex = rotaryEncoder.readEncoder();
        break;
        case SUB_MENU_SHOW_DATA:
            showDataMenuIndex = rotaryEncoder.readEncoder();
        break;
        case SUB_MENU_CONFIG:
            configMenuIndex = rotaryEncoder.readEncoder();
        break; 
        case SUB_MENU_START_TEST:
            startTestMenuIndex = rotaryEncoder.readEncoder();
        break;            
}
}*/

/********************************************************
Initialisation de l'écran LCD 
    - Init rotary encoder
    - Init LCD
    - Init menu
***********************************************************/
void MenuManager::initLCD(){
   // initRotaryEncoder();
   //initLcd();
    rotaryEncoder.setBoundaries(0, bingMenuSize-1, false);
    updateData(0,0,0,0,0); //Initialisation des variables
    mainScreen();
}

/********************************************************
Fonction qui centre le titre sur l'écran
***********************************************************/
void MenuManager::centerTitle(const char* titre){
    int longueurTitre = strlen(titre);
    lcd.clear();
    lcd.setCursor((colonne-longueurTitre)/2, 0); //pour centrer titre
    lcd.print(String(titre));
}

/********************************************************
Fonction qui gère le scrolling de l'affichage
    - Si l'index de l'option qu'on veut afficher est supérieur à 3, on décale les options vers le haut
    - Si le nombre d'options est inférieur ou égal à 3, rien
    - On retourne la position du curseur
***********************************************************/
int MenuManager::scrollingDisplay(int cursorPosition){
    // Si le nombre d'options est inférieur ou égal à 3, pas de scrolling
      int maxVisible = ligne - 1; // 3 lignes visibles
    if (cursorPosition >= maxVisible) {
        return cursorPosition - (maxVisible - 1);
    } else {
        return 0;
    }
}

/********************************************************
Fonction qui affiche le menu
    - Affiche seulement la liste des options passée
    - Prend en paramètre une liste de const char* pour les options
***********************************************************/
void MenuManager::displayMenu(const char* titre, const char* options[], int optionsSize, int cursorPosition) {
        
    // Affichage du titre centré
    centerTitle(titre);
    int startIndex = 0;
    int optionIndex = 0;
    //Gérer le scrolling des options si le nombre d'options est supérieur à 3
    startIndex = scrollingDisplay(cursorPosition);
    

    // Afficher les options
    for (int i = 0; i < 3; i++) {
        lcd.setCursor(0, i + 1);
        optionIndex = startIndex + i;
        
        if (optionIndex < optionsSize) {
            if (cursorPosition == optionIndex) {
                lcd.print("* " + String(options[optionIndex]));
            } else {
                lcd.print("  " + String(options[optionIndex]));
            }
        }
    }
}

/********************************************************
Fonction qui affiche un menu 
    - Affiche les options et les valeurs des options
    - Prend en paramètre une liste de const char* pour les options
***********************************************************/
void MenuManager::displayMenu(const char* titre, const char* options[], float* variableTest[], int optionsSize, int cursorPosition) {
        
    // Affichage du titre centré
    centerTitle(titre);
    int optionIndex = 0;

    //Gérer le scrolling des options
    int startIndex = scrollingDisplay(cursorPosition);

    // Afficher les options
    for (int i = 0; i < ligne-1; i++) {
        lcd.setCursor(0, i + 1);
        optionIndex = startIndex + i;

       if (optionIndex < optionsSize) {
        // Afficher l'astérisque pour l'élément sélectionné
            if (cursorPosition == optionIndex) {
                lcd.print("* ");
            } else {
                lcd.print("  ");
            }

            // Afficher le nom de l'option
            lcd.print(String(options[optionIndex]));

            // Vérifier si la valeur n'est pas -1000 avant de l'afficher
            if (*variableTest[optionIndex] != -1000) {
                lcd.print(": ");
                lcd.print(String(*variableTest[optionIndex]));
            }
        }
    }
}

/********************************************************
Retourne l'index du menu Bing
    - MAIN = 0
    - START = 1
    - SHOW_RUN_DATA = 2
***********************************************************/
MenuManager::BingMenuOptions MenuManager::getMenuIndex(){
switch (menuBing) {
        case MAIN:
            menuBing = MAIN;
        break;
        case START:
            menuBing = START;
        break;
        case SHOW_RUN_DATA:
            menuBing = SHOW_RUN_DATA;
        break;
     
}
return menuBing;
}

/********************************************************
Fonction qui gère le clic de l'encodeur
    - Gère le clic de l'encodeur pour sortir ou changer de menu
***********************************************************/
void MenuManager::encoderClicManager() {
/*switch (currentMenu) {
        case MAIN_MENU:
            switch (mainMenuIndex) {
                case 0: // Mode Manuel
                    currentMenu = MANUAL_MENU;
                    manualMenuIndex = 0;
                    rotaryEncoder.setBoundaries(0, manualMenuSize -1, false);       //false empêche le loop à 0
                    displayMenu(mainMenuOptions[0], manualMenuOptions, manualMenuSize, manualMenuIndex);
                    break;
                case 1: // Mode Auto
                    currentMenu = AUTO_MENU;
                    autoMenuIndex = 0;
                    rotaryEncoder.setBoundaries(0, autoMenuSize -1, false); 
                    displayMenu(mainMenuOptions[1], autoMenuOptions, autoMenuSize, autoMenuIndex);
                    break;
            }
        break;

        case MANUAL_MENU:
            switch (manualMenuIndex) {
                case ADJUST_HEIGHT:{ //Ajuster hauteur
                    currentMenu = SUB_MENU_HEIGHT;
                    float* selectionTest[] = { variableTest[0], variableTest[1], variableTest[14]}; 
                    rotaryEncoder.setBoundaries(0, adjustHeightMenuSize -1, false);               //Sélection des variables en envoyer. Voir correspondace .h
                    displayMenu(manualMenuOptions[0], adjustHeightMenuOptions, selectionTest, adjustHeightMenuSize, adjustHeightMenuIndex);
                    break;}
                case ADJUST_AZIMUT:{ //Ajuster Azimut
                    currentMenu = SUB_MENU_AZIMUT;
                    float* selectionTest[] = { variableTest[2], variableTest[3], variableTest[14]};
                    rotaryEncoder.setBoundaries(0, adjustAzimutMenuSize -1, false);               
                    displayMenu(manualMenuOptions[1], adjustAzimutMenuOptions, selectionTest, adjustAzimutMenuSize, adjustAzimutMenuIndex);
                    break;}
                case CENTER:{ //Centrer
                    currentMenu = SUB_MENU_CENTER;
                    float* selectionTest[] = { variableTest[12], variableTest[14]};
                    rotaryEncoder.setBoundaries(0, centerMenuSize -1, false);               
                    displayMenu(manualMenuOptions[2],centerMenuOptions, selectionTest, centerMenuSize, centerMenuIndex);
                    break;}
                case SHOW_DATA:{ //Afficher Donnees
                    currentMenu = SUB_MENU_SHOW_DATA;
                    float* selectionTest[] = { variableTest[4], variableTest[5], variableTest[6], variableTest[14]};
                    rotaryEncoder.setBoundaries(0, showDataMenuSize -1, false);               
                    displayMenu(manualMenuOptions[3], showDataMenuOptions,selectionTest,showDataMenuSize, showDataMenuIndex);
                    break;}
                case RETURN_MANUAL: //Retour
                    currentMenu = MAIN_MENU;
                    mainMenuIndex = 0; // Réinitialiser l'index du menu principal
                    manualMenuIndex = 0;
                    rotaryEncoder.setBoundaries(0, mainMenuSize-1, false); 
                    lcd.clear();
                    displayMenu(defaultMenuOptions[0], mainMenuOptions, mainMenuSize, mainMenuIndex);
                    break;
            }
        break;

        case AUTO_MENU:
            switch (autoMenuIndex) {
                case CONFIGURATION:{
                    currentMenu = SUB_MENU_CONFIG;
                    float* selectionTest[] = {variableTest[7], variableTest[8], variableTest[9], variableTest[10], variableTest[11], variableTest[14]}; 
                    rotaryEncoder.setBoundaries(0, configMenuSize -1, false);               
                    displayMenu(autoMenuOptions[0], configMenuOptions, selectionTest, configMenuSize, configMenuIndex); // Add an additional argument for the index
                    break;}
                case START_TEST:{
                    currentMenu = SUB_MENU_START_TEST;
                    float* selectionTest[] = { variableTest[1], variableTest[3], variableTest[4], variableTest[13], variableTest[14]}; 
                    rotaryEncoder.setBoundaries(0, startTestMenuSize -1, false);               
                    displayMenu(autoMenuOptions[1], startTestMenuOptions, selectionTest, startTestMenuSize, startTestMenuIndex);
                    break;}
                case RETURN_AUTO:
                    currentMenu = MAIN_MENU;
                    autoMenuIndex = 0;
                    rotaryEncoder.setBoundaries(0, mainMenuSize -1, false); 
                    lcd.clear();
                    displayMenu(defaultMenuOptions[0], mainMenuOptions, mainMenuSize, mainMenuIndex);
                    break;
            }
        break;

        case SUB_MENU_HEIGHT:
        switch (adjustHeightMenuIndex) {
            case 0:{ //Hauteur demandee
                // Appel de la fonction d'ajustement
                *variableTest[0] = adjustValueWithEncoder(adjustHeightMenuOptions[0], *variableTest[0], 1.0, -90.0, 90.0);

                // Rechargement du menu une fois l'ajustement terminé
                float* selectionTest[] = { variableTest[0], variableTest[1], variableTest[14] };
                rotaryEncoder.setBoundaries(0, adjustHeightMenuSize - 1, false);
                displayMenu(manualMenuOptions[0], adjustHeightMenuOptions, selectionTest, adjustHeightMenuSize, adjustHeightMenuIndex);
                break;}
            case 1: //val reelle, on ne fait rien
                break;
            case 2: //retour
                currentMenu = MANUAL_MENU;
                adjustHeightMenuIndex = 0;
                rotaryEncoder.setBoundaries(0, manualMenuSize -1, false); 
                displayMenu(mainMenuOptions[0], manualMenuOptions, manualMenuSize, 0);
                break;
        }
        break;

        case SUB_MENU_AZIMUT:
        switch (adjustAzimutMenuIndex) {
            case 0:{ //Azimut demandee
                // Appel de la fonction d'ajustement
                *variableTest[2] = adjustValueWithEncoder(adjustAzimutMenuOptions[0], *variableTest[0], 1.0, -90.0, 90.0);

                // Rechargement du menu une fois l'ajustement terminé
                float* selectionTest[] = { variableTest[2], variableTest[3], variableTest[14] };
                rotaryEncoder.setBoundaries(0, adjustHeightMenuSize - 1, false);
                displayMenu(manualMenuOptions[1], adjustAzimutMenuOptions, selectionTest, adjustAzimutMenuSize, adjustAzimutMenuIndex);
                break;}
            case 1: //val reelle, on en fait rien
                break;
            case 2: //retour
                currentMenu = MANUAL_MENU;
                adjustAzimutMenuIndex = 0;
                rotaryEncoder.setBoundaries(0, manualMenuSize -1, false); 
                displayMenu(mainMenuOptions[0], manualMenuOptions,manualMenuSize, 1);
                break;
        }
        break;

        case SUB_MENU_CENTER:
        switch (centerMenuIndex) {
            case 0:{ //centrer
                // Insérer fonction pour centrer

                // Rechargement du menu une fois le centrage terminé
                float* selectionTest[] = { variableTest[12], variableTest[14]};
                rotaryEncoder.setBoundaries(0, centerMenuSize -1, false);               
                displayMenu(manualMenuOptions[2],centerMenuOptions, selectionTest, centerMenuSize, centerMenuIndex);
                break;}
            case 1: //retour
                currentMenu = MANUAL_MENU;
                centerMenuIndex = 0;
                rotaryEncoder.setBoundaries(0, manualMenuSize -1, false); 
                displayMenu(mainMenuOptions[0], manualMenuOptions, manualMenuSize, manualMenuIndex);
                break;
        }
        break;

        case SUB_MENU_SHOW_DATA:
        switch (showDataMenuIndex) {
            case 0:{ //PMPP
                break;}
            case 1:{ //VOLT
                break;}
            case 2:{ //AMPERAGE
                break;}
            case 3: //Return
                currentMenu = MANUAL_MENU;
                //selection.clear();
                rotaryEncoder.setBoundaries(0, manualMenuSize -1, false); 
                displayMenu(mainMenuOptions[0], manualMenuOptions, manualMenuSize, manualMenuIndex);
                break;
        }
        break;

        case SUB_MENU_CONFIG:
        switch (configMenuIndex) {
            case 0:{ //Increment
            // Appel de la fonction d'ajustement
                *variableTest[7] = adjustValueWithEncoder(autoMenuOptions[0], *variableTest[0], 1.0, 0.0, 25.0);

                // Rechargement du menu une fois l'ajustement terminé
                float* selectionTest[] = {variableTest[7], variableTest[8], variableTest[9], variableTest[10], variableTest[11], variableTest[14]}; 
                rotaryEncoder.setBoundaries(0, configMenuSize -1, false);               
                displayMenu(autoMenuOptions[0], configMenuOptions, selectionTest, configMenuSize, configMenuIndex); // Add an additional argument for the index

                //displayMenu(variables[7], 0, 25,configMenuIndex);
                break;}
            case 1:{ //Hauteur Min
                //displayMenu(variables[8],-90, 90,configMenuIndex);
                break;}
            case 2:{ //Hauteur Max
                //displayMenu(variables[9],-90, 90,configMenuIndex);
                break;}
            case 3:{ //Azimut Min
                //displayMenu(variables[10],-90, 90,configMenuIndex);
                break;}
            case 4:{ //Azimut Max
                //displayMenu(variables[11],-90, 90,configMenuIndex);
                break;}
            case 5: //Retour
                currentMenu = AUTO_MENU;
                //selection.clear();
                rotaryEncoder.setBoundaries(0, autoMenuSize -1, false); 
                displayMenu(mainMenuOptions[1], autoMenuOptions, autoMenuSize, autoMenuIndex);
                break;
        }
        break; 

        case SUB_MENU_START_TEST:
        switch (startTestMenuIndex) {
            case 0:{ //Valeur Azimut reelle
                break;}
            case 1:{ //Valeur Hauteur reelle
                break;}
            case 2:{ //PMPP
                break;}
            case 3:{ //Next
               //Fonction pour aller à la prochaine prise de valeur
                break;}
            case 4: //Retour
                currentMenu = AUTO_MENU;
                //autoMenuIndex = 0;
                //selection.clear();
                rotaryEncoder.setBoundaries(0, autoMenuSize -1, false); 
                displayMenu(mainMenuOptions[1], autoMenuOptions,autoMenuSize, autoMenuIndex);
                break;
        }
        break; 
}*/

switch (menuBing) {
        case MAIN:
            switch (bingMenuIndex) {
                case 0:{ // Mode Run
                    menuBing = START;
                    bingMenuIndex = 0;
                    runScreen();
                    break;}
                case 1:{ // Mode Afficher Donnees
                    menuBing = SHOW_RUN_DATA;
                    bingMenuIndex = 1;
                    dataScreen();
                    break;}
            }
        break;

        case START:
            menuBing = MAIN;
            mainScreen();
            break;
        case SHOW_RUN_DATA: 
            menuBing = MAIN;
            mainScreen();   
            break;

}
}

/********************************************************
Affichage de l'écran de chargement
    - Affiche le titre centré
    - Affiche le message de chargement
***********************************************************/
void MenuManager::loadingScreen(){
    lcd.print("--------------------");
    lcd.setCursor(0, 1);
    lcd.print("   ECLIPSE - BING   ");
    lcd.setCursor(0, 2);
    lcd.print("   Chargement...    ");
    lcd.setCursor(0, 3);
    lcd.print("--------------------");
}

/********************************************************
Affichage de l'écran principal
***********************************************************/
void MenuManager::mainScreen(){
    rotaryEncoder.setBoundaries(0, bingMenuSize-1, false); 
    lcd.clear();
    displayMenu(defaultMenuOptions[0], bingMenuOptions, bingMenuSize, bingMenuIndex);
}

/********************************************************
Affichage de l'écran de test
***********************************************************/
void MenuManager::runScreen(){
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("   Test en cours   ");
    lcd.setCursor(0, 2);
    lcd.print(" Clic pour arreter ");
    // Print data point restant?

   /* if (rotaryEncoder.isEncoderButtonClicked()) {
        // Arrêter le test
        lcd.clear();
        lcd.setCursor(0, 1);
        lcd.print("   Test arrete    ");
        delay(2000);
        lcd.clear();
    }*/
}

/********************************************************
Affichage de l'écran de données
***********************************************************/
void MenuManager::dataScreen(){
    lcd.clear();
    float* selectionTest[] = {variableTest[1], variableTest[3], variableTest[4], variableTest[5], variableTest[6]}; //Remplace le pourcntage par 0
    rotaryEncoder.setBoundaries(0, showDataMenuSize -1, false);               
    displayMenu(bingMenuOptions[1], showDataMenuOptions, selectionTest, showDataMenuSize, showDataMenuIndex);
}

/********************************************************
Gère la mémoire des index des menus
***********************************************************/
void MenuManager::indexManager(){
switch (menuBing) {
        case MAIN:
            bingMenuIndex = rotaryEncoder.readEncoder();
            mainScreen();
        break;
        case START:
        break;
        case SHOW_RUN_DATA:
            showDataMenuIndex = rotaryEncoder.readEncoder();  
            dataScreen();
        break;
}
}

/********************************************************
Gestion de l'encodeur rotatif
    - Si l'encodeur à été cliqué
    - Si l'encodeur à été tourné
***********************************************************/
MenuManager::BingMenuOptions MenuManager::encoderManager(){
// Gérer la rotation de l'encodeur
if (rotaryEncoder.encoderChanged()) {
        indexManager();
}
// Gérer le bouton pour sélectionner une option
if (rotaryEncoder.isEncoderButtonClicked()) {
        encoderClicManager();

} 
return getMenuIndex() ;

}

/********************************************************
Fonction qui met à jour les données
    - Prend en paramètre la hauteur, l'azimut, le PMPP, la tension et l'ampérage
    - Met à jour les variables correspondantes
***********************************************************/
void MenuManager::updateData(float hauteur, float azimut, float pmpp, float volt, float ampere){  
posReelleHauteur = hauteur;  
posReelleAzimut = azimut;   
PMPP = pmpp ;              
volt = volt ;             
amperage = ampere ;          
}



