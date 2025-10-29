/**
 * @file Définie une fonction pour chaque station du défi du combattant.
 * Ce sont les procédures à faire lorsque la station est rencontrée.
 */

#include "actions_stations.h"
#include <LibRobus.h>           // Essentielle pour utiliser RobUS.
#include "variables_globales.h" // Inclure les variables globales partagées entre tous les fichiers.
#include "moteur.h"             // Inclure les fonctions en lien avec les moteurs des roues.
#include "suiveur_ligne.h"      // Inclure les fonctions en lien avec le suiveur de ligne.

// Fonction pour seulement avancer jusqu'à retrouver la ligne.

int leds[3] = {10, 11, 12};

void retrouverLigne()
{
}

/**
 * Action à faire lorsque la station de passage sans ligne est atteinte.
 *
 * Le robot avance jusqu'à ce que la ligne termine.
 * Le robot avance tout droit sur 75 cm.
 * Si la ligne n'est pas détectée, le robot va à droite sur max 30 cm.
 * Si la ligne n'est pas détectée, le robot va à gauche sur max 60 cm.
 * Si la ligne n'est pas détectée, le robot revient au milieu en allant 30 cm à droite,
 * puis avance sur max 30 cm.
 * Si la ligne n'est pas détectée, le robot va à droite sur max 30 cm.
 * Si la ligne n'est pas détectée, le robot va à gauche sur max 60 cm.
 * Si la ligne n'est toujours pas détectée, le robot recule et recommence.
 */
void suivreLigne()
{

    float VITESSE_AVANCE = 0.25;                              // Vitesse d'avancement en ligne droite normale
    float VITESSE_CORRECTION_ELEVEE = VITESSE_AVANCE * 0.2; // Vitesse de correction pour retrouver la ligne
    int depassement;
    int alignement;
    int i = 0;

    Serial.println("SUIVEUR: ");

    uint8_t result = SUIVEUR_Read();
    for (int i = 0; i < 3; i++)
    {
        digitalWrite(leds[i], (result & (1 << i)) ? HIGH : LOW);
    }

    switch (SUIVEUR_Read())
    {

    case 0b010: // centré sur la ligne
        Serial.println(" CENTRE: ");
        avancer(VITESSE_AVANCE);
        i = 0;
        break;

    case 0b100: // corrige à gauche
        Serial.println(" FORT GAUCHE: ");
        MOTOR_SetSpeed(RIGHT, VITESSE_CORRECTION_ELEVEE);
        i++;
        break;

    case 0b001: // corrige à droite
        Serial.println(" FORT DROITE: ");
        MOTOR_SetSpeed(LEFT, VITESSE_CORRECTION_ELEVEE);
        i--;
        break;

    case 0b111: // Perpendiculaire à la ligne
        Serial.println(" PERPENDICULAIRE: ");
        /*arreter();        // prend une pause
        delay(100);        // attend un peu pour stabiliser la lecture
        ENCODER_Reset(0); // Reset des encodeurs
        ENCODER_Reset(1);
        depassement = ENCODER_Read(0); // enregistre la distance dépassée
        ENCODER_Reset(0);                  // Reset des encodeurs
        ENCODER_Reset(1);

        alignement = depassement - (4.7 * cmToPulse); // Calcule l'alignement à faire après l'arrêt
        while (ENCODER_Read(0) <= alignement)
        { // Avance pour aligner le suiveur à la ligne une fois tourné
            avancer(0.05);
        }
        arreter();
        tourner(LEFT, 90, 0.1); // Probléatique si vient du dessous
        */

        break;

    case 0b000: // ligne perdue
        Serial.println(" PERDU: ");
        if (i >= 0){
            MOTOR_SetSpeed(RIGHT, 0.35);
        }
        else if (i <= 0){
            MOTOR_SetSpeed(LEFT, 0.35);
        }
        break;
    }
}
void avancerTrouverLigne()
{
    const int distance = 80; // Distancce en cm à avancer
    uint8_t OutputSuiveur = SUIVEUR_Read();

    if (OutputSuiveur == 0b101) // A MODIFIER POUR 111 !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    {
        // Détection de la ligne perpendiculaire
        arreter();        // prend une pause
        ENCODER_Reset(0); // Reset des encodeurs
        ENCODER_Reset(1);

        while (ENCODER_Read(0) <= distance * cmToPulse)
        {
            // Avance jusqu'à la distance spécifiée
            avancer(0.2);
        }

        if (ENCODER_Read(0) >= distance * cmToPulse)
        {
            // Vérification de la distance atteinte
            arreter();
        }

        if (OutputSuiveur == 0b000)
        {
            retrouverLigne();
        }
        else
        {
            suivreLigne();
        }
    }
}

/**
 * Action à faire lorsque la station de la quille est atteinte.
 *
 * Le robot doit trouver la quille. Il fait un tour à 360 degrés et utilise
 * le capteur à ultrasons pour la détecter.
 * Le robot enregistre à quel angle il va partir.
 * Le robot avance dans la direction de la quille jusqu'à ce que la valeur de la distance
 * soit très proche.
 * Le robot fonce dans la quille.
 * Le robot recule un peu pour laisser de l'espace.
 * Le robot tourne pour faire face à la ligne (en utilisant le nombre de degrés tourné).
 * Le robot avance jusqu'à retrouver la ligne (utilise la méthode retrouverLigne()).
 * L'état du robot est changé à SUIVRE_LIGNE pour avancer jusqu'au prochain défi.
 */
void renverserQuille()
{
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
}

/**
 * Action à faire lorsque la station de danse est atteinte.
 *
 * Le robot tourne de 135 degrés à gauche.
 * Le robot avance de ... cm.
 * Le robot tourne de 90 degrés à gauche.
 * Le robot avance de ... cm.
 * Le robot tourne de 90 degrés à gauche.
 * Le robot avance de ... cm.
 * Le robot tourne de 90 degrés à gauche.
 * Le robot avance de ... cm.
 * Le robot tourne de 45 degrés à gauche pour faire face à la ligne.
 * Le robot avance jusqu'à retrouver la ligne (utilise la méthode retrouverLigne()).
 * (Le robot recule s'il n'a pas retrouvé la ligne.)
 * Le robot tourne de 90 degrés à droite pour être parallèle à la ligne.
 * L'état du robot est changé à SUIVRE_LIGNE pour avancer jusqu'au prochain défi.
 */
void danserLosange()
{
    // TODO: tourner() est asynchrone. Il faudrait une deuxième fonction.
    tourner(LEFT, 135, 1);
    avancer(1);
    tourner(LEFT, 90, 1);
    avancer(1);
    tourner(LEFT, 90, 1);
    avancer(1);
    tourner(LEFT, 90, 1);
    avancer(1);
    tourner(LEFT, 45, 1);
    retrouverLigne();
    tourner(RIGHT, 90, 1);
    currentEtat = SUIVRE_LIGNE;
}
