/**
 * Projet: AutobUS
 * Equipe: P-12
 * Auteurs: Aimeric Bouillon, Mathieu Cabana, Samuel Houle, William Larouche, Alexi Ledoux, Antoine Ouellette, Adam Turcotte, Samy Yamouni
 * Description: Boucle principale du robot. S'occupe des états du robot (avancer, arrêt, tourner, penser).
 * Date: 2025-10-07
 */


#include <LibRobus.h> // Essentielle pour utiliser les fonctions RobUS.

#include "constantes.h"
#include "variablesGlobales.h" // Variables globales utilisées par le programme
#include "moteur.h" // Fichier pour controller les moteurs.
#include "labyrinthe.h" // Fichier qui se souvient des pos et prend les décisions dans le labyrinthe.

//Donner des valeurs aux variables globales
Etat currentEtat = ARRET;
Etat previousEtat = ARRET;
int nbCaseAvance = 1;
unsigned long startMillis = 0;

bool bumperArr;

// Todo : Remove before final code version
int nbTours = 1;
int coteTour = 0;

/**
 * Fonction qui fait beep
 */
void beep(const int count) {
    for (int i = 0; i < count; i++) {
        AX_BuzzerON();
        delay(100);
        AX_BuzzerOFF();
        delay(100);
    }
    delay(400);
}

/**
 * Fonction d'initialisation (Setup)
 * Exécutée une seule fois lorsque le robot est allumé.
 * Initialise les capteurs et prépare ce qui doit être prêt avant la loop().
 */
void setup() {
    BoardInit(); // Initialisation de la carte RobUS.
    beep(1);
}


//***************************
//IMPORTANT Avant de compiler le code, changer pour le bon model de robot dans les constantes.
//***************************

/**
 * Fonctions de boucle infinie
 * Se fait appeler perpétuellement après que le setup() soit terminé.
 * Quand la fonction atteint la fin, elle recommence au début.
 * @note: Ne pas ajouter de delay() dans cette boucle.
 */
void loop() {
    previousEtat = currentEtat;


    //Todo : changer le départ pour le sifflet
    if (ROBUS_IsBumper(REAR)) {
        if (currentEtat == 0) {
            beep(2);
            currentEtat = PRISE_DECISION;
        }
    }

    //Tourne a gauche et a droite pour des testes.
    if (ROBUS_IsBumper(RIGHT)) {
        if (currentEtat == 0) {
            coteTour = 1;
            nbTours = 1;
            currentEtat = TOURNER_DROITE;
            calculateGoals();
        }
    } else if (ROBUS_IsBumper(LEFT)) {
        if (currentEtat == 0) {
            coteTour = -1;
            nbTours = 1;
            currentEtat = TOURNER_GAUCHE;
            calculateGoals();
        }
    }

    if (isGoal()) {
        //Si on est rendu, compute le mouvement et cherche le prochain
        endMouvement();
        currentEtat = PRISE_DECISION;


        // Todo : Remove before final code version
        // Pour faire les tests de tourner
        if (coteTour != 0) {
            if (nbTours < 4 * 2) {
                if (coteTour == 1) currentEtat = TOURNER_DROITE;
                else if (coteTour == -1) currentEtat = TOURNER_GAUCHE;
                calculateGoals();
                nbTours++;
            } else {
                coteTour = 0;
                currentEtat = ARRET;
            }
        }
    }

    // Todo : Remove before final code version
    // Serial.print("Completion : ");
    // Serial.print(encoderLeftCompletion);
    // Serial.print(" : ");
    // Serial.print(encoderRightCompletion);
    // Serial.print(" ;; pulse :");
    // Serial.print(ENCODER_Read(LEFT));
    // Serial.print(" , ");
    // Serial.print(ENCODER_Read(RIGHT));
    // Serial.print(" ; goal :");
    // Serial.print(encoderLeftGoal);
    // Serial.print(" , ");
    // Serial.println(encoderRightGoal);
    //

    if (previousEtat != currentEtat) {
        /* Si le mouvement change, prendre une pause
         * et réinitialiser la distance parcourut par chaque roue.
         */
        arret();
        delay(50);
        startMillis = millis() + 10;
        resetEncoders();
    } else {
        switch (currentEtat) {
            case ARRET:
                //Rien à faire, il arrête déjà quand il change.
                break;
            case AVANCER:
            case TOURNER_DROITE:
            case TOURNER_GAUCHE:
            case TOURNER_180:
                ajusteVitesse();
                effectueDeplacement();
                break;
            case PRISE_DECISION:
                updateWall();
                findNextMove();
                break;
            default:
                currentEtat = ARRET;
                break;
        }
    }

    delay(10); //Pour décharger le CPU
}
