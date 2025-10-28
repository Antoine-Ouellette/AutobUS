/**
 * @file Fonctions en lien avec les moteurs des roues du robot.
 * Inclut les déplacements et le PID.
 */

#include "moteur.h"
#include <LibRobus.h> // Essentielle pour utiliser RobUS.
#include "variables_globales.h" // Inclure les variables globales partagées entre tous les fichiers.

// Variable pour le timer de rotation.
unsigned long millisTourner = 0;


enum MODE { LIGNE, AUTONOME };

MODE mode_PID = LIGNE; // Mode du PID pour faire l'ajustement.

// Constantes PID pour avancer sans la ligne.
// PID {Gauche, Droit}
int k_p[2] = {1, 1}; // Facteur p
int k_i[2] = {0, 0}; // Facteur i
int k_d[2] = {0, 0}; // Facteur d
int error_PID[2] = {0, 0}; // Erreur a corrigé
int error_previous_PID[2] = {0, 0}; // Erreur de la fois précédente
int output_PID[2] = {0, 0}; // Vitesse réelle pour les deux moteurs
unsigned long millisLastPID[2] = {0, 0}; // Temps du dernier ajustement


void setGoal(int vitesse, MOUVEMENT mouvement, int distance) {
    //TODO : coder les goals en pulse pour les moteurs
}


void ajustementPIDAutonome() {
    //TODO : (par adam) Implementer la formule du PID
}

void ajustementPIDLigne() {
    // TODO : (par adam) Implémenter la fonction pour ajuster selon la ligne vu
}


void arreter() {
    // Mettre les vitesses des deux moteurs à 0%.
    MOTOR_SetSpeed(LEFT, 0);
    MOTOR_SetSpeed(RIGHT, 0);
}


void avancer(float vitesse) {
    // Appliquer une vitesse aux deux moteurs du robot.
    MOTOR_SetSpeed(LEFT, vitesse);
    MOTOR_SetSpeed(RIGHT, vitesse);
}


void reculer(float vitesse) {
    // Appliquer une vitesse aux deux moteurs du robot.
    MOTOR_SetSpeed(LEFT, -vitesse);
    MOTOR_SetSpeed(RIGHT, -vitesse);
}

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
    if (currentMillis - millisTourner >= 8000 * degres / 360) {
        // ex: 8 secondes pour un tour complet.
        // Arrêter de tourner.
        arreter();
    }
}

void ajusteVitesse() {
    if (mode_PID == LIGNE) {
        ajustementPIDLigne();
    } else {
        ajustementPIDAutonome();
    }
}
