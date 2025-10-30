/**
 * @file Définie une fonction pour chaque station du défi du combattant.
 * Ce sont les procédures à faire lorsque la station est rencontrée.
 */

#include "actions_stations.h"
#include <LibRobus.h>           // Essentielle pour utiliser RobUS.
#include "variables_globales.h" // Inclure les variables globales partagées entre tous les fichiers.
#include "moteur.h"             // Inclure les fonctions en lien avec les moteurs des roues.
#include "detecteur_IR.h"
#include "suiveur_ligne.h" // Inclure les fonctions en lien avec le suiveur de ligne.

int Etat_mur = 0;
int Etat_avance_trouve = 0;
int Etat_retrouver = 4;
int losange = 0;
// Variable pour la quille
float degrer;
float distance;

enum Etat_quille {
    etat_avance_au_centre_quille,
    etat_attente_centre_quille,
    etat_tours,
    etat_recherche_quille,
    etat_avanceQuille,
    etat_attente_davancer,
    etat_reculer,
    etat_attente_deRevenir,
    etat_revenir,
    etat_attente_revenir,
    etat_avance_carre,
    etat_attente_fin
};

Etat_quille etat_quille = etat_avance_au_centre_quille;

int i_suivre_ligne = 0; // mémoire de la dernière direction prise
bool startedFollow = false;

// Fonction pour seulement avancer jusqu'à retrouver la ligne.

/**
 * Fonction pour seulement avancer jusqu'à retrouver la ligne.
 *
 * Le robot avance tout droit.
 * Si la ligne est détectée, il se tourne parallèle à la ligne.
 * L'état du robot est changé à SUIVRE_LIGNE pour continuer le défi.
 */
void retrouverLigne() {
    const float distanceDroite = 30;
    const float distanceGauche = 60;
    const float vitesse = 0.3;

    switch (Etat_retrouver) {
        case 4:
            setGoal(vitesse, TOUR_DROIT, 30);
            Etat_retrouver = 5;
            break;

        case 5:
            if (SUIVEUR_Read() > 0) {
                Etat_retrouver = 12;
                break;
            }
            if (isGoal()) {
                Etat_retrouver = 6;
            }
            break;

        case 6:
            setGoal(vitesse, AVANCE, distanceDroite);
            Etat_retrouver = 7;
            break;

        case 7:
            if (SUIVEUR_Read() > 0) {
                Etat_retrouver = 12;
                break;
            }
            if (isGoal()) {
                Etat_retrouver = 8;
            }
            break;

        case 8:
            setGoal(vitesse, TOUR_GAUCHE, 90);
            Etat_retrouver = 9;
            break;

        case 9:
            if (SUIVEUR_Read() > 0) {
                Etat_retrouver = 16;
                break;
            }
            if (isGoal()) {
                Etat_retrouver = 10;
            }
            break;

        case 10:
            setGoal(vitesse, AVANCE, distanceGauche);
            Etat_retrouver = 11;
            break;

        case 11:
            if (SUIVEUR_Read() > 0) {
                Etat_retrouver = 16;
                break;
            }
            if (isGoal()) {
                Etat_retrouver = 4;
            }
            break;

        // Si on voit à partir de la droite
        case 12:
            setGoal(vitesse, TOUR_GAUCHE, 30);
            Etat_retrouver = 13;
            break;
        case 13:
            if (isGoal()) {
                Etat_retrouver = 4;
                startedFollow = false;
                currentEtat = SUIVRE_LIGNE;
            }

        // SI on voit à partir de la gauche
        case 16:
            setGoal(vitesse, TOUR_DROIT, 50);
            Etat_retrouver = 17;
            break;
        case 17:
            if (isGoal()) {
                Etat_retrouver = 4;
                startedFollow = false;
                currentEtat = SUIVRE_LIGNE;
            }
    }
}

void suivreLigne() {
    float VITESSE_AVANCE = 0.25; // Vitesse d'avancement en ligne droite normale
    float VITESSE_CORRECTION_ELEVEE = VITESSE_AVANCE * 0.2; // Vitesse de correction pour retrouver la ligne

    Serial.println("SUIVEUR: ");

    if (!startedFollow) {
        i_suivre_ligne = 0;
        startedFollow = true;
    }

    // uint8_t result = SUIVEUR_Read();
    // for (int i = 0; i < 3; i++)
    // {
    //     digitalWrite(leds[i], (result & (1 << i)) ? HIGH : LOW);
    // }


    switch (SUIVEUR_Read()) {
        case 0b010: // centré sur la ligne
            Serial.println(" CENTRE: ");
            avancer(VITESSE_AVANCE);
            i_suivre_ligne = 0;
            break;

        case 0b100: // corrige à gauche
            Serial.println(" FORT GAUCHE: ");
            MOTOR_SetSpeed(LEFT, VITESSE_AVANCE);
            MOTOR_SetSpeed(RIGHT, VITESSE_CORRECTION_ELEVEE);
            i_suivre_ligne++;
            break;

        case 0b001: // corrige à droite
            Serial.println(" FORT DROITE: ");
            MOTOR_SetSpeed(LEFT, VITESSE_CORRECTION_ELEVEE);
            MOTOR_SetSpeed(RIGHT, VITESSE_AVANCE);
            i_suivre_ligne--;
            break;

        case 0b000: // ligne perdue
            Serial.println(" PERDU: ");
            if (i_suivre_ligne >= 0) {
                MOTOR_SetSpeed(RIGHT, 0.35);
            } else if (i_suivre_ligne <= 0) {
                MOTOR_SetSpeed(LEFT, 0.35);
            }
            break;
    }
}

