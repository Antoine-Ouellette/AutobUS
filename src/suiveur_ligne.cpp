/**
 * @file Fonction en lien avec le suiveur de ligne
 * Inclue la lecture du capteur et la conversion vers une valeur binaire
 */

#include "suiveur_ligne.h"
#include <Arduino.h>
#include <pins_arduino.h>

#include "variables_globales.h"

int pins[3] = {A8, A9, A10}; //Pins du suiveur {gauche, centre, droit}


float seuil_centre = 0; // Seuil de luminosité qui représente la ligne
float seuil_extern = 0; // Seuil de luminosité qui représente le sol
float incertitude_SL = 0; // Écart accepté du seuil (incertitude suiveur de ligne)

uint8_t resultat = 0b000;

void initialisation_seuils() {
    seuil_centre = analogRead(pins[1]);
    seuil_extern = (analogRead(pins[0]) + analogRead(pins[2])) / 2;
    incertitude_SL = abs(seuil_extern - seuil_centre) / 4;
}

uint8_t SUIVEUR_Read() {
    int lecture = 0;

    for (int i = 0; i < 3; i++) {
        lecture = analogRead(pins[i]);
        resultat = (seuil_centre - incertitude_SL < lecture && lecture < seuil_centre + incertitude_SL) << i;
    }

    return resultat;
}
