/**
Projet: AutobUS
Equipe: P-12
Auteurs: Adam Turcotte
Description: Fonction easeInOut
Date: 2 octobre 2025
*/

#include "fonctionMath.h"
#include <math.h>

double easeInOutCustom(const float x) {
    if (x < .3)
        return 0.7 * x + 0.3;
    return 1 - pow(1 - 0.7, 3);
}
