/**
 * @file Variables globales partagées entre tous les fichiers.
 * 
 * Le fichier .h fait que les variables peuvent être modifiées dans les fichiers.
 * Le fichier .h doit être inclus dans chaque fichier qui utilise ces variables.
 */

#ifndef VARIABLES_GLOBALES_H
#define VARIABLES_GLOBALES_H

#include "Arduino.h"
#include <Adafruit_TCS34725.h>

#define CONSOLE_DEBUG 1 // Pour activer les print dans la console, 0=non, 1=oui.

//Constantes qui sont spécifiques au différent robot.

#define  DiamDRobot  19.4
#define  DiamGRobot  19.4

#define PIN_BUTTON 49 // Pin du bouton pour demander l'arrêt à la prochaine station de bus.

#define ARRET_STATION_DELAY 10000 // Nombre de millisecondes que le robot est censé rester à un arrêt de bus.
constexpr long contourner_delay = 10000; // Temps que le robot attend avant de contourner l'obstacle;
constexpr long clignotant_delay = 300; // Temps entre les états du clignotant

// Constantes
constexpr float ppmsMax = 10; // pulse/ms max
constexpr int ppsMax = 4650 / 0.40; // pulse/s Max
constexpr float completionGoal = 0.98; //Pourcentage de completion nécessaire pour avoir fini le mouvement.
constexpr double cmToPulse = 3200 / (7.62 * PI); //Ratio qui converti les cm en pulse pour les moteurs

constexpr float rayonRobot = 9.75; // Rayon entre le centre et la roue du robot en cm
constexpr float degToCmGauche = (PI * DiamGRobot / 360); //Ratio qui converti les degrés en cm
constexpr float degToCmDroit = (PI * DiamDRobot / 360); //Ratio qui converti les degrés en cm

//distance entre l'obstacle et capteur

constexpr float DistanceObstacle = rayonRobot * 2;

constexpr int leds[4] = {10, 11, 12, 13}; //DEL {bleu, rouge, verte, jaune}
constexpr int ledsClignotant[4] = {39, 41, 40, 42}; // DEL {Av.G, Ar.G, Av.D, Ar.D}
constexpr int ledArretDemande = 43;
extern int numero_arret;

constexpr float VitesseSuivreLigne = 0.20;
constexpr float VitesseControunerObstacle = 0.15;
constexpr float distLigne = 21.02; // distance entre les deux lignes en cm
constexpr float distRoueSuiveur = 5.45; // distance entre la roue et le suiveur de ligne en cm
constexpr float rayonCourbe = 27.5; // Rayon de courbure des virages 90 du parcours
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
 * Classe pour utiliser le capteur de couleur.
 * Provient de la bibliothèque Adafruit_TCS34725.
 */
extern Adafruit_TCS34725 ColorSensor;

/**
 * Indique si le robot a terminé de contourner l'obstacle.
 * Vrai si le robot a terminé, faux sinon.
 */
extern bool isObstacleContourne;

#endif
