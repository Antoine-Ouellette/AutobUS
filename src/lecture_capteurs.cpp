/**
 * @file Fonctions pour la lecture de chaque capteur.
 */
#include <LibRobus.h> // Essentielle pour utiliser RobUS.
#include "variables_globales.h" // Inclure les variables globales partagées entre tous les fichiers.
#include "lecture_capteurs.h"

#include "arret_bus.h"

/******************************************************************************
Variables globales et #define
L'ensemble des fonctions y ont accès.
******************************************************************************/

/**
 * Enregistre l'état du bouton d'arrêt demandé.
 * L'état initial est tension à HIGH (pas appuyé).
 */
int buttonState = HIGH;

/**
 * Enregistre l'état précédent du bouton d'arrêt demandé.
 */
int lastButtonState = HIGH;

/**
 * Enregistre le temps au dernier changement d'état du bouton.
 */
long lastChangeTime = 0;

/**
 * Définition d'une couleur.
 * Contient les seuils de valeurs acceptées pour conclure que le capteur voit cette couleur.
 * @property name: Nom de la couleur.
 * @property redMin: Valeur minimum de rouge pour dire que c'est cette couleur.
 * @property redMax: Valeur maximum de rouge pour dire que c'est cette couleur.
 * @property greenMin: Valeur minimum de vert pour dire que c'est cette couleur.
 * @property greenMax: Valeur maximum de vert pour dire que c'est cette couleur.
 * @property blueMin: Valeur minimum de bleu pour dire que c'est cette couleur.
 * @property blueMax: Valeur maximum de bleu pour dire que c'est cette couleur.
 */
struct ColorDefinition {
    const char* name;
    uint16_t redMin, redMax;
    uint16_t greenMin, greenMax;
    uint16_t blueMin, blueMax;
};

/**
 * Liste des couleurs possibles qui peuvent être détectées par le capteur.
 * Tableau contenant des définitions de couleurs.
 */
ColorDefinition possibleColors[] = {
    {"NOIR", 0, 50, 0, 50, 0, 50},
    {"BLANC", 200, 255, 200, 255, 200, 255},
    {"ROUGE", 200, 255, 0, 100, 0, 100},
    {"VERT", 0, 100, 200, 255, 0, 100},
    {"BLEU", 0, 100, 0, 100, 200, 255},
    {"JAUNE", 200, 255, 200, 255, 0, 100},
    {"ROSE", 200, 255, 0, 100, 200, 255},
    {"MAUVE", 200, 255, 0, 100, 200, 255}
};

/**
 * Valeurs actuelles détectées par le capteur de couleur.
 * Ce sont les valeurs brutes lues du capteur. Elles ne sont pas encore ajustées
 * selon l'éclairage ambiant.
 */
uint16_t colorSensorRawRedValue, colorSensorRawGreenValue, colorSensorRawBlueValue, colorSensorRawBrightnessValue;

/**
 * Valeurs actuelles détectées par le capteur de couleur.
 * Ce sont les valeurs ajustées selon l'éclairage ambiant.
 */
float colorSensorRedValue, colorSensorGreenValue, colorSensorBlueValue;

/**
 * Keeps track of the sum of color values for averaging.
 */
float colorSensorSumR = 0, colorSensorSumG = 0, colorSensorSumB = 0;

/**
 * Valeurs de la couleur calculée après plusieurs échantillons.
 */
float averageRed = 0, averageGreen = 0, averageBlue = 0;

/**
 * Nombre d'échantillons valides pris pour le calcul de la moyenne.
 */
int numberValidSamples = 0;

/**
 * Lit l'état du capteur de couleur.
 * Détecte s'il y a une station de bus en dessous du robot.
 * @return true si une station de bus est détectée, false sinon.
 */
bool lireCapteurCouleur() {
    // Réinitialiser les sommes et le compteur d'échantillons valides.
    colorSensorSumR = 0;
    colorSensorSumG = 0;
    colorSensorSumB = 0;
    numberValidSamples = 0;

    for (int i = 0; i < 10; i++) {
        // Lire les valeurs du capteur de couleur.
        ColorSensor.getRawData(
            &colorSensorRawRedValue,
            &colorSensorRawGreenValue,
            &colorSensorRawBlueValue,
            &colorSensorRawBrightnessValue
        );

        // Calculer les vraies valeurs de couleur en fonction de la luminosité.
        if (colorSensorRawBrightnessValue > 0) { // évite la division par zéro.
            colorSensorRedValue = (float)colorSensorRawRedValue / colorSensorRawBrightnessValue;
            colorSensorGreenValue = (float)colorSensorRawGreenValue / colorSensorRawBrightnessValue;
            colorSensorBlueValue = (float)colorSensorRawBlueValue / colorSensorRawBrightnessValue;

            colorSensorSumR += colorSensorRedValue;
            colorSensorSumG += colorSensorGreenValue;
            colorSensorSumB += colorSensorBlueValue;
            numberValidSamples++;
        }
        delay(20);
    }
    
    // Calculer la couleur moyenne.
    averageRed = colorSensorSumR / numberValidSamples;
    averageGreen = colorSensorSumG / numberValidSamples;
    averageBlue = colorSensorSumB / numberValidSamples;

    // Retourner le nom de la couleur correspondante.
    const char* detectedColor = "Inconnue";
    // foreach color in possibleColors.
    for (auto &color : possibleColors) {
        // Si dans le range de la couleur.
        if (
            averageRed >= color.redMin && averageRed <= color.redMax &&
            averageGreen >= color.greenMin && averageGreen <= color.greenMax &&
            averageBlue >= color.blueMin && averageBlue <= color.blueMax
        ) {
            // C'est cette couleur.
            detectedColor = color.name;
            break;
        }
    }

    // Si la couleur détectée n'est pas NOIR ou BLANC, c'est une station de bus.
    return (strcmp(detectedColor, "NOIR") != 0 && strcmp(detectedColor, "BLANC") != 0);
}

/**
 * Lit l'état du capteur de proximité.
 * Détecte s'il y a un obstacle devant le robot.
 * @return true si un obstacle est détecté, false sinon.
 */
bool lireCapteurProximite() {
    // ...
    return false;
}

/**
 * Lit l'état du bouton d'arrêt demandé.
 */
void lireBoutonArretDemande() {
    // // Read current state of button pin.
    // int nowButtonState = digitalRead(PIN_BUTTON);
    // Serial.print("Button state: "); Serial.println(nowButtonState);

    // // If button pin state has changed, record the time point.
    // if (nowButtonState != lastButtonState) {
    //     lastChangeTime = millis();
    // }

    // // If button state changes, and stays stable for a while, then it should have skipped the bounce area.
    // if (millis() - lastChangeTime > 10) {
    //     if (buttonState != nowButtonState) { // Only proceed if button state has changed.
    //         buttonState = nowButtonState;
    //         if (buttonState == LOW) { // Low level indicates the button is pressed.
    //             isArreterProchaineStation = true;
    //             Serial.println("Arrêt demandé.");
    //         }
    //     }
    // }

    // // Save the latest state for next time.
    // lastButtonState = nowButtonState;
    // Serial.print("Button state: "); Serial.println(digitalRead(PIN_BUTTON));
    
    if(!digitalRead(PIN_BUTTON)) {

        ajouterArretDemande(nextArret);

#if CONSOLE_DEBUG
        Serial.println("Arrêt demandé.");
#endif

    }
}