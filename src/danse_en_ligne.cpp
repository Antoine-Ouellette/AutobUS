/**
 * Projet: AutobUS
 * Equipe: P-12
 * Auteurs: Aimeric Bouillon, Mathieu Cabana, Samuel Houle, William Larouche,
 * Alexi Ledoux, Antoine Ouellette, Adam Turcotte, Samy Yamouni Description:
 * Boucle principale du robot. S'occupe des états du robot (avancer, arrêt,
 * tourner). Date: 2025-10-02
 */
#include "actions_stations.h" // Inclure les actions à faire pour chaque station.
#include "detecteur_couleur.h" // Inclure les fonctions en lien avec le détecteur de couleurs.
#include "moteur.h" // Inclure les fonctions en lien avec les moteurs des roues.
#include "suiveur_ligne.h" // Inclure les fonctions en lien avec le suiveur de ligne.
#include "variables_globales.h" // Inclure les variables globales partagées entre tous les fichiers.
#include <LibRobus.h> // Essentielle pour utiliser RobUS.
unsigned long lastUpdatePID = 0;
int danse = -1;
int prev_rnd = 0;
int alter = 0;

unsigned long prevMil = 0;
unsigned long prevMil2 = 0;

#define L_UP 180
#define L_FORWARD 120
#define L_DOWN 60

#define R_UP 0
#define R_FORWARD 60
#define R_DOWN 140

/**
 * Fonction d'initialisation (Setup)
 * Exécutée une seule fois lorsque le robot est allumé.
 * Initialise les capteurs et prépare ce qui doit être prêt avant la loop().
 */
void setup() {
    BoardInit(); // Initialisation de la carte RobUS.
    for (int i = 0; i < 4; i++) {
        pinMode(leds[i], OUTPUT);
    }

    Serial.begin(
        9600); // Initialisation de la communication série pour le débogage.

    while (!ROBUS_IsBumper(REAR));
    SERVO_Enable(LEFT);
    SERVO_Enable(RIGHT);

    SERVO_SetAngle(LEFT, L_FORWARD);
    SERVO_SetAngle(RIGHT, R_FORWARD);
    randomSeed(analogRead(A0));
}

/**
 * Fonctions de boucle infinie
 * Se fait appeler perpétuellement après que le setup() soit terminé.
 * Quand la fonction atteint la fin, elle recommence au début.
 * @note: Ne pas ajouter de delay() dans cette boucle.
 */
