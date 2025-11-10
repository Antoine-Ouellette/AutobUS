/**
 * @file Fonctions pour la lecture de chaque capteur.
 */

#ifndef LECTURE_CAPTEURS_H
#define LECTURE_CAPTEURS_H
/**
 * Lit l'état du capteur de couleur.
 * Détecte s'il y a une station de bus en dessous du robot.
 * @return true si une station de bus est détectée, false sinon.
 */
bool lireCapteurCouleur();

/**
 * Lit l'état du capteur de proximité.
 * Détecte s'il y a un obstacle devant le robot.
 * @return true si un obstacle est détecté, false sinon.
 */
bool lireCapteurProximite();

/**
 * Lit l'état du bouton d'arrêt demandé.
 * @return true si le bouton est appuyé, false sinon.
 */
bool lireBoutonArretDemande();
#endif //LECTURE_CAPTEURS_H