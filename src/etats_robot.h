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
#include "arret.h"


#define ARRET_STATION_DELAY 5000 // Nombre de millisecondes que le robot est censé rester à un arrêt de bus.

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1   //   QT-PY / XIAO
extern Adafruit_SH1106G display; 

/**
 * Le robot contourne un obstacle détecté devant lui.
 * Ensuite, il retrouve la ligne noire par terre.
 * Enfin, il retourne à suivre la ligne.
 */
void etatContournerObstacle();

void affichage_ecran(Arret arret);

/**
 * Réaction lorsque le robot atteint une station de bus.
 * 
 * Le robot met à jour l'écran du nom du prochain arrêt.
 * Si l'arrêt à cette station a été demandé, il attend 5 secondes.
 * Sinon, il ne s'arrête pas (-> SUIVRE_LIGNE).
 */
void reagirStation();



#endif