/**
 * Action à faire lorsque la station de passage sans ligne est atteinte.
 *
 * Le robot avance jusqu'à ce que la ligne termine.
 * Le robot avance tout droit sur 75 cm.
 * Si la ligne n'est pas détectée, le robot va à droite sur max 30 cm.
 * Si la ligne n'est pas détectée, le robot va à gauche sur max 60 cm.
 * Si la ligne n'est pas détectée, le robot revient au milieu en allant 30 cm à droite,
 * puis avance sur max 30 cm.
 * Si la ligne n'est pas détectée, le robot va à droite sur max 30 cm.
 * Si la ligne n'est pas détectée, le robot va à gauche sur max 60 cm.
 * Si la ligne n'est toujours pas détectée, le robot recule et recommence.
 */
void avancerTrouverLigne() {
    const int distance = 120; // Distancce en cm à avancer
    uint8_t OutputSuiveur = SUIVEUR_Read();

    switch (Etat_avance_trouve) {
        case 0:
            setGoal(0.3, AVANCE, distance);
            Etat_avance_trouve++;
            break;

        case 1:
            if (isGoal()) {
                Etat_avance_trouve++;
            }
            break;

        case 2:
            if (OutputSuiveur == 0b000) {
                currentEtat = RETROUVER_LIGNE;
            } else {
                currentEtat = SUIVRE_LIGNE;
            }
            break;
    }
}

/**
 * Action à faire lorsque la station de la quille est atteinte.
 *
 * Le robot doit trouver la quille. Il fait un tour à 360 degrés et utilise
 * le capteur à ultrasons pour la détecter.
 * Le robot enregistre à quel angle il va partir.
 * Le robot avance dans la direction de la quille jusqu'à ce que la valeur de la distance
 * soit très proche.
 * Le robot fonce dans la quille.
 * Le robot recule un peu pour laisser de l'espace.
 * Le robot tourne pour faire face à la ligne (en utilisant le nombre de degrés tourné).
 * Le robot avance jusqu'à retrouver la ligne (utilise la méthode retrouverLigne()).
 * L'état du robot est changé à SUIVRE_LIGNE pour avancer jusqu'au prochain défi.
 */
void renverserQuille() {
    switch (etat_quille) {
        case etat_avance_au_centre_quille:
            setGoal(0.1, AVANCE, 7);
            etat_quille = etat_attente_centre_quille;
            break;

        case etat_attente_centre_quille:
            if (isGoal()) {
                etat_quille = etat_tours;
            }
            break;

        case etat_tours:
            setGoal(0.1, TOUR_GAUCHE, 360);
            etat_quille = etat_recherche_quille;
            break;

        case etat_recherche_quille:
            distance = lireDistance_quille();
            if (distance < 35) {
                arreter();
                degrer = lireDistance_roue();
                etat_quille = etat_avanceQuille;
            }
            break;

        case etat_avanceQuille:
            setGoal(0.3, AVANCE, distance + 10);
            etat_quille = etat_attente_davancer;
            break;
        case etat_attente_davancer:
            if (isGoal()) {
                etat_quille = etat_reculer;
            }
            break;

        case etat_reculer:
            setGoal(0.3, RECULE, distance + 10);
            etat_quille = etat_attente_deRevenir;
            break;

        case etat_attente_deRevenir:
            if (isGoal()) {
                etat_quille = etat_revenir;
            }
            break;

        case etat_revenir:
            setGoal(0.3, TOUR_DROIT, degrer);
            etat_quille = etat_attente_revenir;
            break;

        case etat_attente_revenir:
            if (isGoal()) {
                etat_quille = etat_avance_carre;
            }
            break;

        case etat_avance_carre:
            setGoal(0.3, AVANCE, 15);
            etat_quille = etat_attente_fin;

            break;

        case etat_attente_fin:
            if (isGoal()) {
                etat_quille = etat_avance_au_centre_quille;
                currentEtat = RETROUVER_LIGNE;
                break;
            }
    }
}

/**
 * Action à faire lorsque la station de contournement d'un obstacle est atteinte.
 *
 * Le robot avance jusqu'à ce que le capteur ultrason détecte le mur à une distance de ... cm.
 * Le robot tourne de 90 degrés à droite.
 * Le robot avance de ... cm.
 * Le robot tourne de 90 degrés à gauche.
 * Le robot avance jusqu'à retrouver la ligne (utilise la méthode retrouverLigne()).
 * L'état du robot est changé à SUIVRE_LIGNE pour avancer jusqu'au prochain défi.
 */
