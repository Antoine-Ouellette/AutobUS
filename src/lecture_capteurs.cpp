/**
 * @file Fonctions pour la lecture de chaque capteur.
 */
#include <LibRobus.h> // Essentielle pour utiliser RobUS.
#include "variables_globales.h" // Inclure les variables globales partagées entre tous les fichiers.
#include "lecture_capteurs.h"

int buttonState = HIGH;     // Record button state, and initial the state to high level 
int relayState = LOW;       // Record relay state, and initial the state to low level 
int lastButtonState = HIGH; // Record the button state of last detection 
long lastChangeTime = 0;    // Record the time point for button state change

/**
 * Lit l'état du capteur de couleur.
 * Détecte s'il y a une station de bus en dessous du robot.
 * @return true si une station de bus est détectée, false sinon.
 */
bool lireCapteurCouleur() {
    // ...
    return false;
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
 * @return true si le bouton est appuyé, false sinon.
 */
bool lireBoutonArretDemande() {
    // Read current state of button pin.
    int nowButtonState = digitalRead(PIN_BUTTON);

    // If button pin state has changed, record the time point.
    if (nowButtonState != lastButtonState) {
        lastChangeTime = millis();
    }

    // If button state changes, and stays stable for a while, then it should have skipped the bounce area.
    if (millis() - lastChangeTime > 10) {
        if (buttonState != nowButtonState) { // Only proceed if button state has changed.
            buttonState = nowButtonState;
            if (buttonState == LOW) { // Low level indicates the button is pressed.
                isArreterProchaineStation = true;
            }
        }
    }

    // Save the latest state for next time.
    lastButtonState = nowButtonState;
}