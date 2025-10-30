/**
 * @file Fonctions en lien avec les moteurs des roues du robot.
 * Inclut les déplacements et le PID.
 */

#include "moteur.h"
#include <LibRobus.h> // Essentielle pour utiliser RobUS.

#include "actions_stations.h"
#include "variables_globales.h" // Inclure les variables globales partagées entre tous les fichiers.
#include "suiveur_ligne.h"


// ####### Le moteur gauche est le moteur master #### //
// Variable des paramètres des moteurs
float ramp_duration = 3000; // Temps d'accélération du début du mouvement.
float initialSpeed = 0.15; // Vitesse au début du mouvement.
float vitesseGoal = 0; //Vitesse voulue par les moteurs
int side[2] = {0, 0}; // Coté de la rotation des roues 1, avant -1 arriere.
float distGoal = 0; // Distance voulu pour chaque moteurs, tout mouvement est symétrique, on se fie au gauche.

enum MODE { LIGNE, AUTONOME };

MODE mode_PID = LIGNE; // Mode du PID pour faire l'ajustement.

// Constantes PID
// PID {LEFT, RIGHT}
float k_p[2] = {0.002, 0.002}; // Facteur p
float k_i[2] = {0.002, 0.002}; // Facteur i
float k_d[2] = {0.00005, 0.00005}; // Facteur d
long current_error_PID[2] = {0, 0}; // ERREUR actuel du PID
long error_previous_PID[2] = {0, 0}; // Erreur de la fois précédente
long integral_PID[2] = {0, 0}; // Intégral utilisé dans le PID
float derivat_PID[2] = {0, 0}; // Dérivé du PID.
float old_output_PID[2] = {0, 0};
unsigned long millisLastPID = 0; // Temps du dernier ajustement
int32_t pulseTotalSum[2] = {0, 0}; // Distance total que chaque moteur devrai avoir fait;
float encCompletion[2] = {0, 0}; // Pourcentage de completion des encodeurs.

void ENCODERS_Reset() {
    ENCODER_Reset(LEFT);
    ENCODER_Reset(RIGHT);
}

void resetPIDStats() {
    ENCODERS_Reset();
    vitesseGoal = 0;
    for (int i = 0; i < 2; i++) {
        current_error_PID[i] = 0;
        error_previous_PID[i] = 0;
        integral_PID[i] = 0;
        derivat_PID[i] = 0;
        old_output_PID[i] = 0;
        pulseTotalSum[i] = 0;
        encCompletion[i] = 0;
    }
}

void setGoal(const float vitesse, const MOUVEMENT mouvement, const float distance) {
    isMoving = true;
    resetPIDStats();
    //Définir le temps de la dernière mesure, qui va être maintenant.
    millisLastPID = millis();

    if (distance > 0) mode_PID = AUTONOME;
    else mode_PID = LIGNE;

    if (mode_PID == AUTONOME) {
        switch (mouvement) {
            case TOUR_GAUCHE:
                side[LEFT] = -1;
                side[RIGHT] = 1;
                vitesseGoal = vitesse; // La roue droite avance.
                distGoal = distance * degToCmGauche * cmToPulse;
                break;
            case TOUR_DROIT:
                // Appliquer des vitesses opposées aux deux moteurs.
                side[LEFT] = 1;
                side[RIGHT] = -1;
                vitesseGoal = vitesse; // La roue gauche avance.
                distGoal = distance * degToCmDroit * cmToPulse;
                break;
            case AVANCE:
                // Appliquer une vitesse aux deux moteurs du robot.
                side[LEFT] = 1;
                side[RIGHT] = 1;
                vitesseGoal = vitesse;
                distGoal = distance * cmToPulse;
                break;
            case RECULE:
                // Appliquer une vitesse aux deux moteurs du robot.
                side[LEFT] = -1;
                side[RIGHT] = -1;
                vitesseGoal = vitesse;
                distGoal = -distance * cmToPulse;
                break;
        }
    } else {
        // TODO on se fie a William
        return;
    }
}

void calcEncCompletion() {
    const int32_t encLeft = ENCODER_Read(LEFT);
    const int32_t encRight = ENCODER_Read(RIGHT);
    encCompletion[LEFT] = max(encLeft / distGoal, 0);
    // On fait le max pour être sûr de ne pas avoir de négatif.
    encCompletion[RIGHT] = max(encRight / distGoal, 0); // Même chose que l'autre ^
}

bool isGoal() {
    if (mode_PID == LIGNE) return false;

    calcEncCompletion();

    // Si le but est atteint
    if (encCompletion[LEFT] > completionGoal || encCompletion[RIGHT] > completionGoal) {
        isMoving = false;
        arreter();
        return true;
    }

    return false;
}

