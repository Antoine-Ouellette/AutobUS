/**
Projet: AutobUS
Equipe: P-12
Auteurs: Antoine Ouellette, Adam Turcotte
Description: Constante nécessaire pour
Date: 7 octobre 2025
*/
#ifndef AUTOBUS_CONSTANTES_H
#define AUTOBUS_CONSTANTES_H

#include <Arduino.h>
#define VISITE 0b10000 // Masque binaire qui défini une case visitée.
#define NORD 0b01000   // Masque binaire qui défini un mur au nord.
#define EST 0b00100    // Masque binaire qui défini un mur à l'est.
#define SUD 0b00010    // Masque binaire qui défini un mur au sud.
#define OUEST 0b00001  // Masque binaire qui défini un mur à l'ouest.

//Constantes qui sont spécifiques au différent robot.
#define ROBOTA 1
#define ROBOTB 2
#define MODEL ROBOTB //changer entre A et B dépendant du robot utilisé

#if MODEL == ROBOTB
//Robot B
#define  DiamDRobot  19.6
#define  DiamGRobot  19.4
#endif

#if MODEL == ROBOTA
//Robot A
#define DiamDRobot 18.7
#define DiamGRobot 19.0
#endif


/**
 * Vitesse maximale à laquelle le robot pourra avancer.
 * C'est le pourcentage de la vitesse maximale.
 * Doit être entre 0 et 1. ex: 0.5 = 50%.
 */
constexpr float vitesseMax = .7;
constexpr float completionGoal = 0.99;

/**
 * Nombre de pulse par millis seconde maximal que le moteur le plus lent est capable d'atteindre.
 */
constexpr float ppmMax = 5.5;

constexpr int l_case = 50; //Longueur de la case en cm
constexpr double cmToPulse = 3200 / (7.62 * PI); //Ratio qui converti les cm en pulse pour les moteurs

constexpr float degToCmGauche = (PI * DiamGRobot / 360); //Ratio qui converti les degrés en cm
constexpr float degToCmDroit = (PI * DiamDRobot / 360); //Ratio qui converti les degrés en cm

constexpr float angle = 90; //Angle pour tourner (degré)

constexpr int pinVert = 48;
constexpr int pinRouge = 49;


#endif //AUTOBUS_CONSTANTES_H