void contournerObstacle() {
    uint16_t distanceSeuil = 25;
    switch (Etat_mur) {
        case 0: {
            setGoal(0.2, AVANCE, 100);
            Etat_mur = 1;
            break;
        }
        case 1: // Le robot avance jusqu'à ce que le capteur ultrason détecte le mur à une distance de ... cm.
        {
            float distanceActuelle = lireDistance_quille();
            if (distanceActuelle <= distanceSeuil) {
                arreter();
                Etat_mur = 2;
            }
            break;
        }

        case 2: // Avancer plus loin que l'obstacle
        {
            setGoal(0.2, TOUR_DROIT, 90);
            Etat_mur = 3;
            break;
        }

        case 3: {
            if (isGoal()) {
                Etat_mur = 4;
            }
            break;
        }
        case 4: {
            setGoal(0.2, AVANCE, 40);
            Etat_mur = 5;
            break;
        }
        case 5: {
            if (isGoal()) {
                Etat_mur = 6;
            }
            break;
        }
        case 6: {
            setGoal(0.2, TOUR_GAUCHE, 90);
            Etat_mur = 7;
            break;
        }

        case 7: {
            if (isGoal()) {
                Etat_mur = 8;
            }
            break;
        }
        case 8: {
            setGoal(0.2, AVANCE, 60);
            Etat_mur = 9;
            break;
        }
        case 9: {
            if (isGoal()) {
                Etat_mur = 10;
            }
            break;
        }
        case 10: {
            setGoal(0.2, TOUR_GAUCHE, 90);
            Etat_mur = 11;
            break;
        }

        case 11: {
            if (isGoal()) {
                Etat_mur = 12;
            }
            break;
        }
        case 12: {
            setGoal(0.2, AVANCE, 40);
            Etat_mur = 13;
            break;
        }
        case 13: {
            if (isGoal()) {
                Etat_mur = 14;
            }
            break;
        }
        case 14: {
            setGoal(0.2, TOUR_DROIT, 90);
            Etat_mur = 15;
            break;
        }

        case 15: {
            if (isGoal()) {
                Etat_mur = 16;
            }
            break;
        }
        case 16: {
            Etat_mur = 0;
            currentEtat = RETROUVER_LIGNE;
            break;
        }
    }
}

/**
 * Action à faire lorsque la station de danse est atteinte.
 *
 * Le robot tourne de 135 degrés à gauche.
 * Le robot avance de ... cm.
 * Le robot tourne de 90 degrés à gauche.
 * Le robot avance de ... cm.
 * Le robot tourne de 90 degrés à gauche.
 * Le robot avance de ... cm.
 * Le robot tourne de 90 degrés à gauche.
 * Le robot avance de ... cm.
 * Le robot tourne de 45 degrés à gauche pour faire face à la ligne.
 * Le robot avance jusqu'à retrouver la ligne (utilise la méthode retrouverLigne()).
 * (Le robot recule s'il n'a pas retrouvé la ligne.)
 * Le robot tourne de 90 degrés à droite pour être parallèle à la ligne.
 * L'état du robot est changé à SUIVRE_LIGNE pour avancer jusqu'au prochain défi.
 */
void danserLosange() {
    switch (losange) {
        case 0:
            setGoal(0.2, AVANCE, 40);
            losange++;
            break;

        case 1:
            if (isGoal()) {
                losange++;
            }
            break;

        case 2:
            setGoal(0.2, TOUR_DROIT, 45);
            losange++;
            break;

        case 3:
            if (isGoal()) {
                losange++;
            }
            break;

        case 4:
            setGoal(0.2, RECULE, 20);
            losange++;
            break;

        case 5:
            if (isGoal()) {
                losange++;
            }
            break;

        case 6:
            setGoal(0.2, TOUR_GAUCHE, 90);
            losange++;
            break;

        case 7:
            if (isGoal()) {
                losange++;
            }
            break;

        case 8:
            setGoal(0.2, RECULE, 20);
            losange++;
            break;

        case 9:
            if (isGoal()) {
                losange++;
            }
            break;

        case 10:
            setGoal(0.2, TOUR_DROIT, 90);
            losange++;
            break;

        case 11:
            if (isGoal()) {
                losange++;
            }
            break;

        case 12:
            setGoal(0.2, AVANCE, 20);
            losange++;
            break;

        case 13:
            if (isGoal()) {
                losange++;
            }
            break;

        case 14:
            setGoal(0.2, TOUR_GAUCHE, 90);
            losange++;
            break;

        case 15:
            if (isGoal()) {
                losange++;
            }
            break;

        case 16:
            setGoal(0.2, AVANCE, 20);
            losange++;
            break;

        case 17:
            if (isGoal()) {
                losange++;
            }
            break;

        case 18:
            setGoal(0.2, TOUR_DROIT, 45);
            losange++;
            break;

        case 19:
            if (isGoal()) {
                losange = 0;
                currentEtat = RETROUVER_LIGNE;
            }
            break;
    };
}
