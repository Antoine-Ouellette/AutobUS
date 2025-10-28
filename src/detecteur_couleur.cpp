/**
 * Fonction en lien avec le détecteur de couleurs
 * Inclue la lecture du capteur et la conversion vers une valeur de l'enum COULEURS
 */

#include "detecteur_couleur.h"
#include <Arduino.h>
#include <Adafruit_TCS34725.h>


//Variable pour connaitre le sensor de couleur
Adafruit_TCS34725 colorSensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

// TODO : ajuster pour les vraies valeurs des couleurs
uint16_t couleurs[nbCouleurs][3] = {
    {0, 0, 0}, // Noir
    {255, 242, 212}, // Rouge
    {221, 293, 218}, // Vert
    {204, 265, 222}, // Bleu
    {281, 301, 204} // Jaune

};

// TODO : ajuster pour les vraies valeurs
int incertitude_DC = 20; // Écart accepté (incertitude détecteur couleur)

void initColorSensor() {
    colorSensor.begin();
    //Met la led à ON pour pouvoir la lire
    colorSensor.setInterrupt(false);
    delay(50); // prend 50 ms pour l'initialiser
}

COULEURS COLORSENSOR_Read() {
    //Variables pour enregistrer les valeurs de couleurs lues
    uint16_t r, g, b, clear;

    //Lire les valeurs
    colorSensor.getRawData(&r, &g, &b, &clear);

    for (int i = 0; i < nbCouleurs; i++) {
        //Boucle pour regarder toutes les couleurs

        if (couleurs[i][0] - incertitude_DC > r || couleurs[i][0] + incertitude_DC < r) continue;
        if (couleurs[i][1] - incertitude_DC > g || couleurs[i][1] + incertitude_DC < g) continue;
        if (couleurs[i][2] - incertitude_DC > b || couleurs[i][2] + incertitude_DC < b) continue;

        return intToColor(i);
    }

    return NOIR;
}


COULEURS intToColor(const int n) {
    switch (n) {
        case 0:
            return NOIR;
        case 1:
            return ROUGE;
        case 2:
            return VERT;
        case 3:
            return BLEU;
        case 4:
            return JAUNE;
        default: return NOIR;
    }
}
