/**
 * @file Une fonction pour chaque état possible du robot.
 * Exécute les actions associées à chaque état.
 */
#ifndef ETATS_ROBOT_H
#define ETATS_ROBOT_H 
/******************************************************************************
Variables et #define
accessibles seulement dans ce fichier.
******************************************************************************/
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include "arret_bus.h"


/**
 * Réaction lorsque le robot atteint une station de bus.
 * 
 * Le robot met à jour l'écran du nom du prochain arrêt.
 * Si l'arrêt à cette station a été demandé, il attend 5 secondes.
 * Sinon, il ne s'arrête pas (-> SUIVRE_LIGNE).
 */
void reagirStation();
#endif