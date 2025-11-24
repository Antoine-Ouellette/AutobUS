//
// Created by Adam on 2025-11-23.
//

#ifndef AUTOBUS_ARRET_BUS_H
#define AUTOBUS_ARRET_BUS_H

#include "capteurs/detecteur_couleur.h"

// Struct qui définie les arrets
typedef struct {
    char nom[30];
    COULEURS couleur;
    bool isDemande;
} Arret;

constexpr int nbArret = 5;

extern int currentArret, nextArret;
extern Arret arrets[];
extern bool lumiereArret;

/**
 * Initialise la pin pour la lumière indicatrice del'arret
 */
void LumiereArret_init();

/**
 * Met à jour l'état de la lumière d'arrêt demandé
 */
void updateLumiereArret();

/**
 * Vérifie si le bus est actuellement à un arrêt.
 *
 * @return `true` si le bus est à un arrêt, sinon `false`.
 */
bool isArret();

/**
 * Affiche le prochain arret sur l'écran
 */
void afficherProchainArret();

/**
 * Ajouter un arret demandé, pour arrêter quand on va être rendu.
 * @param Id numéro de l'arret
 */
void ajouterArretDemande(int Id);


#endif //AUTOBUS_ARRET_BUS_H
