//
// Created by Adam on 2025-11-24.
//

#ifndef AUTOBUS_ECRAN_H
#define AUTOBUS_ECRAN_H

#include <Adafruit_SH110X.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1   //   QT-PY / XIAO
extern Adafruit_SH1106G display;

void ECRAN_init();

void affichage_ecran(char text[]);

#endif //AUTOBUS_ECRAN_H