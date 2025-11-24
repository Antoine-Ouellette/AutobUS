//
// Created by Adam on 2025-11-24.
//

#include "ecran.h"
#include <Fonts/FreeSans12pt7b.h>

Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void ECRAN_init() {
    display.begin(0x3c, true); // Address 0x3C for 128x64
    display.setFont(&FreeSans12pt7b);
}

void affichage_ecran(char text[]) {
    display.clearDisplay();
    display.setTextSize(1); // Normal 1:1 pixel scale
    display.setTextColor(SH110X_WHITE); // Draw white text
    display.setCursor(0, 28); // Start at top-left corner
    display.println(text);
    display.display();
}
