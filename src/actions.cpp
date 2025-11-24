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


unsigned long lastClignote = 0;
bool clignoteG, clignoteGauche, clignoteD,clignoteDroit;

void CLIGNOTANTS_init()
{
    clignoteG = false;
    clignoteGauche = false;
    clignoteD = false;
    clignoteDroit = false;

    for (int i = 0; i < 4; i++){
        pinMode(ledsClignotant[i],OUTPUT);
    }
}

void ajouteClignotant(const int cote) {
    if (cote == RIGHT) clignoteD = true;
    else if (cote == LEFT) clignoteG = true;
}

void enleveClignotant() {
    clignoteG = false;
    clignoteGauche = false;
    clignoteD = false;
    clignoteDroit = false;
}

void updateClignotant() {
    if (millis() < lastClignote + clignotant_delay) return;

    if (clignoteG) {
        for (int i = 0; i < 2; i++) {
            digitalWrite(ledsClignotant[i], clignoteGauche ? HIGH : LOW);
        }
    }
    if (clignoteD) {
        for (int i = 2; i < 4; i++) {
            digitalWrite(ledsClignotant[i], clignoteDroit ? HIGH : LOW);
        }
    }

    if (clignoteG)
        clignoteGauche = !clignoteGauche;
    if (clignoteD)
        clignoteDroit = !clignoteDroit;
    lastClignote = millis();
}