void loop() {
    if (isMoving) {
        ajusteVitesse();
        isGoal();
    }
    switch (danse) {
        case -1:
            prevMil = millis();
            danse++;
            break;
        case 0:
            if (millis() - prevMil > 24000) {
                prevMil = millis();
                danse++;
            }
            break;

        case 1:
            SERVO_SetAngle(LEFT, L_UP);
            if (millis() - prevMil > 2000) {
                prevMil = millis();
                danse++;
            }
            break;

        case 2:
            SERVO_SetAngle(RIGHT, R_UP);
            if (millis() - prevMil > 2000) {
                prevMil = millis();
                danse++;
            }
            break;

        case 3:
            SERVO_SetAngle(LEFT, L_DOWN);
            if (millis() - prevMil > 2000) {
                prevMil = millis();
                danse++;
            }
            break;

        case 4:
            SERVO_SetAngle(RIGHT, R_DOWN);
            if (millis() - prevMil > 2000) {
                prevMil = millis();
                danse++;
            }
            break;

        case 5:
            digitalWrite(ROUGE, 1);
            if (millis() - prevMil > 2000) {
                prevMil = millis();
                danse++;
            }
            break;

        case 6:
            digitalWrite(leds[1], 0);
            digitalWrite(leds[3], 1);
            if (millis() - prevMil > 2000) {
                prevMil = millis();
                danse++;
            }
            break;

        case 7:
            digitalWrite(leds[3], 0);
            digitalWrite(leds[2], 1);
            if (millis() - prevMil > 2000) {
                prevMil = millis();
                danse++;
            }
            break;

        case 8:
            digitalWrite(leds[2], 0);
            digitalWrite(leds[0], 1);
            if (millis() - prevMil > 2000) {
                prevMil = millis();
                danse++;
            }
            break;

        case 9:
            SERVO_SetAngle(LEFT, L_FORWARD);
            SERVO_SetAngle(RIGHT, R_FORWARD);
            digitalWrite(leds[0], 0);
            digitalWrite(leds[1], 1);
            if (millis() - prevMil > 2000) {
                prevMil = millis();
                danse++;
            }
            break;

        case 10:
            SERVO_SetAngle(LEFT, L_UP);
            SERVO_SetAngle(RIGHT, R_UP);
            digitalWrite(leds[1], 0);
            digitalWrite(leds[3], 1);
            if (millis() - prevMil > 2000) {
                prevMil = millis();
                danse++;
            }
            break;

        case 11:
            SERVO_SetAngle(LEFT, L_FORWARD);
            SERVO_SetAngle(RIGHT, R_FORWARD);
            digitalWrite(leds[3], 0);
            digitalWrite(leds[2], 1);
            if (millis() - prevMil > 2000) {
                prevMil = millis();
                danse++;
            }
            break;

        case 12:
            SERVO_SetAngle(LEFT, L_DOWN);
            SERVO_SetAngle(RIGHT, R_DOWN);
            digitalWrite(leds[2], 0);
            digitalWrite(leds[0], 1);
            if (millis() - prevMil > 2000) {
                prevMil = millis();
                danse++;
                setGoal(0.4, TOUR_DROIT, 360);
            }
            break;

        case 13:
            SERVO_SetAngle(LEFT, L_UP);
            SERVO_SetAngle(RIGHT, R_UP);

            if (millis() - prevMil2 > 500) {
                prevMil2 = millis();
                int rnd = random(4);
                digitalWrite(leds[rnd], 1);
                digitalWrite(leds[prev_rnd], 0);
                prev_rnd = rnd;
            }

            if (millis() - prevMil > 2000) {
                prevMil = millis();
                danse++;
            }
            break;

        case 14:
            SERVO_SetAngle(LEFT, L_DOWN);
            SERVO_SetAngle(RIGHT, R_DOWN);
            if (millis() - prevMil2 > 250) {
                prevMil2 = millis();
                if (alter == 0) {
                    digitalWrite(leds[1], HIGH);
                    digitalWrite(leds[3], LOW);
                    alter = 1;
                } else {
                    digitalWrite(leds[1], LOW);
                    digitalWrite(leds[3], HIGH);
                    alter = 0;
                }
            }

            if (millis() - prevMil > 4000) {
                prevMil = millis();
                danse++;
            }
            break;

        case 15:
            if (millis() - prevMil2 > 250) {
                prevMil2 = millis();
                if (alter == 0) {
                    digitalWrite(leds[3], HIGH);
                    digitalWrite(leds[2], LOW);
                    alter = 1;
                } else {
                    digitalWrite(leds[3], LOW);
                    digitalWrite(leds[2], HIGH);
                    alter = 0;
                }
            }

            if (millis() - prevMil > 4000) {
                prevMil = millis();
                danse++;
            }
            break;

        case 16:
            if (millis() - prevMil2 > 250) {
                prevMil2 = millis();
                if (alter == 0) {
                    digitalWrite(leds[2], HIGH);
                    digitalWrite(leds[0], LOW);
                    alter = 1;
                } else {
                    digitalWrite(leds[2], LOW);
                    digitalWrite(leds[0], HIGH);
                    alter = 0;
                }
            }

            if (millis() - prevMil > 4000) {
                prevMil = millis();
                danse++;
            }
            break;

        case 17:
            if (millis() - prevMil2 > 250) {
                prevMil2 = millis();
                if (alter == 0) {
                    digitalWrite(leds[1], HIGH);
                    digitalWrite(leds[0], LOW);
                    alter = 1;
                } else {
                    digitalWrite(leds[1], LOW);
                    digitalWrite(leds[0], HIGH);
                    alter = 0;
                }
            }

            if (millis() - prevMil > 4000) {
                prevMil = millis();
                danse++;
            }
            break;

        case 18:
            SERVO_SetAngle(LEFT, L_FORWARD);
            SERVO_SetAngle(RIGHT, R_FORWARD);

            if (millis() - prevMil2 > 500) {
                prevMil2 = millis();
                int rnd = random(4);
                digitalWrite(leds[rnd], 1);
                digitalWrite(leds[prev_rnd], 0);
                prev_rnd = rnd;
            }

            if (millis() - prevMil > 4000) {
                prevMil = millis();
                danse++;
            }
            break;

        case 19: // P1
            SERVO_SetAngle(LEFT, L_UP);
            SERVO_SetAngle(RIGHT, R_UP);
            if (millis() - prevMil > 4000) {
                prevMil = millis();
                danse++;
                setGoal(0.2, RECULE, 20);
            }
            break;

        case 20:
            if (millis() - prevMil > 2000) {
                prevMil = millis();
                danse++;
                setGoal(0.2, TOUR_DROIT, 45);
            }
            break;

        case 21: // P2
            if (millis() - prevMil > 2000) {
                prevMil = millis();
                danse++;
                setGoal(0.2, AVANCE, 28.284);
            }
            break;

        case 22:
            if (millis() - prevMil > 2000) {
                prevMil = millis();
                danse++;
                setGoal(0.2, TOUR_GAUCHE, 90);
            }
            break;

        case 23: // P3
            if (millis() - prevMil > 2000) {
                prevMil = millis();
                danse++;
                setGoal(0.2, AVANCE, 28.284);
            }
            break;

        case 24:
            if (millis() - prevMil > 2000) {
                prevMil = millis();
                danse++;
                setGoal(0.2, TOUR_DROIT, 90);
            }
            break;

        case 25: // P4
            if (millis() - prevMil > 2000) {
                prevMil = millis();
                danse++;
                setGoal(0.2, RECULE, 28.284);
            }
            break;

        case 26:
            if (millis() - prevMil > 2000) {
                prevMil = millis();
                danse++;
                setGoal(0.2, TOUR_GAUCHE, 90);
            }
            break;

        case 27: // P1
            if (millis() - prevMil > 2000) {
                prevMil = millis();
                danse++;
                setGoal(0.2, RECULE, 28.284);
            }
            break;

        case 28:
            if (millis() - prevMil > 2000) {
                prevMil = millis();
                danse++;
                setGoal(0.2, TOUR_DROIT, 90);
            }

            break;

        case 29: // P2
            if (millis() - prevMil > 2000) {
                prevMil = millis();
                danse++;
                setGoal(0.2, AVANCE, 28.284);
            }
            break;

        case 30:
            if (millis() - prevMil > 2000) {
                prevMil = millis();
                danse++;
                setGoal(0.2, TOUR_GAUCHE, 90);
            }
            break;

        case 31: // P3
            if (millis() - prevMil > 2000) {
                prevMil = millis();
                danse++;
                setGoal(0.2, AVANCE, 28.284);
            }
            break;

        case 32:
            if (millis() - prevMil > 2000) {
                prevMil = millis();
                danse++;
                setGoal(0.2, TOUR_DROIT, 90);
            }
            break;

        case 33: // P4
            if (millis() - prevMil > 2000) {
                prevMil = millis();
                danse++;
                setGoal(0.2, RECULE, 28.284);
            }
            break;

        case 34:
            if (millis() - prevMil > 1333) {
                prevMil = millis();
                danse++;
                setGoal(0.2, TOUR_DROIT, 45);
            }
            break;

        case 35: // P4
            if (millis() - prevMil > 1333) {
                prevMil = millis();
                danse++;
                setGoal(0.2, AVANCE, 20);
            }
            break;

        case 36:
            if (millis() - prevMil > 1333) {
                prevMil = millis();
                danse++;
                setGoal(0.2, TOUR_DROIT, 90);
            }
            break;

        case 37: //Début changement position
            if (millis() - prevMil > 2000) {
                prevMil = millis();
                danse++;
                setGoal(0.2, RECULE, 20);
            }
            break;

        case 38:
            if (millis() - prevMil > 1500) {
                prevMil = millis();
                danse++;
                setGoal(0.2, TOUR_GAUCHE, 90);
            }
            break;

        case 39:
            if (millis() - prevMil > 500) {
                prevMil = millis();
                danse++;
                setGoal(0.2, AVANCE, 80);
            }
            break;

        case 40:
            if (millis() - prevMil > 4000) {
                prevMil = millis();
                danse++;
                setGoal(0.2, TOUR_DROIT, 90);
            }
            break;

        case 41:
            if (millis() - prevMil > 500) {
                prevMil = millis();
                danse++;
                setGoal(0.2, AVANCE, 20);
                //suivreLigne();
            }
            break;
        case 42:
            if (millis() - prevMil > 1500) {
                prevMil = millis();
                danse++;
                setGoal(0.2, AVANCE, 20);
            }
        case 43:
            if (millis() - prevMil > 2000) {
                prevMil = millis();
                danse++;
                digitalWrite(ROUGE, 1);
            }
            break;

        default:
            break;
    }

    delay(10); // Délai pour décharger le CPU.
}
