/**
 * @file Fonction en lien avec le suiveur de ligne
 * Inclue la lecture du capteur et la conversion vers une valeur binaire
 */
 
#include "suiveur_ligne.h"
#include <Arduino.h>
 
constexpr int nbPins = 6;
constexpr int nbPinsPerSensor = 3;
int pins[nbPins] = {A8, A9, A10, A11, A12, A13}; //Pins du suiveur gauche {gauche, centre, droit} et du suiveur droit{gauche, centre, droit}

//TODO : Hardcode les valeurs
float seuil_ligneGauche = 0; // Seuil de luminosité qui représente la ligne
float seuil_externGauche = 0; // Seuil de luminosité qui représente le sol
float incertitude_SL_Gauche = 0; // Écart accepté du seuil (incertitude suiveur de ligne)
float seuil_ligneDroit = 0; // Seuil de luminosité qui représente la ligne
float seuil_externDroit = 0; // Seuil de luminosité qui représente le sol
float incertitude_SL_Droit = 0; // Écart accepté du seuil (incertitude suiveur de ligne)

void SUIVEUR_init() {
    for (int i = 0; i < nbPins; i++) {
        pinMode(pins[i], INPUT);
    }
    seuil_ligneGauche = analogRead(pins[2]);
    seuil_externGauche = (analogRead(pins[1]) + analogRead(pins[0])) / 2.0f;
    incertitude_SL_Gauche = abs(seuil_externGauche - seuil_ligneGauche) / 4.0;

    // seuil_ligneDroit = analogRead(pins[4]);
    // seuil_externDroit = (analogRead(pins[3]) + analogRead(pins[5])) / 2.0f;
    // incertitude_SL_Droit = abs(seuil_externDroit - seuil_ligneDroit) / 4.0;
}


uint8_t SUIVEUR_Read(int ID) {
    uint8_t resultat = 0b000; //Résultat du sensor
    //if (!ID) {
    for (int i = 0; i < nbPinsPerSensor; i++) {
        const int lecture = analogRead(pins[i+3*ID]);
        const bool isLine = ((seuil_ligneGauche - incertitude_SL_Gauche) < lecture && lecture < (seuil_ligneGauche + incertitude_SL_Gauche));
        resultat |= isLine << i;
        Serial.print(" ");
        Serial.print(lecture);
        Serial.print(" ");
    }
    // }
    // else if (ID) {
    //     for (int i = 3; i < nbPins; i++) {
    //         const int lecture = analogRead(pins[i]);
    //         const bool isLine = ((seuil_ligneDroit - incertitude_SL_Droit) < lecture && lecture < (seuil_ligneDroit + incertitude_SL_Droit));
    //         resultat |= isLine << i;
    //     }
    // }
    return resultat;
}

/*bool detectUsingThreshold(float seuil, float incertitude, int raw) {
    return (raw > (seuil - incertitude) && raw < (seuil + incertitude));
}
*/