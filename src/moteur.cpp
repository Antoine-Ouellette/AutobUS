/**
Projet: AutobUS
Equipe: P-12
Auteurs: Adam Turcotte
Description: fonction de contrôle des moteurs et des encodeurs
Date: 2 octobre 2025
*/

#include <LibRobus.h>
#include "variablesGlobales.h"
#include "constantes.h"
#include "fonctionMath.h"
#include "moteur.h"


//Variables moteurs
float vitesseReel = 0.40; //Vitesse réelle selon la fonction smooth
int side[2] = {1, 1}; //Sense dans lequel la roue tourne -1 = arriere, 1 = avant
float mult_v_g = 1; //Multiplicateur de vitesse gauche
float mult_v_d = 1; //Multiplicateur de vitesse droit

//Encodeurs
int encoderRightGoal = 0; //Objectif de pulse de l'encodeur droit
int encoderLeftGoal = 0; //Objectif de pulse de l'encodeur gauche
double encoderRightCompletion = 1.0; //Pourcentage de completion de l'objectif de pulse
double encoderLeftCompletion = 1.0; //Pourcentage de completion de l'objectif de pulse

/**
 * Vérifie si les moteurs ont atteint le nb de tour voulut
 * @return true : si le but est atteint \n
           false : sinon
 */
bool isGoal() {
    if (encoderRightGoal != 0 || encoderLeftGoal != 0) {
        encoderRightCompletion = ENCODER_Read(RIGHT) / (double) encoderRightGoal;
        encoderLeftCompletion = ENCODER_Read(LEFT) / (double) encoderLeftGoal;

        if (encoderRightCompletion > 0.97 || encoderLeftCompletion > 0.97) {
            return true;
        }
    }
    return false;
}

void resetGoals() {
    encoderLeftGoal = 0;
    encoderRightGoal = 0;
}

void resetEncoders() {
    ENCODER_Reset(LEFT);
    ENCODER_Reset(RIGHT);
}

void calculateGoals() {
    switch (currentEtat) {
        case 1: //Avance
            side[LEFT] = 1;
            side[RIGHT] = 1;
            encoderRightGoal = (int) (l_case * cmToPulse * nbCaseAvance);
            encoderLeftGoal = encoderRightGoal;
            break;

        case 2: //Tourne droite
            side[LEFT] = 1;
            side[RIGHT] = -1;
            encoderLeftGoal = int(angle * degToCm * cmToPulse);
            encoderRightGoal = -int(angle * degToCm * cmToPulse);
            break;

        case 3: //Tourne gauche
            side[LEFT] = -1;
            side[RIGHT] = 1;
            encoderLeftGoal = -int(angle * degToCm * cmToPulse);
            encoderRightGoal = int(angle * degToCm * cmToPulse);
            break;

        case 4: //Faire un 180
            side[LEFT] = -1;
            side[RIGHT] = 1;
            encoderLeftGoal = -int(180 * degToCm * cmToPulse);
            encoderRightGoal = int(180 * degToCm * cmToPulse);
            break;

        default:
            break;
    }
}

void effectueDeplacement() {
    MOTOR_SetSpeed(RIGHT, vitesseReel * mult_v_d * side[RIGHT]);
    MOTOR_SetSpeed(LEFT, vitesseReel * mult_v_g * side[LEFT]);
}

void arret() {
    MOTOR_SetSpeed(RIGHT, 0);
    MOTOR_SetSpeed(LEFT, 0);
};

void ajusteVitesse() {
    const unsigned long currentTime = millis();
    if (!ENCODER_Read(RIGHT) || !ENCODER_Read(LEFT))return;

    if (encoderLeftCompletion < 0) encoderLeftCompletion = 0;
    if (encoderRightCompletion < 0) encoderRightCompletion = 0;

    vitesseReel = vitesseMax * easeInOutCustom(encoderRightCompletion);

    //Donne le nb de pulse qui devrait être fait depuis le début du mouvement.
    double supposedCount = vitesseReel * ppmMax * (currentTime - startMillis);

    //Calcule le ratio d'ajustement nécessaire pour chaque moteur.
    mult_v_d = supposedCount / (ENCODER_Read(RIGHT) * side[RIGHT]);
    mult_v_g = supposedCount / (ENCODER_Read(LEFT) * side[LEFT]);


    //Si un moteur est rendu plus loin que l'autre ajuste rend l'autre un peu plus vite.
    if (encoderLeftCompletion > 0 && encoderRightCompletion > 0) {
        if (encoderLeftCompletion < encoderRightCompletion) {
            mult_v_g *= (encoderRightCompletion / encoderLeftCompletion) * 1.05;
        } else if (encoderRightCompletion < encoderLeftCompletion) {
            mult_v_d *= (encoderLeftCompletion / encoderRightCompletion) * 1.05;
        }
    }

    //Si le multiplicateur est rendu négatif remet le positif pour éviter des erreurs.
    if (mult_v_d < 0) mult_v_d *= -1;
    if (mult_v_g < 0) mult_v_g *= -1;

    //Si une des vitesses est plus grande que 1, baisser les deux vitesses an gardant le même ratio.
    if (mult_v_d > 1 || mult_v_g > 1) {
        if (mult_v_d > mult_v_g) {
            mult_v_g /= mult_v_d;
            mult_v_d = 1;
        } else {
            mult_v_d /= mult_v_g;
            mult_v_g = 1;
        }
    }
}
