/**
Projet: AutobUS
Equipe: P-12
Auteurs: Adam Turcotte
Description: Fonction easeInOut
Date: 7 octobre 2025
*/

#ifndef AUTOBUS_FONCTIONMATH_H
#define AUTOBUS_FONCTIONMATH_H

/**
 * Fonction qui permet de moduler la vitesse selon son avancement total sur le but.
 * Faible au début avec une vitesse maximal au milieu, et une vitesse plus faible vers la fin.
 * @param x Avancement dans le mouvement entre 0 et 1.
 * @return Multiplicateur de vitesse pour l'avancement en entrée.
 */
double easeInOutCustom(float x);

double easeInOutSin(float x);

double easeInOutCos(float x);

double easeInOutGausse(float x);

#endif //AUTOBUS_FONCTIONMATH_H
