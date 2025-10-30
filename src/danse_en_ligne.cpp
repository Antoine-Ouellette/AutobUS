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
int danse = 0;

unsigned long prevMil = 0;

#define L_UP 180
#define L_FORWARD 120
#define L_DOWN 60

#define R_UP 0
#define R_FORWARD 60
#define R_DOWN 140

#define BLEU 10
#define ROUGE 11
#define VERT 12
#define JAUNE 13

/**
 * Fonction d'initialisation (Setup)
 * Exécutée une seule fois lorsque le robot est allumé.
 * Initialise les capteurs et prépare ce qui doit être prêt avant la loop().
 */
void setup() {
    BoardInit(); // Initialisation de la carte RobUS.
    pinMode(BLEU, OUTPUT);
    pinMode(ROUGE, OUTPUT);
    pinMode(VERT, OUTPUT);
    pinMode(JAUNE, OUTPUT);

    Serial.begin(
        9600); // Initialisation de la communication série pour le débogage.

    while (!ROBUS_IsBumper(REAR))
        ;
    SERVO_Enable(LEFT);
    SERVO_Enable(RIGHT);

    SERVO_SetAngle(LEFT, L_FORWARD);
    SERVO_SetAngle(RIGHT, R_FORWARD);
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
    case 0:
        if (millis() - prevMil > /*24000*/ 2000) {
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
        digitalWrite(ROUGE, 0);
        digitalWrite(JAUNE, 1);
        if (millis() - prevMil > 2000) {
            prevMil = millis();
            danse++;
        }
        break;

    case 7:
        digitalWrite(JAUNE, 0);
        digitalWrite(VERT, 1);
        if (millis() - prevMil > 2000) {
            prevMil = millis();
            danse++;
        }
        break;

    case 8:
        digitalWrite(VERT, 0);
        digitalWrite(BLEU, 1);
        if (millis() - prevMil > 2000) {
            prevMil = millis();
            danse++;
        }
        break;

    case 9:
    SERVO_SetAngle(LEFT, L_FORWARD);
    SERVO_SetAngle(RIGHT, R_FORWARD);
    digitalWrite(BLEU, 0);
    digitalWrite(ROUGE, 1);
        if (millis() - prevMil > 2000) {
            prevMil = millis();
            danse++;
        }
        break;

    case 10:
        SERVO_SetAngle(LEFT, L_UP);
    SERVO_SetAngle(RIGHT, R_UP);
        digitalWrite(ROUGE, 0);
        digitalWrite(JAUNE, 1);
        if (millis() - prevMil > 2000) {
            prevMil = millis();
            danse++;
        }
        break;

    case 11:
        SERVO_SetAngle(LEFT, L_FORWARD);
    SERVO_SetAngle(RIGHT, R_FORWARD);
        digitalWrite(JAUNE, 0);
        digitalWrite(VERT, 1);
        if (millis() - prevMil > 2000) {
            prevMil = millis();
            danse++;
        }
        break;

    case 12:
        SERVO_SetAngle(LEFT, L_DOWN);
    SERVO_SetAngle(RIGHT, R_DOWN);
        digitalWrite(VERT, 0);
        digitalWrite(BLEU, 1);
        if (millis() - prevMil > 2000) {
            prevMil = millis();
            danse++;
            setGoal(0.4, TOUR_DROIT, 360);
        }
        break;

            case 13:
        SERVO_SetAngle(LEFT, L_UP);
    SERVO_SetAngle(RIGHT, R_UP);
        digitalWrite(VERT, 1);
        digitalWrite(BLEU, 1);
            
        if (millis() - prevMil > 2000) {
            prevMil = millis();
            danse++;
        }
        break;

                    case 14:
        SERVO_SetAngle(LEFT, L_UP);
    SERVO_SetAngle(RIGHT, R_UP);
        digitalWrite(VERT, 1);
        digitalWrite(BLEU, 1);
            
        if (millis() - prevMil > 4000) {
            prevMil = millis();
            danse++;
        }
        break;

    default:
        break;
    }

    delay(10); // Délai pour décharger le CPU.
}
