/**
 * @file Fonctions en lien avec les moteurs des roues du robot.
 * Inclut les déplacements et le PID.
 */
#include <LibRobus.h> // Essentielle pour utiliser RobUS.
#include "variables_globales.h" // Inclure les variables globales partagées entre tous les fichiers.

// Variable pour le timer de rotation.
unsigned long millisTourner = 0;

// Constantes PID.
//...

/**
 * Arrêter le déplacement du robot.
 */
void arreter() {
    // Mettre les vitesses des deux moteurs à 0%.
    MOTOR_SetSpeed(LEFT, 0);
    MOTOR_SetSpeed(RIGHT, 0);
}

/**
 * Démarrer le déplacement du robot à une vitesse donnée.
 * Active les 2 moteurs jusqu'à ce qu'on les arrête.
 * @param vitesse Pourcentage de la vitesse à laquelle les moteurs doivent rouler.
 * La valeur doit être entre 0 et 1. 1 étant pour avancer à 100% de la vitesse possible.
 * TODO: Sera modifié pour utiliser un PID.
 */
void avancer(float vitesse) {
    // Appliquer une vitesse aux deux moteurs du robot.
    MOTOR_SetSpeed(LEFT, vitesse);
    MOTOR_SetSpeed(RIGHT, vitesse);
}

/**
 * Démarrer le déplacement du robot en marche arrière à une vitesse donnée.
 * Active les 2 moteurs jusqu'à ce qu'on les arrête.
 * @param vitesse Pourcentage de la vitesse à laquelle les moteurs doivent rouler.
 * La valeur doit être entre 0 et 1. 1 étant pour reculer à 100% de la vitesse possible.
 * TODO: Sera modifié pour utiliser un PID.
 */
void reculer(float vitesse) {
    // Appliquer une vitesse aux deux moteurs du robot.
    MOTOR_SetSpeed(LEFT, -vitesse);
    MOTOR_SetSpeed(RIGHT, -vitesse);
}

/**
 * Tourner le robot à gauche d'un certain nombre de degrés à une vitesse donnée.
 * @param direction LEFT ou RIGHT pour indiquer dans quelle direction tourner.
 * @param degres Nombre de degrés à tourner.
 * @param vitesse Pourcentage de la vitesse à laquelle les moteurs doivent rouler.
 * La valeur doit être entre 0 et 1. 1 étant pour tourner à 100% de la vitesse possible.
 * TODO: Sera modifié pour utiliser un PID. Ceci est un exemple.
 */
void tourner(bool direction, int degres, float vitesse) {
    // Si c'est la première fois qu'on entre dans la fonction.
    if (millisTourner == 0) {
        if (direction == LEFT) {
            // Appliquer des vitesses opposées aux deux moteurs.
            MOTOR_SetSpeed(LEFT, -vitesse); // La roue gauche recule.
            MOTOR_SetSpeed(RIGHT, vitesse); // La roue droite avance.
        } else {
            // Appliquer des vitesses opposées aux deux moteurs.
            MOTOR_SetSpeed(LEFT, vitesse); // La roue gauche avance.
            MOTOR_SetSpeed(RIGHT, -vitesse); // La roue droite recule.
        }
    }

    // Mettre à jour le compteur global currentMillis.
    currentMillis = millis();

    // Si le temps de rotation est écoulé.
    if (currentMillis - millisTourner >= 8000*degres/360) { // ex: 8 secondes pour un tour complet.
        // Arrêter de tourner.
        arreter();
    }
}