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
 * État actuel du robot.
 */
Etat currentEtat = ARRET; // Au début, le robot est à l'arrêt.

/**
 * État précédent du robot.
 */
Etat previousEtat = ARRET;

/**
 * Nombre de millisecondes depuis le démarrage du robot.
 * Utilisé pour les timers sans bloquer le programme.
 */
unsigned long currentMillis = 0;