/**
* @file Fonctions en lien avec les moteurs des roues du robot.
 * Inclut les déplacements et le PID.
 */

#ifndef AUTOBUS_MOTEUR_H
#define AUTOBUS_MOTEUR_H

/**
 * SUIVRE_LIGNE : Quand le Robot veut suivre la ligne.\n
 * AVANCE : Le Robot avance en ligne droite par en avant.\n
 * RECULE : Le Robot avance en ligne droite par en arrière.\n
 * TOUR_GAUCHE : Le Robot tourne vers la gauche avec le décalage de son centre voulu.\n
 * TOUR_DROIT : Le Robot tourne vers la droite avec le décalage de son centre voulu.\n
 */
enum MOUVEMENT { SUIVRE_LA_LIGNE, TOUR_GAUCHE, TOUR_DROIT, AVANCE, RECULE };

/**
 * Reset les deux encodeurs
 */
void ENCODERS_Reset();

void resetPIDStats();

/**
 * Permet de définir le type et le mouvement que le robot doit faire.
 * @param vitesse Pourcentage de la vitesse à laquelle les moteurs doivent rouler.
 * La valeur doit être entre 0 et 1. 1 étant pour reculer à 100% de la vitesse possible.
 * @param mouvement Mouvement que le robot veut faire {TOUR_GAUCHE, TOUR_DROIT, AVANCE, RECULE}
 * @param distance Distance en cm / Angle en degré.
 * @param rayon  Rayon de courbure quand il tourne, la distance de son point de rotation.
 *
 * @attention Voici les combinaisons possibles :\n
 * @code ({vit})@endcode : suivre ligne\n
 * @code ({vit}, {mouv}, {dist})@endcode : avancer/tourner sur place\n
 * @code ({vit}, {mouv}, {dist}, {rayon})@endcode tourner à un rayon
 */
void mouvementMoteurs(float vitesse, MOUVEMENT mouvement = SUIVRE_LA_LIGNE, float distance = 0, float rayon = 0);

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

/**
 * Fait avancer le robot a une vitesse
 * @param speed Pourcentage de la vitesse 0 - 1, de 0% - 100%
 */
void avancer(float speed);

/**
 * Fait avancer le robot avec une vitesse différente dans chaque roue.
 * @param speedLeft Pourcentage de la vitesse 0-1, de 0%-100%
 * @param speedRight Pourcentage de la vitesse 0-1, de 0%-100%
 */
void avancer(float speedLeft,float speedRight) ;


#endif //AUTOBUS_MOTEUR_H
