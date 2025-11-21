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

/**
 * Faire suivre la ligne au robot en utilisant les capteurs suiveurs de ligne.
 * Utilise une logique de type bang-bang pour corriger la trajectoire.
 *
 * @param VITESSE_AVANCE La vitesse à laquelle le robot doit avancer en suivant la ligne.
 */

void suivreLigne(float VITESSE_AVANCE) {
    float VITESSE_CORRECTION = VITESSE_AVANCE * 0.4; // Vitesse de correction pour retrouver la ligne
    float VITESSE_BANG = VITESSE_AVANCE * 1.2; // Vitesse pour le coup de virage quand la ligne est perdue

    if (!startedFollow) {
        suivre_ligne_retroaction = 0;
        startedFollow = true;
    }
/*  //Protection si l'autobus est en sens inverse
    void checkCrossDetection() {
    int rawLeft0  = analogRead(pins[0]);
    int rawRight0 = analogRead(pins[3]);

    // Does right sensor see something that would count as LEFT line?
    if (detectUsingThreshold(seuil_ligneGauche, incertitude_SL_Gauche, rawRight0)||(detectUsingThreshold(seuil_ligneDroit, incertitude_SL_Droit, rawLeft0)) {
        Serial.println("Right sensor matches LEFT line threshold!"){
        ajouteClignotant(LEFT);
        ajouteClignotant(RIGHT);
        mouvementMoteurs(VITESSE_AVANCE, TOUR_GAUCHE, 180);
        }
*/
    // Combine les deux suiveurs de ligne sous un nombre binaire
    uint8_t combinaisonSensors = SUIVEUR_Read(LEFT) << 3 | (SUIVEUR_Read(RIGHT));
    const int delaiArret = 2000;
    static int etapeArret = 0;  // Étape actuelle dans la séquence d'arrêt
    static unsigned long timerArret = 0;

    Serial.println(combinaisonSensors, BIN);
    return;
    
    //si 1, ligne est détectée, si 0, plancher
    switch (combinaisonSensors) {
        case 0b000001:
        case 0b100000:
        case 0b100001: // centré entre les lignes
            eteindreLedsSuiveur();
            digitalWrite(10, HIGH);
            digitalWrite(11, HIGH);

            avancer(VITESSE_AVANCE);
            suivre_ligne_retroaction = 0;
            break;

        case 0b001111:
        case 0b111100:
        case 0b111110:
        case 0b011111:
        case 0b111000:
        case 0b000111:
        case 0b111111: //Ligne d'arrêt
            eteindreLedsSuiveur();
            digitalWrite(10, HIGH);
            digitalWrite(11, HIGH);
            digitalWrite(12, HIGH);
            digitalWrite(13, HIGH);

            switch (etapeArret) {
                case 0://arrête
                    arreter();
                    timerArret = millis();
                    etapeArret++;
                    break;

                case 1: //attend pour faire son stop
                    if (millis() - timerArret >= delaiArret) {
                        // Après le délai, avancer de nouveau
                        timerArret = 0; // Réinitialiser le timer
                        etapeArret++;
                    }
                    break;

                case 2: //traverse l'intersection
                    mouvementMoteurs((VITESSE_AVANCE*0.85), AVANCE, distLigne);
                    etapeArret++;
                    break;

                case 3:  //retrouve sa ligne
                    uint8_t suivGauche = SUIVEUR_Read(LEFT);
                    uint8_t suivDroit = SUIVEUR_Read(RIGHT);
                    uint8_t comb = (suivGauche << 3) | suivDroit;

                    if (comb == 0b000000) {
                        avancer(VITESSE_AVANCE);
                    } else {
                        etapeArret = 0;
                    }
                    break;
            }
            break;

        case 0b010000: // corrige à gauche
            eteindreLedsSuiveur();
            digitalWrite(12, HIGH);

            MOTOR_SetSpeed(LEFT, VITESSE_AVANCE);
            MOTOR_SetSpeed(RIGHT, VITESSE_CORRECTION);
            suivre_ligne_retroaction++;
            break;

        case 0b000010: // corrige à droite
            eteindreLedsSuiveur();
            digitalWrite(13, HIGH);

            MOTOR_SetSpeed(LEFT, VITESSE_CORRECTION);
            MOTOR_SetSpeed(RIGHT, VITESSE_AVANCE);
            suivre_ligne_retroaction--;
            break;

        case 0b100011:
        case 0b100111:
        case 0b000011: // virage à Gauche
            eteindreLedsSuiveur();
            digitalWrite(11, HIGH);

            mouvementMoteurs(VITESSE_AVANCE, TOUR_DROIT, 90, rayonRobot + ajustVirage);
            suivre_ligne_retroaction++;
            break;

        case 0b110001:
        case 0b111001:
        case 0b110000: // virage à Droite
            eteindreLedsSuiveur();
            digitalWrite(10, HIGH);

            mouvementMoteurs(VITESSE_AVANCE, TOUR_GAUCHE, 90, rayonRobot + ajustVirage);
            suivre_ligne_retroaction--;
            break;

            case 0b000000:
            case 0b001000:
            case 0b000100: // ligne perdue
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


