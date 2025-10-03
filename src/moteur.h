/**
Projet: AutobUS
Equipe: P-12
Auteurs: Antoine Ouellette, Adam Turcotte
Description: Fonction de contrôle des moteurs et des encodeurs
Date: 2 octobre 2025
*/
#ifndef AUTOBUS_MOTEUR_H
#define AUTOBUS_MOTEUR_H

/**
 * Détermine si au moins un des deux encodeurs a atteint son objectif.
 * @return true : objectif atteint\n false : objectif non atteint
 */
bool isGoal();

/**
 * Permet de réinitialiser les objectifs pour les deux encodeurs.
 */
void resetGoals();

/**
 * Permet de réinitialiser la distance enregistrée par les encodeurs des roues.
 */
void resetEncoders();

/**
 * Calcule les objectifs, en pulse, pour chaque encodeur.
 * L'objectif est calculé par la distance en cm convertie en pulse.
 * Indique la direction dans laquelle le moteur doit tourner avec side[]
 * @details But possible : avancer (distance), tourner droite (90°), tourner gauche (90°),
 * tourner 180°
 */
void calculateGoals();

/**
 * Fait avancer le robot dans la direction voulue par l'état.
 * Modifie la vitesse des moteurs pour qu'ils suivent le PID.
 */
void effectueDeplacement();

/**
 * Met le robot à l'arrêt
 * Arrête les deux moteurs
 */
void arret();

/**
 * Ajuste la vitesse des moteurs selon la distance qu'il aurait dû avoir parcouru
 * depuis le début du mouvement.
 */
void ajusteVitesse();

#endif //AUTOBUS_MOTEUR_H
