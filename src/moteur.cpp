/**
 * @file Fonctions en lien avec les moteurs des roues du robot.
 * Inclut les déplacements et le PID.
 */

#include "moteur.h"

#include <LibRobus.h> // Essentielle pour utiliser RobUS.
#include "variables_globales.h" // Inclure les variables globales partagées entre tous les fichiers.
#include "actions.h"


// ####### Le moteur gauche est le moteur master #### //
// Variable des paramètres des moteurs
float ramp_duration = 3000; // Temps d'accélération du début du mouvement.
float initialSpeed = 0.15; // Vitesse au début du mouvement.
int side[2] = {0, 0}; // Coté de la rotation des roues 1, avant -1 arriere.
float GoalVitesse[2] = {0, 0}; //Vitesse voulue par les moteurs
float GoalDistance[2] = {0, 0}; // Distance voulue pour chaque moteur.

enum MODE { LIGNE, AUTONOME }; // Mode de fonctionnement du PID en suivre une ligne et être autonome.

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

    for (int i = 0; i < 2; i++) {
        GoalVitesse[i] = 0;
        current_error_PID[i] = 0;
        error_previous_PID[i] = 0;
        integral_PID[i] = 0;
        derivat_PID[i] = 0;
        old_output_PID[i] = 0;
        pulseTotalSum[i] = 0;
        encCompletion[i] = 0;
    }
}

void mouvementMoteurs(const float vitesse, const MOUVEMENT mouvement, const float distance, const float rayon) {
    isMoving = true;
    resetPIDStats();
    //Définir le temps de la dernière mesure, qui va être maintenant.
    millisLastPID = millis();


    switch (mouvement) {
        case TOUR_GAUCHE:
            // TODO : Ajuster pour le décentrage
            side[RIGHT] = 1;
            GoalVitesse[RIGHT] = vitesse;
            GoalDistance[RIGHT] = (distance * (PI * (rayon + (DiamGRobot/2)) / 180)) * cmToPulse;

            if (rayon <= (DiamGRobot/2)) {
                side[LEFT] = -1;
                GoalDistance[LEFT] = (distance * (PI * ((DiamGRobot/2) - rayon) / 180)) * cmToPulse;
            } else {
                side[LEFT] = 1;
                GoalDistance[LEFT] = (distance * (PI * (rayon - (DiamGRobot/2)) / 180)) * cmToPulse;
            }

            GoalVitesse[LEFT] = side[LEFT] * GoalVitesse[RIGHT] * ((rayon - (DiamGRobot/2)) / (rayon + (DiamGRobot/2)));

            break;
        case TOUR_DROIT:
            // TODO : Ajuster pour le décentrage
            side[LEFT] = 1;
            GoalVitesse[LEFT] = vitesse;
            GoalDistance[LEFT] = (distance * (PI * (rayon + (DiamDRobot/2)) / 180)) * cmToPulse;

            if (rayon <= (DiamDRobot/2)) {
                side[RIGHT] = -1;
                GoalDistance[RIGHT] = (distance * (PI * ((DiamDRobot/2) - rayon) / 180)) * cmToPulse;
            } else {
                side[RIGHT] = 1;
                GoalDistance[RIGHT] = (distance * (PI * (rayon - (DiamDRobot/2)) / 180)) * cmToPulse;
            }

            GoalVitesse[RIGHT] = side[RIGHT] * GoalVitesse[LEFT] * ((rayon - (DiamDRobot/2)) / (rayon + (DiamDRobot/2)));

            break;
        case AVANCE:
            // Appliquer une vitesse aux deux moteurs du robot.
            side[LEFT] = 1;
            side[RIGHT] = 1;
            GoalVitesse[LEFT] = vitesse;
            GoalVitesse[RIGHT] = vitesse;
            GoalDistance[0] = distance * cmToPulse;
            GoalDistance[1] = distance * cmToPulse;
            break;
        case RECULE:
            // Appliquer une vitesse aux deux moteurs du robot.
            side[LEFT] = -1;
            side[RIGHT] = -1;
            GoalVitesse[LEFT] = vitesse;
            GoalVitesse[RIGHT] = vitesse;
            GoalDistance[0] = -distance * cmToPulse;
            GoalDistance[1] = -distance * cmToPulse;
            break;
        case SUIVRE_LA_LIGNE:
            GoalVitesse[LEFT] = vitesse;
            mode_PID = LIGNE;
            currentEtat = SUIVRE_LIGNE;
            return;
    }
    mode_PID = AUTONOME;
}

void calcEncCompletion() {
    if (mode_PID == LIGNE) return;

    const int32_t encLeft = side[LEFT] * ENCODER_Read(LEFT);
    const int32_t encRight = side[RIGHT] * ENCODER_Read(RIGHT);
    encCompletion[LEFT] = max(encLeft / GoalDistance[LEFT], 0);
    // On fait le max pour être sûr de ne pas avoir de négatif.
    encCompletion[RIGHT] = max(encRight / GoalDistance[RIGHT], 0); // Même chose que l'autre ^
}

bool isGoal() {
    if (mode_PID == LIGNE) return false;

    calcEncCompletion();

    // Si le but est atteint pour les deux
    if (encCompletion[LEFT] > completionGoal && encCompletion[RIGHT] > completionGoal) {
        if (isMoving) {
            arreter();
            resetPIDStats();
        }
        isMoving = false;
        return true;
    }

    return false;
}

void ajusteVitesse() {
    //Si on suit la ligne, on veut toujours avoir à avancer
    if (mode_PID == LIGNE) {
        suivreLigne(GoalVitesse[LEFT]);
        return;
    }

    const unsigned long currentTime = millis();
    const float delta = max(currentTime - millisLastPID, 1) / 1000.0; // En seconde
    millisLastPID = currentTime;
    float vitesseReel[2] = {GoalVitesse[LEFT], GoalVitesse[RIGHT]};

    calcEncCompletion();

    // if (abs(GoalDistance) > 15 * cmToPulse) {
    //     int32_t distLEFT = ENCODER_Read(LEFT) * side[LEFT];
    //     if (abs(distLEFT) < ramp_duration) {
    //         // Linear ramp from 0.20 to GoalVitesse
    //         float ratio = (float) abs(distLEFT) / ramp_duration; // 0 → 1
    //         vitesseReel = initialSpeed + ratio * (GoalVitesse - initialSpeed);
    //     }
    //     // Peut décélérer en fait, si la fin est trop proche du début.
    //     if (abs(GoalDistance - distLEFT) < ramp_duration) {
    //         float ratio = abs(GoalDistance - distLEFT) / ramp_duration; // 0 → 1
    //         vitesseReel = initialSpeed + ratio * (vitesseReel - initialSpeed);
    //     }
    // }

    //Donne le nb de pulse qui devrait être fait depuis le début du mouvement.
    if (abs(pulseTotalSum[LEFT]) - abs(GoalDistance[LEFT]) < 1)
        pulseTotalSum[LEFT] += vitesseReel[LEFT] * ppsMax * delta * side[LEFT];

    if (abs(pulseTotalSum[RIGHT]) - abs(GoalDistance[RIGHT]) < 1)
        pulseTotalSum[RIGHT] += vitesseReel[RIGHT] * ppsMax * delta * side[RIGHT];

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
        // Serial.print(" GoalDistance:");
        // Serial.print(GoalDistance);
    }
    // Serial.println("");
}

// TODO : will met ton code de suivre la ligne ici
//GoalVitesse : la vitesse qui est demandé
// void ajusteVitesseLigne() ...

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
