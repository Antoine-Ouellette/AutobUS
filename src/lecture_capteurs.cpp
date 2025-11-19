/**
 * @file Fonctions pour la lecture de chaque capteur.
 */
#include <LibRobus.h> // Essentielle pour utiliser RobUS.
#include "variables_globales.h" // Inclure les variables globales partagées entre tous les fichiers.
#include "lecture_capteurs.h"
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
    if (digitalRead(PIN_BUTTON) == LOW) {
        isArreterProchaineStation = true;
        return true;
    } else {
        return false;
    }
}