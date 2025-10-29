/**
* @file Fonctions en lien avec les moteurs des roues du robot.
 * Inclut les déplacements et le PID.
 */

#ifndef AUTOBUS_MOTEUR_H
#define AUTOBUS_MOTEUR_H

enum MOUVEMENT { TOUR_GAUCHE, TOUR_DROIT, AVANCE, RECULE };

/**
 * Reset les deux encodeurs
 */
void ENCODERS_Reset();

void resetPIDStats();

/**
 * Permet d'établir l'objectif de chaque moteur, quand on veut se déplacer.
 * @param vitesse Pourcentage de la vitesse à laquelle les moteurs doivent rouler.
 * La valeur doit être entre 0 et 1. 1 étant pour reculer à 100% de la vitesse possible.
 * @param mouvement Mouvement que le robot veut faire {TOUR_GAUCHE, TOUR_DROIT, AVANCE, RECULE}
 * @param distance Distance en cm / Angle en degré. Cela dépend s'il avance ou tourne.
 * Si aucune distance n'est inclus, il va avancer en suivant la ligne
 */
void setGoal(float vitesse, MOUVEMENT mouvement, float distance = 0);

/**
 * Calcule l'avancement de completion pour les deux encodeurs
 * met l'avancement dans @code encCompletion @endcode entre 0 et 1
 */
void calcEncCompletion();

/**
 * Fonction qui permet de savoir si le robot a fini son mouvement. Si le mouvement est terminé, il va arreter tout seul.
 * @return true : si le robot a fini le mouvement demandé, sinon false
 */
bool isGoal();

/**
 * Ajuste la vitesse des moteurs pour qu'il avance droit avec le PID
 */
void ajusteVitesse();


/**
 * Arrêter le déplacement du robot.
 */
void arreter();


#endif //AUTOBUS_MOTEUR_H
