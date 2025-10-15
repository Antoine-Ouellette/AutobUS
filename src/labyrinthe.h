/**
Projet: AutobUS
Equipe: P-12
Auteurs: Adam Turcotte
Description: Fonctions d'action dans le labyrinthe
Date: 3 octobre 2025
*/

#ifndef AUTOBUS_LABYRINTHE_H
#define AUTOBUS_LABYRINTHE_H

/**Re-met les encodeurs a 0.
Met a jour la position et la rotation dans les variables. */
void endMouvement() ;

/** Met à jour le mur dans la case an face du robot. */
void ajouteMurEnFace();

/**Regarde s'il y a un mur devant lui et l'ajoute à la carte*/
void updateWall() ;

/**
 * Calcule le prochain déplacement pour avancer dansa le labyrinthe
 */
void findNextMove();

void allerNord();
void allerOuest();
void allerEst();
void allerSud();

#endif //AUTOBUS_LABYRINTHE_H