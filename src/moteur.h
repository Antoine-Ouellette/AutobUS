/**
* @file Fonctions en lien avec les moteurs des roues du robot.
 * Inclut les déplacements et le PID.
 */

#ifndef AUTOBUS_MOTEUR_H
#define AUTOBUS_MOTEUR_H

enum MOUVEMENT { TOUR_GAUCHE, TOUR_DROIT, AVANCE, RECULE };


/**
 * Permet d'établir l'objectif de chaque moteur, quand on veut avancer d'une distance fixe
 * @param vitesse Vitesse que le robot veut pendant son déplacement
 * @param mouvement Mouvement que le robot veut faire {TOUR_GAUCHE, TOUR_DROIT, AVANCE, RECULE}
 * @param distance Distance en cm que le robot doit faire
 * @remark Si aucune distance n'est inclus, il va avancer en suivant la ligne
 */
void setGoal(int vitesse, MOUVEMENT mouvement, int distance = 0);


/**
 * Ajuste la vitesse des moteurs pour avoir la vitesse voulue dans @code setGoal()
 */
void ajustementPIDAutonome();


/**
 * Ajuste la vitesse des moteurs pour qu'il reste sur la ligne
 */
void ajustementPIDLigne();


/**
 * Arrêter le déplacement du robot.
 */
void arreter();


/**
 * Démarrer le déplacement du robot à une vitesse donnée.
 * Active les 2 moteurs jusqu'à ce qu'on les arrête.
 * @param vitesse Pourcentage de la vitesse à laquelle les moteurs doivent rouler.
 * La valeur doit être entre 0 et 1. 1 étant pour avancer à 100% de la vitesse possible.
 * \n TODO: Sera modifié pour utiliser un PID de suiveur de ligne.
 */
void avancer(float vitesse);


/**
 * Démarrer le déplacement du robot en marche arrière à une vitesse donnée.
 * Active les 2 moteurs jusqu'à ce qu'on les arrête.
 * @param vitesse Pourcentage de la vitesse à laquelle les moteurs doivent rouler.
 * La valeur doit être entre 0 et 1. 1 étant pour reculer à 100% de la vitesse possible.
 * TODO: Sera modifié pour utiliser un PID de suiveur de ligne.
 */
void reculer(float vitesse);


/**
 * Tourner le robot à gauche d'un certain nombre de degrés à une vitesse donnée.
 * @param direction LEFT ou RIGHT pour indiquer dans quelle direction tourner.
 * @param degres Nombre de degrés à tourner.
 * @param vitesse Pourcentage de la vitesse à laquelle les moteurs doivent rouler.
 * La valeur doit être entre 0 et 1. 1 étant pour tourner à 100% de la vitesse possible.
 * TODO: Sera modifié pour utiliser un PID. Ceci est un exemple.
 */
void tourner(bool direction, int degres, float vitesse);


/**
 * Ajuste la vitesse des moteurs selon le mode de PID choisi par @code mode_PID
 */
void ajusteVitesse();

#endif //AUTOBUS_MOTEUR_H
