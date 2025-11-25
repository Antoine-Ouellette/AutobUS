//
// Created by Adam on 2025-11-24.
//

#include "manette_IR.h"

#include "arret_bus.h"
#include "ecran.h"
#include "variables_globales.h"

void reagirManetteIR() {
    uint32_t code = REMOTE_read();

    // Pas de code, ne rien faire.
    if (code == 0) return;

    // For debug
    Serial.println(code,HEX);

    for (uint32_t i = 0; i < nbCode; i++) {
        if (codes[i] == code) {
            if (equivArret[i] == -1) {
                ajouterArretDemande(nextArret);
                return;
            } else {
                ajouterArretDemande(equivArret[i]);
                return;
            }
        }
    }
}
