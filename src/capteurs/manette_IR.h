//
// Created by Adam on 2025-11-24.
//

#ifndef AUTOBUS_MANETTE_IR_H
#define AUTOBUS_MANETTE_IR_H
#include "LibRobus.h"

constexpr int nbCode = 6;
//Code qui vient de la manette
constexpr uint32_t codes[nbCode] = {0xFFA857, 0xFF30CF, 0xFF18E7, 0xFF7A85, 0xFF10EF, 0xFF38C7};
// Équivalent pour l'arrêt, -1 = prochain arret
constexpr int equivArret[nbCode] = {-1, 0, 1, 2, 3, 4};


void reagirManetteIR();
#endif //AUTOBUS_MANETTE_IR_H
