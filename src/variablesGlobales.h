/**
Projet: AutobUS
Equipe: P-12
Auteurs: Antoine Ouellette, Adam Turcotte
Description: Contient les variables globales du projet
Date: 2 octobre 2025
*/

#ifndef AUTOBUS_VARIABLESGLOBALES_H
#define AUTOBUS_VARIABLESGLOBALES_H

/**
 * Valeurs possibles de l'état du robot.
 * ARRET: Le robot est à l'arrêt.
 * AVANCER: Le robot avance.
 * TOURNER_DROITE: Le robot tourne à droite.
 * TOURNER_GAUCHE: Le robot tourne à gauche.
 * TOURNER_180: Le robot tourne de 180° vers la gauche.
 * PRISE_DECISION: Le robot décide le prochain mouvement à effectuer.
 */
enum Etat { ARRET, AVANCER, TOURNER_DROITE, TOURNER_GAUCHE, TOURNER_180, PRISE_DECISION };

// État actuel du robot.
Etat currentEtat = ARRET; // Au départ, le robot est à l'arrêt.

// État du robot dans l'itération précédente de loop().
Etat previousEtat = ARRET;

// Nombre de case que le robot doit avancer durant le prochain déplacement.
int nbCaseAvance = 0;

//Temps en millis seconde au début du mouvement (Etat).
unsigned long startMillis = 0;

#endif //AUTOBUS_VARIABLESGLOBALES_H
