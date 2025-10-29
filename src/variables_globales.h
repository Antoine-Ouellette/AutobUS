/**
 * @file Variables globales partagées entre tous les fichiers.
 * 
 * Le fichier .h fait que les variables peuvent être modifiées dans les fichiers.
 * Le fichier .h doit être inclus dans chaque fichier qui utilise ces variables.
 */

#ifndef VARIABLES_GLOBALES_H
#define VARIABLES_GLOBALES_H

#include "Arduino.h"

//Constantes qui sont spécifiques au différent robot.
#define ROBOTA 1
#define ROBOTB 2
#define MODEL ROBOTA //changer entre A et B dépendant du robot utilisé

#if MODEL == ROBOTB
//Robot B
#define  DiamDRobot  19.6
#define  DiamGRobot  19.4
#endif

#if MODEL == ROBOTA
//Robot A
#define DiamDRobot 18.8
#define DiamGRobot 19.0
#endif


// Constantes
constexpr float ppmsMax = 10; // pulse/ms max
constexpr int ppsMax = 4650 / 0.40; // pulse/s Max
constexpr float completionGoal = 0.99; //Pourcentage de completion nécessaire pour avoir fini le mouvement.
constexpr double cmToPulse = 3200 / (7.62 * PI); //Ratio qui converti les cm en pulse pour les moteurs

constexpr float degToCmGauche = (PI * DiamGRobot / 360); //Ratio qui converti les degrés en cm
constexpr float degToCmDroit = (PI * DiamDRobot / 360); //Ratio qui converti les degrés en cm

constexpr int  leds[4] = {10, 11, 12, 13}; //DEL {bleu, rouge, verte, jaune}


/**
 * Valeurs possibles de l'état du robot.
 * ARRET: Le robot est à l'arrêt.
 * SUIVRE_LIGNE: Le robot avance et suit la ligne par terre.
 * CONTOURNER_OBSTACLE: Le robot contourne un obstacle.
 * QUILLE: Le robot trouve et renverse la quille.
 * DANSE: Le robot fait la danse en forme de losange.
 * PAS_LIGNE: Le robot avance tout droit et essaie de retrouver la ligne.
 */
enum Etat { ARRET, SUIVRE_LIGNE, CONTOURNER_OBSTACLE, QUILLE, DANSE, PAS_LIGNE };

// Variables globales partagées entre tous les fichiers

/**
 * État actuel du robot.
 */
extern Etat currentEtat;

/**
 * État précédent du robot.
 */
extern Etat previousEtat;

/**
 * Nombre de millisecondes depuis le démarrage du robot.
 * Utilisé pour les timers sans bloquer le programme.
 */
extern unsigned long currentMillis;

/**
 * Variable pour savoir si le robot est en train de faire un déplacement
 */
extern bool isMoving;

#endif
