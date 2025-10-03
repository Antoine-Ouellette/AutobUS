/**
Projet: AutobUS
Equipe: P-12
Auteurs: Antoine Ouellette, Adam Turcotte
Description: Constante nécessaire pour
Date: 3 octobre 2025
*/
#ifndef AUTOBUS_CONSTANTES_H
#define AUTOBUS_CONSTANTES_H

#include <Arduino.h>
#define VISITE 0b10000 // Masque binaire qui défini une case visitée.
#define NORD 0b01000   // Masque binaire qui défini un mur au nord.
#define EST 0b00100    // Masque binaire qui défini un mur à l'est.
#define SUD 0b00010    // Masque binaire qui défini un mur au sud.
#define OUEST 0b00001  // Masque binaire qui défini un mur à l'ouest.

/**
 * Vitesse maximale à laquelle le robot pourra avancer.
 * C'est le pourcentage de la vitesse maximale.
 * Doit être entre 0 et 1. ex: 0.5 = 50%.
 */
constexpr float vitesseMax  = .5;

/**
 * Nombre de pulse par millis seconde maximal que le moteur le plus lent est capable d'atteindre.
 */
constexpr float ppmMax = 5.55;

constexpr int l_case = 50; //Longueur de la case en cm
constexpr double cmToPulse = 3200 / (7.62 * PI); //Ratio qui converti les cm en pulse pour les moteurs
constexpr double degToCm = PI * 19.5 / 360; //Ratio qui converti les degrés en cm
constexpr float angle = 90; //Angle pour tourner (degré)

constexpr int pinVert = 48;
constexpr int pinRouge = 49;


#endif //AUTOBUS_CONSTANTES_H
