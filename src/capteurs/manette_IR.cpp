//
// Created by Adam on 2025-11-24.
//

#include "manette_IR.h"
#include "arret_bus.h"
#include "variables_globales.h"

void reagirManetteIR() {
    uint32_t code = REMOTE_read();

    // Pas de code, ne rien faire.
    if (code == 0) return;

    // For debug
    #if CONSOLE_DEBUG
        Serial.println(code,HEX);
    #endif

    for (uint32_t i = 0; i < nbCode; i++) {
        if (codes[i] == code) {
            isArreterProchaineStation = true;
            // ajouterArretDemande(nextArret);
#if CONSOLE_DEBUG
            Serial.print("[MANETTE] : prochain arret demande = ");
            Serial.println(nextArret);
#endif
            return;
        }
    }
}
