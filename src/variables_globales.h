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
#define MODEL ROBOTB //changer entre A et B dépendant du robot utilisé

#if MODEL == ROBOTB
//Robot B
#define  DiamDRobot  19.4
#define  DiamGRobot  19.4
#endif

#if MODEL == ROBOTA
//Robot A
#define DiamDRobot 18.8
#define DiamGRobot 19.0
#endif

#define PIN_BUTTON A15 // Pin du bouton pour demander l'arrêt à la prochaine station de bus.

constexpr long contourner_delay = 10000; // Temps que le robot attend avant de contourner l'obstacle;
constexpr long clignotant_delay = 200; // Temps entre les états du clignotant

// Constantes
constexpr float ppmsMax = 10; // pulse/ms max
constexpr int ppsMax = 4650 / 0.40; // pulse/s Max
constexpr float completionGoal = 0.99; //Pourcentage de completion nécessaire pour avoir fini le mouvement.
constexpr double cmToPulse = 3200 / (7.62 * PI); //Ratio qui converti les cm en pulse pour les moteurs

constexpr float rayonRobot = 9.7; // Rayon entre le centre et la roue du robot en cm
constexpr float degToCmGauche = (PI * DiamGRobot / 360); //Ratio qui converti les degrés en cm
constexpr float degToCmDroit = (PI * DiamDRobot / 360); //Ratio qui converti les degrés en cm

//distance entre l'obstacle et capteur
constexpr float DistanceObstacle = rayonRobot * 2 + 5;

constexpr int leds[4] = {10, 11, 12, 13}; //DEL {bleu, rouge, verte, jaune}
constexpr int ledsClignotant[4]={}; // DEL {Av.G, Ar.G, Av.D, Ar.D}

extern int numero_arret;


/**
 * Variable pour savoir si le robot est en train de faire un déplacement
 */
extern bool isMoving;

/**
 * Valeurs possibles de l'état du robot.
 * ARRET: Le robot est à l'arrêt.
 * SUIVRE_LIGNE: Le robot avance et suit la ligne par terre.
 * CONTOURNER_OBSTACLE: Le robot contourne un obstacle, puis revient sur la ligne.
 * STATION_BUS: Le robot attend à la station de bus.
 */
enum Etat { ARRET, SUIVRE_LIGNE, CONTOURNER_OBSTACLE, STATION_BUS };

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
 * Quel était le temps quand le timer a débuté.
 * Sert à tracker le temps écoulé depuis le début du timer.
 */
extern unsigned long tempsDebutTimerEtatRobot;

/**
 * Quel était le temps quand le timer a débuté.
 * Sert à tracker le temps écoulé depuis le début du timer.
 */
extern unsigned long tempsDebutTimerContourner;

/**
 * Indique si le robot doit s'arrêter à la prochaine station de bus.
 */
extern bool isArreterProchaineStation;

/**
 * Indique si le robot a terminé de contourner l'obstacle.
 * Vrai si le robot a terminé, faux sinon.
 */
extern bool isObstacleContourne;

#endif
