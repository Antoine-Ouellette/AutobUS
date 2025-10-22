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

#endif