void ajusteVitesse() {
    const unsigned long currentTime = millis();
    const float delta = max(currentTime - millisLastPID, 1) / 1000.0; // En seconde
    millisLastPID = currentTime;
    float vitesseReel = vitesseGoal;

    //Si on suit la ligne, on veut toujours avoir à avancer
    if (mode_PID == LIGNE) {
        //TODO : Appel la fonction de will?
        suivreLigne();
        return;
    }

    calcEncCompletion();

    // if (abs(distGoal) > 15 * cmToPulse) {
    //     int32_t distLEFT = ENCODER_Read(LEFT) * side[LEFT];
    //     if (abs(distLEFT) < ramp_duration) {
    //         // Linear ramp from 0.20 to vitesseGoal
    //         float ratio = (float) abs(distLEFT) / ramp_duration; // 0 → 1
    //         vitesseReel = initialSpeed + ratio * (vitesseGoal - initialSpeed);
    //     }
    //     // Peut décélérer en fait, si la fin est trop proche du début.
    //     if (abs(distGoal - distLEFT) < ramp_duration) {
    //         float ratio = abs(distGoal - distLEFT) / ramp_duration; // 0 → 1
    //         vitesseReel = initialSpeed + ratio * (vitesseReel - initialSpeed);
    //     }
    // }

    //Donne le nb de pulse qui devrait être fait depuis le début du mouvement.
    pulseTotalSum[LEFT] += vitesseReel * ppsMax * delta * side[LEFT];
    pulseTotalSum[RIGHT] += vitesseReel * ppsMax * delta * side[RIGHT];

    current_error_PID[LEFT] = pulseTotalSum[LEFT] - ENCODER_Read(LEFT);
    current_error_PID[RIGHT] = pulseTotalSum[RIGHT] - ENCODER_Read(RIGHT);


    for (int i = 0; i < 2; i++) {
        integral_PID[i] += current_error_PID[i] * delta;
        integral_PID[i] = constrain(integral_PID[i], -1000, 1000);

        if (current_error_PID[i] && error_previous_PID[i]) {
            derivat_PID[i] = float(current_error_PID[i] - error_previous_PID[i]) / delta;
        } else
            derivat_PID[i] = 0;
        error_previous_PID[i] = current_error_PID[i];

        float output_PID = k_p[i] * current_error_PID[i] +
                           k_i[i] * integral_PID[i] +
                           k_d[i] * derivat_PID[i];

        if (old_output_PID[i] * output_PID < 0) {
            output_PID = 0;
            delay(30);
        }

        output_PID = constrain(output_PID, -1, 1);
        old_output_PID[i] = output_PID;

        MOTOR_SetSpeed(i, output_PID);
        //
        // Serial.print(" P:");
        // Serial.print(k_p[i] * current_error_PID[i]);
        // Serial.print(" I:");
        // Serial.print(k_i[i] * integral_PID[i]);
        // Serial.print(" D:");
        // Serial.print(k_d[i] * derivat_PID[i]);
        // Serial.print(" goalS:");
        // Serial.print(pulseTotalSum[i]);
        // Serial.print(" Speed:");
        // Serial.print(ENCODER_Read(i));
        // Serial.print(" distGoal:");
        // Serial.print(distGoal);
    }
    // Serial.println("");
}


// void ajustementPIDLigne() {
//     switch (SUIVEUR_Read()) {
//         case 0b000:
//             //Tu es mort.. tu a perdu la ligne
//             // TODO : que faire quand il perd la ligne
//             break;
//         case 0b001:
//             // Tu es rendu trop à gauche
//             vitesseGoal[RIGHT] = vitesseBase[RIGHT] * 0.5;
//             ajustementPID();
//             break;
//         case 0b011:
//             // Tu es trop à gauche, mais pas tant
//             vitesseGoal[RIGHT] = vitesseBase[RIGHT] * 0.7;
//             ajustementPID();
//             break;
//         case 0b010:
//             // Tu avance droit, good job
//             vitesseGoal[LEFT] = vitesseBase[LEFT];
//             vitesseGoal[RIGHT] = vitesseBase[RIGHT];
//             ajustementPID();
//             break;
//         case 0b110:
//             // Tu es trop à droite, mais pas tant
//             vitesseGoal[LEFT] = vitesseBase[LEFT] * 0.7;
//             ajustementPID();
//             break;
//         case 0b100:
//             // Tu es rendu trop à droite
//             vitesseGoal[LEFT] = vitesseBase[LEFT] * 0.5;
//             ajustementPID();
//             break;
//         case 0b111:
//             // Tu est rendu sur une ligne!?!
//             // TODO : on fait quoi sur une ligne horizontale?
//             break;
//     }
// }


void arreter() {
    isMoving = false;

    // Mettre les vitesses des deux moteurs à 0%.
    MOTOR_SetSpeed(LEFT, 0);
    MOTOR_SetSpeed(RIGHT, 0);
}

void avancer(float speed) {
    MOTOR_SetSpeed(LEFT, speed);
    MOTOR_SetSpeed(RIGHT, speed);
}
