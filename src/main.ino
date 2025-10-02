/**
 * Projet: AutobUS
 * Equipe: P-12
 * Auteurs: Aimeric Bouillon, Mathieu Cabana, Samuel Houle, William Larouche, Alexi Ledoux, Antoine Ouellette, Adam Turcotte, Samy Yamouni
 * Description: Boucle principale du robot. S'occupe des états du robot (avancer, arrêt, tourner).
 * Date: 2025-10-02
 */
#include <LibRobus.h> // Essentielle pour utiliser les fonctions RobUS.

/**
 * Vitesse à laquelle le robot avance.
 * C'est le pourcentage de la vitesse maximale.
 * Doit être entre 0 et 1. ex: 0.5 = 50%.
 */
#define VITESSE_AVANCER 1

/**
 * Valeurs possibles de l'état du robot.
 * ARRET: Le robot est à l'arrêt.
 * AVANCER: Le robot avance.
 * TOURNER_GAUCHE: Le robot tourne à gauche.
 * TOURNER_DROITE: Le robot tourne à droite.
 */
enum Etat { ARRET, AVANCER, TOURNER_GAUCHE, TOURNER_DROITE };

// État actuel du robot.
Etat currentEtat = ARRET; // Au départ, le robot est à l'arrêt.

// État du robot dans l'itération précédente de loop().
Etat previousEtat = ARRET;

/**
 * Met le robot à l'arrêt.
 * Arrête les 2 moteurs.
 */
void arreter() {
    MOTOR_SetSpeed(RIGHT, 0);
    MOTOR_SetSpeed(LEFT, 0);
}

/**
 * Fait avancer le robot à une certaine vitesse.
 * Active les 2 moteurs jusqu'à ce qu'on les arrête.
 * @param vitesse Pourcentage de la vitesse à laquelle les moteurs doivent rouler.
 * La valeur doit être entre 0 et 1. 1 étant pour avancer à 100% de la vitesse possible.
 */
void avancer(float vitesse) {
    MOTOR_SetSpeed(RIGHT, vitesse);
    MOTOR_SetSpeed(LEFT, vitesse);
}

/**
 * Fait reculer le robot à une certaine vitesse.
 * Active les 2 moteurs jusqu'à ce qu'on les arrête.
 * @param vitesse Pourcentage de la vitesse à laquelle les moteurs doivent rouler.
 * La valeur doit être entre 0 et 1. 1 étant pour reculer à 100% de la vitesse possible.
 */
void reculer(float vitesse) {
    MOTOR_SetSpeed(RIGHT, -vitesse);
    MOTOR_SetSpeed(LEFT, -vitesse);
}

/**
 * Fait tourner le robot vers la gauche à une certaine vitesse.
 * Active les 2 moteurs jusqu'à ce qu'on change leur valeur de vitesse.
 * @param vitesse Pourcentage de la vitesse à laquelle les moteurs doivent rouler.
 * La valeur doit être entre 0 et 1. 1 étant pour que les moteurs roulent à 100% de la vitesse possible.
 */
void tournerGauche(float vitesse) {
    MOTOR_SetSpeed(RIGHT, vitesse);
    MOTOR_SetSpeed(LEFT, -vitesse);
}

/**
 * Fait tourner le robot vers la droite à une certaine vitesse.
 * Active les 2 moteurs jusqu'à ce qu'on change leur valeur de vitesse.
 * @param vitesse Pourcentage de la vitesse à laquelle les moteurs doivent rouler.
 * La valeur doit être entre 0 et 1. 1 étant pour que les moteurs roulent à 100% de la vitesse possible.
 */
void tournerDroite(float vitesse) {
    MOTOR_SetSpeed(RIGHT, -vitesse);
    MOTOR_SetSpeed(LEFT, vitesse);
}

/**
 * Fonction d'initialisation (Setup)
 * Exécutée une seule fois lorsque le robot est allumé.
 * Initialise les capteurs et prépare ce qui doit être prêt avant la loop().
 */
void setup() {
    BoardInit(); // Initialisation de la carte RobUS.
}

/**
 * Fonctions de boucle infinie
 * Se fait appeler perpétuellement après que le setup() soit terminé.
 * Quand la fonction atteint la fin, elle recommence au début.
 * @note: Ne pas ajouter de delay() dans cette boucle.
 */
void loop() {
    previousEtat = currentEtat; // Sauvegarder l'état actuel dans la variable previousEtat avant de mettre à jour currentEtat.
    
    // Si l'état a changé depuis la dernière itération de loop().
    if (previousEtat != currentEtat) {
        if (currentEtat == ARRET) {
            arreter(); // Mettre le robot à l'arrêt.
        }
    }

    // Gérer quoi faire dépendamment de l'état actuel.
    switch (currentEtat) {
        case AVANCER:
            avancer(VITESSE_AVANCER); // Entrer dans la fonction pour la procédure de suivi de la ligne.
            break;
        case TOURNER_GAUCHE:
            tournerGauche(VITESSE_AVANCER); // Entrer dans la fonction pour la procédure pour tourner à gauche.
            break;
        case TOURNER_DROITE:
            tournerDroite(VITESSE_AVANCER); // Entrer dans la fonction pour la procédure pour tourner à droite.
            break;
    }

    delay(10); // délai pour décharger le CPU.
}