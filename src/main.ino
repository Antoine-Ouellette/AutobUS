/**
 * Projet: AutobUS
 * Equipe: P-12
 * Auteurs: Aimeric Bouillon, Mathieu Cabana, Samuel Houle, William Larouche, Alexi Ledoux, Antoine Ouellette, Adam Turcotte, Samy Yamouni
 * Description: Boucle principale du robot. S'occupe des états du robot (avancer, arrêt, tourner, penser).
 * Date: 2025-10-02
 */

#include <LibRobus.h> // Essentielle pour utiliser les fonctions RobUS.
#include "variablesGlobales.h" // Variables globales utilisées par le programme
#include "moteur.h" // Fichier pour controller les moteurs.
#include "labyrinthe.h" // Fichier qui se souvient des pos et prend les décisions dans le labyrinthe.

bool bumperArr;

/**
    Fonction qui fait beep
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

/**
 * Fonctions de boucle infinie
 * Se fait appeler perpétuellement après que le setup() soit terminé.
 * Quand la fonction atteint la fin, elle recommence au début.
 * @note: Ne pas ajouter de delay() dans cette boucle.
 */
void loop() {
    previousEtat = currentEtat;

    //Todo : changer le départ pour le sifflet
    bumperArr = ROBUS_IsBumper(REAR);
    if (bumperArr) {
        if (currentEtat == 0) {
            beep(2);
            currentEtat = PRISE_DECISION;
        } else {
            beep(1);
            currentEtat = ARRET;
        }
    }

    if (isGoal()) {
        //Si on est rendu, compute le mouvement et cherche le prochain
        endMouvement();
        currentEtat = PRISE_DECISION;
    }

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
                arret();
                break;
            case AVANCER:
            case TOURNER_DROITE:
            case TOURNER_GAUCHE:
            case TOURNER_180:
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
