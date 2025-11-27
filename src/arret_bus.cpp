//
// Created by Adam on 2025-11-23.
//

#include "arret_bus.h"

#include "ecran.h"
#include "variables_globales.h"

int currentArret = 0, nextArret = 0;
Arret arrets[nbArret] = {
    Arret{"UDES", BLEU, false},
    Arret{"410-112", JAUNE, false},
    Arret{"Boul J-C", ROUGE, false},
    Arret{"Panneton", GRIS, false},
    Arret{"Roy", BRUN, false}
};
bool lumiereArret = false;

void LumiereArret_init() {
    pinMode(ledArretDemande,OUTPUT);
}

void updateLumiereArret() {
    if (!lumiereArret) {
        if (arrets[nextArret].isDemande) {
            digitalWrite(ledArretDemande,HIGH);
        }
    } else {
        if (!arrets[nextArret].isDemande) {
            digitalWrite(ledArretDemande,LOW);
        }
    }
}


bool isArret() {
    COULEURS couleur = COLOR_SENSOR_Read();
    for (int i = 0; i < nbArret; i++) {
        if (arrets[i].couleur == couleur) {
            currentArret = i;
            nextArret = (currentArret + 1) % nbArret;
#if CONSOLE_DEBUG
            Serial.print("[ARRET] lu arret : ");
            Serial.print(i);
            Serial.print(" = ");
            Serial.println(arrets[i].nom);
#endif


            return true;
        }
    }
    return false;
}

void afficherProchainArret() {
    affichage_ecran(arrets[nextArret].nom);
}

void ajouterArretDemande(int Id) {
    Id = Id % nbArret;

    arrets[Id].isDemande = true;
}
