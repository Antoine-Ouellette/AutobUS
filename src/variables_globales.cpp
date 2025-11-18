/**
 * @file Instanciation des variables globales et déclaration de leurs valeurs par défaut.
 * 
 * Les variables ici stockent les valeurs.
 * @warning Ne jamais inclure le fichier .cpp dans un autre.
 * Toujours inclure le fichier .h .
 * Le fichier .cpp n'a pas besoin d'être #include parce que le compilateur lie les 2 fichiers ensemble dans un .o .
 */

#include "variables_globales.h" // Récupérer la déclaration des variables globales.


/**
 * Variable pour savoir si le robot est en train de faire un déplacement
 */
bool isMoving = false;

/**
 * État actuel du robot.
 */
Etat currentEtat = ARRET; // Au début, le robot est à l'arrêt.

/**
 * État précédent du robot.
 */
Etat previousEtat = currentEtat;

/**
 * Nombre de millisecondes depuis le démarrage du robot.
 * Utilisé pour les timers sans bloquer le programme.
 */
unsigned long currentMillis = 0;

/**
 * Quel était le temps quand le timer a débuté.
 * Sert à tracker le temps écoulé depuis le début du timer.
 */
unsigned long tempsDebutTimerEtatRobot = 0;

unsigned long tempsDebutTimerContourner = 0;

/**
 * Indique si le robot doit s'arrêter à la prochaine station de bus.
 */
bool isArreterProchaineStation = false;

/**
 * Indique si le robot a terminé de contourner l'obstacle.
 * Vrai si le robot a terminé, faux sinon.
 */
bool isObstacleContourne = false;