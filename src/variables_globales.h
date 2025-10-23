/**
 * @file Variables globales partagées entre tous les fichiers.
 * 
 * Le fichier .h fait que les variables peuvent être modifiées dans les fichiers.
 * Le fichier .h doit être inclus dans chaque fichier qui utilise ces variables.
 */

#ifndef VARIABLES_GLOBALES_H
#define VARIABLES_GLOBALES_H

// Constantes

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
 * Indique si le robot doit s'arrêter à la prochaine station de bus.
 */
extern bool isArreterProchaineStation;

/**
 * Indique si le robot a terminé de contourner l'obstacle.
 * Vrai si le robot a terminé, faux sinon.
 */
extern bool isObstacleContourne;

#endif