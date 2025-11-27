//
// Created by Adam on 2025-11-24.
//

#ifndef AUTOBUS_MANETTE_IR_H
#define AUTOBUS_MANETTE_IR_H
#include "LibRobus.h"

constexpr int nbCode = 21;
// ## Pour que chaque bouton demande le prochain arret ##
//Code qui vient de la manette          +         1         2         3         4        5          6         7         8         9         0       100+      200+       -         EQ        |<<      >>|        >||       ch-       ch        ch+
constexpr uint32_t codes[nbCode] = {0xFFA857, 0xFF30CF, 0xFF18E7, 0xFF7A85, 0xFF10EF, 0xFF38C7, 0xFF5AA5, 0xFF42BD, 0xFF4AB5, 0xFF52AD, 0xFF6897, 0xFF9867, 0xFFB04F, 0xFFE01F, 0xFF906F, 0xFF22DD, 0xFF02FD, 0xFFC23D, 0xFFA25D, 0xFF629D, 0xFFE21D };
// ##


void reagirManetteIR();
#endif //AUTOBUS_MANETTE_IR_H
