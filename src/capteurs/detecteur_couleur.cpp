/**
 * Fonction en lien avec le détecteur de couleurs
 * Inclue la lecture du capteur et la conversion vers une valeur de l'enum COULEURS
 */

#include <Arduino.h>
#include <Adafruit_TCS34725.h>
#include "detecteur_couleur.h"


//Variable pour connaitre le sensor de couleur
Adafruit_TCS34725 colorSensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_16X);
#define COMMAND_BIT 0x80


// TODO : ajuster pour les vraies valeurs
int sampleIndex = 0;
RGB couleurSample[nbSamples];

unsigned long nextReading = 0;
unsigned long integration_ms = 50;

void COLOR_SENSOR_init() {
    for (int i = 0; i < nbSamples; i++) {
        couleurSample[i].r = 0;
        couleurSample[i].g = 0;
        couleurSample[i].b = 0;
    }

    colorSensor.begin();
    // Met la led à ON pour pouvoir la lire
    colorSensor.setInterrupt(false);
    delay(50); // prend 50 ms pour l'initialiser
}

void COLOR_SENSOR_update() {
    if (millis() < nextReading) return; // Couleur pas encore prête
    nextReading = millis() + integration_ms;

    sampleIndex = (sampleIndex + 1) % nbSamples;

    // Lire les valeurs
    couleurSample[sampleIndex].r = colorSensor.read16(TCS34725_RDATAL);
    couleurSample[sampleIndex].g = colorSensor.read16(TCS34725_GDATAL);
    couleurSample[sampleIndex].b = colorSensor.read16(TCS34725_BDATAL);

    // ## Utile lors de l'ajustement des couleurs à lires ##

    // Serial.print("Read : ");
    // Serial.print(couleurSample[sampleIndex].r);
    // Serial.print(" ");
    // Serial.print(couleurSample[sampleIndex].g);
    // Serial.print(" ");
    // Serial.println(couleurSample[sampleIndex].b);
}


COULEURS COLOR_SENSOR_Read() {
    // Variables pour enregistrer les valeurs de couleurs
    uint16_t r = 0, g = 0, b = 0;

    // Faire la moyenne
    for (const RGB i: couleurSample) {
        r += i.r / float(nbSamples);
        g += i.g / float(nbSamples);
        b += i.b / float(nbSamples);
    }

    // Serial.print("Real : ");
    // Serial.print(r);
    // Serial.print(" ");
    // Serial.print(g);
    // Serial.print(" ");
    // Serial.print(b);
    // Serial.println("");

    for (int i = 0; i < nbCouleurs; i++) {
        // Boucle pour regarder toutes les couleurs

        // Si la couleur ne respecte pas un des seuille, aller voir la prochaine couleurs enregistré
        if (couleursDef[i].r - incertitude_DC < r && r < couleursDef[i].r + incertitude_DC) {
            //Chill rien faire
        } else {
            continue;
        }

        if (couleursDef[i].g - incertitude_DC < g && g < couleursDef[i].g + incertitude_DC) {
            //Chill rien faire
        } else {
            continue;
        }

        if (couleursDef[i].b - incertitude_DC < b && b < couleursDef[i].b + incertitude_DC) {
            // Chill rien faire
        } else {
            continue;
        }


        // // ## For Debug ##
        // Serial.print("def ");
        // Serial.print(couleursDef[i].r);
        // Serial.print(" Color : ");
        // Serial.print(i);
        // Serial.print(" = ");
        // Serial.println(intToColor(i));
        // // ## ##
        return intToColor(i);
    }

    return BLANC;
}


COULEURS intToColor(const int n) {
    if (n < 0 || n >= nbCouleurs) {
        return NOIR; // Valeur par défaut
    }
    return couleurs[n];
}
