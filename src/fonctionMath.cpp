/**
Projet: AutobUS
Equipe: P-12
Auteurs: Adam Turcotte
Description: Fonction easeInOut
Date: 7 octobre 2025
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

double easeInOutCos(float x) {
    return (-cos(PI * 2 * x) / 2.5) + (1 / 2.2);
}

double easeInOutGausse(const float x) {
    const float a = 0.70;
    const float b = 0.40;
    const float c = 0.17;
    const float d = 0.25;
    if (x < b)
        return pow(.9 * 2.71828, -pow(x - .3, 2) / (1 * pow(0.126, 2))) + 1 - a;
    if (x < b + d)
        return 1;
    return pow(a * 2.71828, -pow(x - (b + d), 2) / (1 * pow(c, 2))) + 1 - a;
}
