/**
 * @file Définie une fonction pour chaque station du défi du combattant.
 * Ce sont les procédures à faire lorsque la station est rencontrée.
 */

#include "variables_globales.h" // Inclure les variables globales partagées entre tous les fichiers.
#include "moteur.cpp"           // Inclure les fonctions en lien avec les moteurs des roues.

/**
 * Fonction pour seulement avancer jusqu'à retrouver la ligne.
 *
 * Le robot avance tout droit.
 * Si la ligne est détectée, il se tourne parallèle à la ligne.
 * L'état du robot est changé à SUIVRE_LIGNE pour continuer le défi.
 */
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
void avancerTrouverLigne()
{
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
    uint8_t idCapteur = 0;
    int distanceSeuil = 15; // Distance en cm pour détecter l'obstacle
    uint16_t distanceActuelle = ROBUS_ReadIR(idCapteur);

    // Le robot avance jusqu'à ce que le capteur ultrason détecte le mur à une distance de ... cm.
    avancer(0.2);
    while (distanceSeuil <= distanceActuelle)
    {
        distanceActuelle = ROBUS_ReadIR(idCapteur);
    }
    arreter();

    // Avancer plus loin que l'obstacle

    bool obstacle = true;
    int distance = 10; // Distance en cm entre chaque essaie
    while (obstacle)
    {
        tourner(RIGHT, 90, 0.2);
        avancer(0.2); // TODO: avance seulement de 10 cm
        tourner(LEFT, 90, 0.2);
        if (ROBUS_ReadIR(idCapteur) > (distanceSeuil + 5))
            obstacle = false;
    }

    // Contourner l'epaisseur de l'objet avec la distance ...

    avancer(0.2);  // TODO: avance seulement de 10 cm

    // Revenir jusqu'a la ligne

    tourner(LEFT, 90, 0.2);
    retrouverLigne();
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