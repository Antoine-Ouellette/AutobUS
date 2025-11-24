#ifndef ARRET_H
#define ARRET_H
#define nb_arret 6
#include "capteurs/detecteur_couleur.h"

// Un arret de bus
typedef struct {
    char nom_arret[30];
    COULEURS couleur[10];
}Arret;

extern Arret arrets[];













#endif // ARRET_H