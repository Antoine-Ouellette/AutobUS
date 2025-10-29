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

int losange = 0;
int Etat_mur = 0;

/**
 * Fonction pour seulement avancer jusqu'à retrouver la ligne.
 *
 * Le robot avance tout droit.
 * Si la ligne est détectée, il se tourne parallèle à la ligne.
 * L'état du robot est changé à SUIVRE_LIGNE pour continuer le défi.
 */
void retrouverLigne() {
}

/**
 * Action à faire lorsque la station de passage sans ligne est atteinte.
 *
 * Le robot avance jusqu'à ce que la ligne termine.
 * Le robot avance tout droit sur 75 cm.
 * Si la ligne n'est pas détectée, le robot va à droite sur max 30 cm.
 * Si la ligne n'est pas détectée, le robot va à gauche sur max 60 cm.
 * Si la ligne n'est pas détectée, le robot revient au milieu en allant 30 cm à
 * droite, puis avance sur max 30 cm. Si la ligne n'est pas détectée, le robot
 * va à droite sur max 30 cm. Si la ligne n'est pas détectée, le robot va à
 * gauche sur max 60 cm. Si la ligne n'est toujours pas détectée, le robot
 * recule et recommence.
 */
void suivreLigne() {
    float VITESSE_AVANCE = 0.2; // Vitesse d'avancement en ligne droite normale
    float VITESSE_CORRECTION_FAIBLE = VITESSE_AVANCE * 0.55; // Vitesse de correction pour retrouver la ligne
    float VITESSE_CORRECTION_ELEVEE = VITESSE_AVANCE * 0.55; // Vitesse de correction pour retrouver la ligne
    int depassement;
    int alignement;

    Serial.println("SUIVEUR: ");

    switch (SUIVEUR_Read()) {
        case 0b010: // centré sur la ligne
            Serial.println(" CENTRE: ");
            avancer(VITESSE_AVANCE);
            break;

        case 0b110: // légèrement à gauche
            Serial.println(" GAUCHE: ");
            MOTOR_SetSpeed(RIGHT, VITESSE_CORRECTION_FAIBLE);
            break;

        case 0b100: // beaucoup à gauche
            Serial.println(" FORT GAUCHE: ");
            MOTOR_SetSpeed(RIGHT, VITESSE_CORRECTION_ELEVEE);
            break;

        case 0b011: // légèrement à droite
            Serial.println(" DROITE: ");
            MOTOR_SetSpeed(LEFT, VITESSE_CORRECTION_FAIBLE);
            break;

        case 0b001: // beaucoup à droite
            Serial.println(" FORT DROITE: ");
            MOTOR_SetSpeed(LEFT, VITESSE_CORRECTION_ELEVEE);
            break;

        case 0b111: // Perpendiculaire à la ligne
            Serial.println(" PERPENDICULAIRE: ");
            arreter(); // prend une pause
            delay(100); // attend un peu pour stabiliser la lecture
            ENCODER_Reset(0); // Reset des encodeurs
            ENCODER_Reset(1);
            depassement = ENCODER_Read(0); // enregistre la distance dépassée
            ENCODER_Reset(0); // Reset des encodeurs
            ENCODER_Reset(1);

            alignement = depassement - (4.7 * cmToPulse); // Calcule l'alignement à faire après l'arrêt
            while (ENCODER_Read(0) <= alignement) {
                // Avance pour aligner le suiveur à la ligne une fois tourné
                avancer(0.05);
            }
            arreter();
            setGoal(0.1, TOUR_GAUCHE, 90); // Probléatique si vient du dessous

            break;

        case 0b000: // ligne perdue
            Serial.println(" PERDU: ");
            retrouverLigne();
            break;
    }
}

void avancerTrouverLigne() {
    const int distance = 80; // Distancce en cm à avancer
    uint8_t OutputSuiveur = SUIVEUR_Read();

    if (OutputSuiveur == 0b101) //A MODIFIER POUR 111 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    {
        // Détection de la ligne perpendiculaire
        arreter();        // prend une pause
        ENCODER_Reset(0); // Reset des encodeurs
        ENCODER_Reset(1);

        while (ENCODER_Read(0) <= distance * cmToPulse) {
            // Avance jusqu'à la distance spécifiée
            avancer(0.2);
        }

        if (ENCODER_Read(0) >= distance * cmToPulse) {
            // Vérification de la distance atteinte
            arreter();
        }

        suivreLigne();
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
void renverserQuille()
{
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
    uint16_t distanceSeuil = 20;
    switch (Etat_mur) {
        case 0: {
            setGoal(0.2, AVANCE, 40);
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
            setGoal(0.2, AVANCE, 50);
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
            currentEtat = SUIVRE_LIGNE;
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
        setGoal(0.2, AVANCE, 20);
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
            currentEtat = SUIVRE_LIGNE;
        }
        break;
    };
}
