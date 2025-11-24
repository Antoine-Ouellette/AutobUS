/**
 * Fonction en lien avec le détecteur de couleurs
 * Inclue la lecture du capteur et la conversion vers une valeur de l'enum COULEURS
 */

#include <Arduino.h>
#include <Adafruit_TCS34725.h>
#include "detecteur_couleur.h"


//Variable pour connaitre le sensor de couleur
Adafruit_TCS34725 colorSensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

// TODO : ajuster pour les vraies valeurs des couleurs
uint16_t couleurs[nbCouleurs][3] = {
    {199, 246, 210}, // Gris
    {255, 145, 129}, // Rouge
    {146, 139, 104}, // Brun
    {93, 144 ,163}, // Bleu
    {313, 175, 137} // Orange
};

// TODO : ajuster pour les vraies valeurs
int incertitude_DC = 25; // Écart accepté (incertitude détecteur couleur)

void COLOR_SENSOR_init() {
    colorSensor.begin();
    // Met la led à ON pour pouvoir la lire
    colorSensor.setInterrupt(false);
    delay(50); // prend 50 ms pour l'initialiser
}

COULEURS COLORSENSOR_Read() {
    // Variables pour enregistrer les valeurs de couleurs lues
    uint16_t r, g, b, clear;

    // Lire les valeurs
    colorSensor.getRawData(&r, &g, &b, &clear);

    // ## Utile lors de l'ajustement des couleurs à lires ##
    // Serial.print(r);
    // Serial.print(" ");
    // Serial.print(g);
    // Serial.print(" ");
    // Serial.print(b);
    // Serial.println("");

    for (int i = 0; i < nbCouleurs; i++) {
        // Boucle pour regarder toutes les couleurs

        // Si la couleur ne respecte pas un des seuille, aller voir la prochaine couleurs enregistré
        if (couleurs[i][0] - incertitude_DC > r || couleurs[i][0] + incertitude_DC < r) continue;
        if (couleurs[i][1] - incertitude_DC > g || couleurs[i][1] + incertitude_DC < g) continue;
        if (couleurs[i][2] - incertitude_DC > b || couleurs[i][2] + incertitude_DC < b) continue;

        return intToColor(i);
    }

    return BLANC;
}


COULEURS intToColor(const int n) {
    switch (n) {
        case 0:
            return GRIS;
        case 1:
            return ROUGE;
        case 2:
            return BRUN;
        case 3:
            return BLEU;
        case 4:
            return ORANGE;
        default: return BLANC;
    }
}
