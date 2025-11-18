/**
 * @file Définie une fonction pour chaque station du défi du combattant.
 * Ce sont les procédures à faire lorsque la station est rencontrée.
 */

#include "actions.h"
#include <LibRobus.h>           // Essentielle pour utiliser RobUS.
#include "variables_globales.h" // Inclure les variables globales partagées entre tous les fichiers.
#include "moteur.h"             // Inclure les fonctions en lien avec les moteurs des roues.
#include "capteurs/suiveur_ligne.h"
#include "capteurs/detecteur_IR.h"

int Etat_mur = 0;
int Etat_retrouver = 4;


int suivre_ligne_retroaction = 0; // mémoire de la dernière direction prise (negative = droite, positive = gauche)
bool startedFollow = false;

void eteindreLedsSuiveur() {
    digitalWrite(10, LOW);
    digitalWrite(11, LOW);
    digitalWrite(12, LOW);
    digitalWrite(13, LOW);
}
// Fonction pour seulement avancer jusqu'à retrouver la ligne.

/**
 * Fonction pour seulement avancer jusqu'à retrouver la ligne.
 *
 * Le robot avance tout droit.
 * Si la ligne est détectée, il se tourne parallèle à la ligne.
 * L'état du robot est changé à SUIVRE_LIGNE pour continuer le défi.
 */
void retrouverLigne() {
/*    const float distanceDroite = 30;
    const float distanceGauche = 60;
    const float vitesse = 0.3;

    switch (Etat_retrouver) {
        case 4:
            mouvementMoteurs(vitesse, TOUR_DROIT, 30);
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
            mouvementMoteurs(vitesse, AVANCE, distanceDroite);
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
            mouvementMoteurs(vitesse, TOUR_GAUCHE, 90);
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
            mouvementMoteurs(vitesse, AVANCE, distanceGauche);
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
            mouvementMoteurs(vitesse, TOUR_GAUCHE, 30);
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
            mouvementMoteurs(vitesse, TOUR_DROIT, 50);
            Etat_retrouver = 17;
            break;
        case 17:
            if (isGoal()) {
                Etat_retrouver = 4;
                startedFollow = false;
                currentEtat = SUIVRE_LIGNE;
            }
    }*/
}

void suivreLigne(float VITESSE_AVANCE) {
    float VITESSE_CORRECTION = VITESSE_AVANCE * 0.5; // Vitesse de correction pour retrouver la ligne
    float VITESSE_BANG = VITESSE_AVANCE * 1.2; // Vitesse pour le coup de virage quand la ligne est perdue 
    if (!startedFollow) {
        suivre_ligne_retroaction = 0;
        startedFollow = true;
    }

    // uint8_t result = SUIVEUR_Read();
    // for (int i = 0; i < 3; i++)
    // {
    //     digitalWrite(leds[i], (result & (1 << i)) ? HIGH : LOW);
    // }

    // Combine les deux suiveurs de ligne sous un nombre binaire
    uint8_t combinaisonSensors = SUIVEUR_Read(LEFT) << 3 | (SUIVEUR_Read(RIGHT));
    const int delaiArret = 2000;
    double timerArret = 0;

    // // Si combinaisonSensor contien 0b100000, le resee des bits on sen fout 
    // if (combinaisonSensors & 0b100000)
    // {

    // }

    
    //si 1, ligne est détectée, si 0, plancher
    switch (combinaisonSensors) {
        case 0b000001:
        case 0b100001: // centré entre les lignes
            eteindreLedsSuiveur();
            digitalWrite(10, HIGH);
            digitalWrite(11, HIGH);
            
            avancer(VITESSE_AVANCE);
            suivre_ligne_retroaction = 0;
            break;

        case 0b000010: // corrige à gauche
            eteindreLedsSuiveur();
            digitalWrite(12, HIGH);
            
            MOTOR_SetSpeed(LEFT, VITESSE_AVANCE);
            MOTOR_SetSpeed(RIGHT, VITESSE_CORRECTION);
            suivre_ligne_retroaction++;
            break;

        case 0b010000: // corrige à droite
            eteindreLedsSuiveur();
            digitalWrite(13, HIGH);
            
            MOTOR_SetSpeed(LEFT, VITESSE_CORRECTION);
            MOTOR_SetSpeed(RIGHT, VITESSE_AVANCE);
            suivre_ligne_retroaction--;
            break;

/*        case 0b000001: // virage à Gauche
            eteindreLedsSuiveur();
            digitalWrite(11, HIGH);
            
            mouvementMoteurs(VITESSE_CORRECTION, TOUR_GAUCHE, 90, rayonRobot+4);
            suivre_ligne_retroaction++;
            break;

        case 0b100000: // virage à Droite
            eteindreLedsSuiveur();
            digitalWrite(10, HIGH);
            
            mouvementMoteurs(VITESSE_CORRECTION, TOUR_DROIT, 90, rayonRobot);
            suivre_ligne_retroaction--;
            break;
*/            
        case 0b111111: //Ligne d'arrêt
            eteindreLedsSuiveur();
            digitalWrite(10, HIGH);
            digitalWrite(11, HIGH);
            digitalWrite(12, HIGH);
            digitalWrite(13, HIGH);
            
            if (!timerArret) {
                suivre_ligne_retroaction = 0;
                arreter();
                timerArret = millis();
            } else if (millis() - timerArret >= delaiArret) {
                // Après le délai, avancer de nouveau
                timerArret = 0; // Réinitialiser le timer
                mouvementMoteurs(VITESSE_AVANCE*0.4, AVANCE, distLigne);
            }
            break;
        
        case 0b000000: // ligne perdue
            eteindreLedsSuiveur();
            
            if (suivre_ligne_retroaction > 0) { //dernière direction prise était à gauche
                MOTOR_SetSpeed(LEFT, VITESSE_BANG);
            } 
            else if (suivre_ligne_retroaction < 0) { //dernière direction prise était à droite
                MOTOR_SetSpeed(RIGHT, VITESSE_BANG);
            }
            break;
        
        default:
            eteindreLedsSuiveur();
            
            avancer(VITESSE_AVANCE);
            break;
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
            mouvementMoteurs(0.2, AVANCE, 100);
            Etat_mur = 1;
            break;
        }
        case 1: // Le robot avance jusqu'à ce que le capteur ultrason détecte le mur à une distance de ... cm.
        {
            float distanceActuelle = IR_ReadDistanceCm(FRONT);
            if (distanceActuelle <= distanceSeuil) {
                arreter();
                Etat_mur = 2;
            }
            break;
        }

        case 2: // Avancer plus loin que l'obstacle
        {
            mouvementMoteurs(0.2, TOUR_DROIT, 90);
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
            mouvementMoteurs(0.2, AVANCE, 40);
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
            mouvementMoteurs(0.2, TOUR_GAUCHE, 90);
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
            mouvementMoteurs(0.2, AVANCE, 60);
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
            mouvementMoteurs(0.2, TOUR_GAUCHE, 90);
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
            mouvementMoteurs(0.2, AVANCE, 40);
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
            mouvementMoteurs(0.2, TOUR_DROIT, 90);
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
            mouvementMoteurs(0.3);
            break;
        }
    }
}


