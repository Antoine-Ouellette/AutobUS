/**
 * @file Fonction en lien avec le suiveur de ligne
 * Inclue la lecture du capteur et la conversion vers une valeur binaire
 */

#include "suiveur_ligne.h"
#include <Arduino.h>

//TODO : Brancher le second sensor pins A11, A12, A13
constexpr int nbPins = 6;
constexpr int nbPinsPerSensor = 3;
int pins[nbPins] = {A10, A9, A8, A13, A12, A11}; //Pins du suiveur gauche {gauche, centre, droit} et du suiveur droit{gauche, centre, droit}

//TODO : Hardcode les valeurs
float seuil_ligne = 0; // Seuil de luminosité qui représente la ligne
float seuil_extern = 0; // Seuil de luminosité qui représente le sol
float incertitude_SL = 0; // Écart accepté du seuil (incertitude suiveur de ligne)

void SUIVEUR_init() {
    for (int i = 0; i < nbPins; i++) {
        pinMode(pins[i], INPUT);
    }
    seuil_ligne = analogRead(pins[2]);
    seuil_extern = (analogRead(pins[1]) + analogRead(pins[0])) / 2.0f;
    incertitude_SL = abs(seuil_extern - seuil_ligne) / 4.;

    Serial.print("Seuil ligne : ");
    Serial.println(pins[0]);
    Serial.println(pins[1]);
    Serial.println(pins[2]);
}


uint8_t SUIVEUR_Read(int ID) {
    uint8_t resultat = 0b000000; //Résultat du sensor

    for (int i = 0; i < nbPinsPerSensor; i++) {
        const int lecture = analogRead(pins[i+3*(ID)]);
        const bool isLine = ((seuil_ligne - incertitude_SL) < lecture && lecture < (seuil_ligne + incertitude_SL));
        resultat |= isLine << i;
    }

    return resultat;
}
