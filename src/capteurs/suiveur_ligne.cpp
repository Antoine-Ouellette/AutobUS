/**
 * @file Fonction en lien avec le suiveur de ligne
 * Inclue la lecture du capteur et la conversion vers une valeur binaire
 */

#include "suiveur_ligne.h"
#include <Arduino.h>

//TODO : Brancher le second sensor pins A11, A12, A13
constexpr int nbPins = 6;
int pins[nbPins] = {A8, A9, A10, A11, A12, A13}; //Pins du suiveur gauche {gauche, centre, droit} et du suiveur droit{gauche, centre, droit}

//TODO : Hardcode les valeurs
float seuil_ligne = 0; // Seuil de luminosité qui représente la ligne
float seuil_extern = 0; // Seuil de luminosité qui représente le sol
float incertitude_SL = 0; // Écart accepté du seuil (incertitude suiveur de ligne)

uint8_t SUIVEUR_Read(int ID) {
    uint8_t resultat = 0b000; //Résultat du sensor

    for (int i = 0; i < nbPins; i++) {
        const int lecture = analogRead(pins[i+3*(ID)]);
        const bool isLine = ((seuil_ligne - incertitude_SL) < lecture && lecture < (seuil_ligne + incertitude_SL));
        resultat |= isLine << i;
    }

    return resultat;
}
