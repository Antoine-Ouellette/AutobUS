/**
 * @file Fonction en lien avec le suiveur de ligne
 * Inclue la lecture du capteur et la conversion vers une valeur binaire
 */

#include "suiveur_ligne.h"
#include <Arduino.h>

constexpr int nbPins = 3;
int pins[nbPins] = {A8, A9, A10}; //Pins du suiveur {gauche, centre, droit}


float seuil_centre = 0; // Seuil de luminosité qui représente la ligne
float seuil_extern = 0; // Seuil de luminosité qui représente le sol
float incertitude_SL = 0; // Écart accepté du seuil (incertitude suiveur de ligne)

void SUIVEUR_init() {
    for (int i = 0; i < nbPins; i++) {
        pinMode(pins[i], INPUT);
    }
    seuil_centre = analogRead(pins[nbPins / 2]);
    seuil_extern = (analogRead(pins[0]) + analogRead(pins[nbPins - 1])) / 2.0f;
    incertitude_SL = abs(seuil_extern - seuil_centre) / 4.;
}

uint8_t SUIVEUR_Read() {
    uint8_t resultat = 0b000; //Résultat du sensor

    for (int i = 0; i < nbPins; i++) {
        const int lecture = analogRead(pins[i]);
        const bool isCenter = ((seuil_centre - incertitude_SL) < lecture && lecture < (seuil_centre + incertitude_SL));
        resultat |= isCenter << i;
    }

    return resultat;
}
