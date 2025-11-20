//
// Created by Adam on 2025-10-24.
//

#ifndef AUTOBUS_ACTIONS_STATIONS_H
#define AUTOBUS_ACTIONS_STATIONS_H
#include "Arduino.h"

void CLIGNOTANT_init();

/**
 * Ajoute un clignotant sur le côté spécifié du robot.
 * @param cote Le côté du clignotant à ajouter (0/LEFT pour gauche, 1/RIGHT pour droite).
 */
void ajouteClignotant(int cote);

/**
 * Retire le clignotant actif sur le robot.
 */
void enleveClignotant();

/**
 * Met à jour l'état du clignotant.
 * Le fait clignoter à interval régulier.
 */
void updateClignotant();

void eteindreLEDsSuiveurLigne();

void suivreLigne(float VITESSE_AVANCE);

void contournerObstacle();

//void quatreClignotants();

#endif //AUTOBUS_ACTIONS_STATIONS_H