void quatreClignotants()
{
    ajouteClignotant(LEFT);
    ajouteClignotant(RIGHT);
}


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

    // Combine les deux suiveurs de ligne sous un nombre binaire
    uint8_t combinaisonSensors = SUIVEUR_Read(LEFT) << 3 | (SUIVEUR_Read(RIGHT));
   const int delaiArret = 2000;

    //Serial.println(combinaisonSensors, BIN);
    //return;

    //si 1, ligne est détectée, si 0, plancher
    switch (combinaisonSensors) {
        
        // centré entre les lignes
        case 0b000001:
        case 0b100000:
        case 0b100001:
            eteindreLedsSuiveur();
            avancer(VITESSE_AVANCE);
            suivre_ligne_retroaction = 0;
            break;

         //Ligne d'arrêt
        case 0b001111:
        case 0b111100:
        case 0b111110:
        case 0b111111:
        case 0b000111:
        case 0b100111:
        case 0b111000:
        case 0b111001:
            Serial.print("LIGNE ARRET DETECTE :");
            Serial.println(combinaisonSensors, BIN);
            Serial.println("[ARRET]");
            arreter();
            while (SUIVEUR_Read(LEFT) != 0b111 && SUIVEUR_Read(RIGHT) != 0b111) {
                Serial.println("[ALLIGNEMENT] : ");
                Serial.println(combinaisonSensors, BIN);
                while (SUIVEUR_Read(LEFT) != 0b111) {
                    MOTOR_SetSpeed(LEFT, VITESSE_CORRECTION);
                    MOTOR_SetSpeed(RIGHT, 0);
                }
                while (SUIVEUR_Read(RIGHT) != 0b111) {
                    MOTOR_SetSpeed(RIGHT, VITESSE_CORRECTION);
                    MOTOR_SetSpeed(LEFT, 0);

                }
            } 
            digitalWrite(10, HIGH);
            Serial.println("[DELAY]");
            delay(delaiArret);
            Serial.println("[FIN DELAY]");
            Serial.println("[TRAVERSE INTERSECTION]");
            mouvementMoteurs((VITESSE_AVANCE*0.85), AVANCE, distLigne+7);
            break;

        // corrige à gauche
        case 0b010000:
            Serial.println("CORRECTION GAUCHE");
            MOTOR_SetSpeed(LEFT, VITESSE_AVANCE);
            MOTOR_SetSpeed(RIGHT, VITESSE_CORRECTION);
            suivre_ligne_retroaction++;
            Serial.print("RETROACTION CORRECTION :");
            Serial.println(suivre_ligne_retroaction);
            break;

         // corrige à droite
        case 0b000010:
            Serial.println("CORRECTION DROITE");
            MOTOR_SetSpeed(LEFT, VITESSE_CORRECTION);
            MOTOR_SetSpeed(RIGHT, VITESSE_AVANCE);
            suivre_ligne_retroaction--;
            Serial.print("RETROACTION CORRECTION : ");
            Serial.println(suivre_ligne_retroaction);
            break;

        // virage
        case 0b110010:
        case 0b110011:
        case 0b100011:
            Serial.print("VIRAGE DETECTE : ");
            Serial.println(combinaisonSensors, BIN);
            Serial.println("[VIRAGE]");            
            mouvementMoteurs(VITESSE_AVANCE, TOUR_GAUCHE, 90, rayonRobot+ajustVirage);
           break;

        // décentré de la ligne
        case 0b000000:
            Serial.print("RETROACTION EXECUTION :");
            Serial.println(suivre_ligne_retroaction);

            eteindreLedsSuiveur();
            if (suivre_ligne_retroaction > 0) { //dernière direction prise était à gauche
                MOTOR_SetSpeed(LEFT, VITESSE_BANG);
            }
            else if (suivre_ligne_retroaction < 0) { //dernière direction prise était à droite
                MOTOR_SetSpeed(RIGHT, VITESSE_BANG);
            }
            break;

        case 0b000100:
            Serial.print("RETROACTION EXECUTION 000100 :");
            Serial.println(suivre_ligne_retroaction);

            eteindreLedsSuiveur();
            if (suivre_ligne_retroaction > 0) { //dernière direction prise était à gauche
                MOTOR_SetSpeed(LEFT, VITESSE_BANG);
            }
            else if (suivre_ligne_retroaction < 0) { //dernière direction prise était à droite
                MOTOR_SetSpeed(RIGHT, VITESSE_BANG);
            }
            else if (suivre_ligne_retroaction == 0) {
                Serial.print("PAS RETROACTION, RECENTRER");
                MOTOR_SetSpeed(LEFT, VITESSE_BANG);
            }
            break;

        case 0b001000:
            Serial.print("RETROACTION EXECUTION 001000 :");
            Serial.println(suivre_ligne_retroaction);

            eteindreLedsSuiveur();
            if (suivre_ligne_retroaction > 0) { //dernière direction prise était à gauche
                MOTOR_SetSpeed(LEFT, VITESSE_BANG);
            }
            else if (suivre_ligne_retroaction < 0) { //dernière direction prise était à droite
                MOTOR_SetSpeed(RIGHT, VITESSE_BANG);
            }
            else if (suivre_ligne_retroaction == 0) {
             Serial.print("PAS RETROACTION, RECENTRER");
                MOTOR_SetSpeed(LEFT, VITESSE_BANG);
            }
            break;


        default:
            Serial.print("DEFAULT : ");
            Serial.println(combinaisonSensors, BIN);
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

void contournerObstacle()
{
    switch (Etat_mur)
    {
    case 0:
    {
        mouvementMoteurs(0.25, AVANCE, 100);
        Etat_mur = 1;
        break;
    }
    case 1: // Le robot avance jusqu'à ce que le capteur ultrason détecte le mur à une distance de ... cm.
    {
        if (IR_ReadDistanceCm(FRONT) <= DistanceObstacle)
        {
            Etat_mur = 2;
        }
        break;
    }

    case 2: // Avancer plus loin que l'obstacle
    {
        mouvementMoteurs(0.25, TOUR_GAUCHE, 90);
        quatreClignotants();
        Etat_mur = 3;
        break;
    }

    case 3:
    {

        if (isGoal())
        {
            Etat_mur = 4;
        }
        break;
    }
    case 4:
    {
        mouvementMoteurs(0.25, AVANCE, 100);
        Etat_mur = 5;
        break;
    }
    case 5:
    {
    
        if (IR_ReadDistanceCm(RIGHT) > DistanceObstacle + 5)
        {
            mouvementMoteurs(0.3, TOUR_DROIT, 90, DistanceObstacle);
            Etat_mur = 7;
        }
        break;
    }
    case 7:
    {
        if (isGoal())
        {
            Etat_mur = 8;
        }
        break;
    }
    case 8:
    {
        mouvementMoteurs(0.25, AVANCE, 100);
        Etat_mur = 9;
        break;
    }
    case 9:
    {
        if (IR_ReadDistanceCm(RIGHT) > DistanceObstacle + 5)
        {
            mouvementMoteurs(0.3, TOUR_DROIT, 90, DistanceObstacle);
            Etat_mur = 11;
        }
        break;
    }

    case 11:
    {
        if (isGoal())
        {
            Etat_mur = 12;
        }
        break;
    }
    case 12:
    {
        mouvementMoteurs(0.25, AVANCE, 100);
        Etat_mur = 13;
        break;
    }
    case 13:
    {
        if (SUIVEUR_Read(0) == 0b111 && SUIVEUR_Read(1) == 0b111)
        {
            mouvementMoteurs(0.3, TOUR_GAUCHE, 90, rayonRobot);
            Etat_mur = 15;
        }
        break;
    }

    case 15:
    {
        if (isGoal())
        {
            Etat_mur = 16;
        }
        break;
    }
    case 16:
    {
        enleveClignotant();
        Etat_mur = 0;
        mouvementMoteurs(0.3, SUIVRE_LA_LIGNE);
        break;
    }
    };
}
