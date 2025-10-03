/**
Projet: AutobUS
Equipe: P-12
Auteurs: Adam Turcotte
Description: Fonction easeInOut
Date: 3 octobre 2025
*/

#include "fonctionMath.h"

#include <Arduino.h>
#include <math.h>

double easeInOutCustom(const float x) {
    if (x < .1)
        return 0.7 * x + 0.1;
    return 1 - pow(1 - 0.7 * x, 3);
}

double easeInOutSin(const float x) {
    double in = 2 * PI * x - (PI / 2);
    return (sin(in) / 2) + 0.5;
}

double easeInOutGausse(const float x) {
    if (x < .4)
        return pow(2.71828, -pow(x - .4, 2) / (2 * pow(.37, 2)));
    if (x < .6)
        return 1;
    return pow(2.71828, -pow(x - .6, 2) / (2 * pow(.37, 2)));
}
