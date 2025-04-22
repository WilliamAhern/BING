#ifndef MENU_MANAGER_H
#define MENU_MANAGER_H

#include <vector>
#include <LCD.h>

//#include <PSA.h>

class MenuManager {
public:
   
    // Enum pour les types de menus
    enum MenuType {
        MAIN_MENU,
        MANUAL_MENU,
        AUTO_MENU,
        //Dinguerie pas idéale mais fonctionnelle. Au prochain programmeur, à changer.
        SUB_MENU_HEIGHT,            // Sous-menu de MANUAL_MENU -> Ajuster Hauteur
        SUB_MENU_AZIMUT,            // Sous-menu de MANUAL_MENU -> Ajuster Azimut
        SUB_MENU_CENTER,            // Sous-menu de MANUAL_MENU -> Centrage
        SUB_MENU_SHOW_DATA,         // Sous-menu de MANUAL_MENU -> Afficher données
        SUB_MENU_CONFIG,            // Sous-menu de AUTO_MENU -> Configuration
        SUB_MENU_START_TEST   // Sous-menu de AUTO_MENU -> Démarrage du test
    };

    // Enum pour les options dans le menu bing
    enum BingMenuOptions {
        MAIN,
        START,
        SHOW_RUN_DATA,
    };

    // Enum pour les options dans le enu Manuel
    enum ManualMenuOptions {
        ADJUST_HEIGHT,
        ADJUST_AZIMUT,
        CENTER,
        SHOW_DATA,
        RETURN_MANUAL
    };

    //Enum pour les options dans le menu Automatique
    enum AutoMenuOptions {
        CONFIGURATION,
        START_TEST,
        RETURN_AUTO
    };

    enum AdjustHeightMenuOptions{
        HEIGHT_VALEUR_DEMANDEE,
        HEIGHT_VALEUR_REELLE,
        RETURN_HEIGHT
    };

    enum AdjustAzimutMenuOptions{
        AZI_VALEUR_DEMANDEE,
        AZI_VALEUR_REELLE,
        RETURN_AZI
    };

    enum CenterMenuOptions{
        CENTER_IN_ACTION,
        RETURN_CENTER
    };

    enum ShowDataMenuOptions{
        MANU_PMPP,
        MANU_VOLT,
        MANU_AMPERAGE,
        RETURN_DATA
    };

    enum ConfigMenusOptions{
        INCREMENT,
        PLAGE_HAUTMIN,
        PLAGE_HAUTMAX,
        PLAGE_AZIMIN,
        PLAGE_AZIMAX,
        RETURN_CONFIG
    };

    enum StartTestMenuOptions{
        VAL_AZIMUT,
        VAL_HAUTEUR,
        AUTO_PMPP,
        AUTO_NEXT,
        RETURN_TEST
    };
    
    /*//Logique de variable affichable pour les différents sous menus
    typedef struct {
      const char* nom;       // Nom à afficher
      float* valeur;         // Pointeur vers la valeur actuelle
      bool modifiable;       // True = peut être modifiée
    } VariableAffichable;*/
    
    //Déclaration du vecteur contenant les informations des variables de type VariableAffichable
    //std::vector<VariableAffichable*> selection;   

    //Constructeur et destructeur
    MenuManager();
    ~MenuManager();

    //Tableau des options des menus
    //Default
    static const char* defaultMenuOptions[]; // juste pour le titre
    static const char* mainMenuOptions[];
    static const char* bingMenuOptions[];
    //Main
    static const char* manualMenuOptions[];
    static const char* autoMenuOptions[];
    //Manual
    static const char* adjustHeightMenuOptions[];
    static const char* adjustAzimutMenuOptions[];
    static const char* centerMenuOptions[];
    static const char* showDataMenuOptions[];
    //Auto
    static const char* configMenuOptions[];
    static const char* startTestMenuOptions[];
    
    //Size des tableaux des options des menus
    //Default
    static const int mainMenuSize;
    static const int bingMenuSize;
    //Main
    static const int manualMenuSize;
    static const int autoMenuSize;
    //Manual
    static const int adjustHeightMenuSize;
    static const int adjustAzimutMenuSize;
    static const int centerMenuSize;
    static const int showDataMenuSize;
    //Auto
    static const int configMenuSize;
    static const int startTestMenuSize;

    //Déclaration des fonctions
    void displayMenu(const char* titre, const char* options[], int optionsSize, int cursorPosition);
    //void displayMenu(const char* titre, const std::vector<VariableAffichable*>& options, int cursorPosition);
    //void displayMenu(VariableAffichable variable, int rangeMin, int rangeMax, int prevMenuIndex);
    //void displayMenu(const char* titre, float* variableTest[], int rangeMin, int rangeMax, int prevMenuIndex);
    //float adjustValueWithEncoder(const char* label, float variableTest, float step, float minVal, float maxVal);
    void displayMenu(const char* titre, const char* options[], float* variableTest[], int optionsSize, int cursorPosition);

    void centerTitle(const char* titre);
    int scrollingDisplay(int cursorPosition);
    //void menuManager();
    BingMenuOptions getMenuIndex();
    void indexManager();
    void encoderClicManager();
    BingMenuOptions encoderManager();
    void loadingScreen();
    void initLCD();
    void mainScreen();
    void runScreen();
    void dataScreen();
    void updateData(float hauteur, float azimut, float pmpp, float volt, float ampere);
  


private:

//Declaration de tableau de variables
//VariableAffichable variables[15];
float* variableTest[15];

// Options pour chaque menu
//static const char* mainMenuOptions[2];
//static const char* manualMenuOptions[5];
//static const char* autoMenuOptions[3];

//MenuType currentMenu = MAIN_MENU;  
BingMenuOptions menuBing = MAIN;               // Menu initial
//ManualMenuOptions currentManualMenu;                // Menu Manuel
//AutoMenuOptions currentAutoMenu;                    // Menu Auto
//AdjustHeightMenuOptions currentAdjustHeightMenu;
//AdjustAzimutMenuOptions currentAdjustAzimutMenu;
//CenterMenuOptions currentCenterMenu;
//ShowDataMenuOptions currentShowDataMenu;
//ConfigMenusOptions currentConfigMenu;
//StartTestMenuOptions currrentStartTestMenu;


//Mémoire des index des menus. Utilisé pour la navigation des menus
//  et pour le gestion des actions lors d'un encoderClic
int mainMenuIndex; 
int bingMenuIndex;               
int manualMenuIndex;              
int autoMenuIndex;                
int adjustHeightMenuIndex;
int adjustAzimutMenuIndex;
int centerMenuIndex;
int showDataMenuIndex;
int configMenuIndex;
int startTestMenuIndex;

float posReelleHauteur;
float posReelleAzimut;
float PMPP;
float tension;
float amperage;

};


#endif

