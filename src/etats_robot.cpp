/**
 * @file Une fonction pour chaque état possible du robot.
 * Exécute les actions associées à chaque état.
 */
#include <LibRobus.h> // Essentielle pour utiliser RobUS.
#include "variables_globales.h" // Inclure les variables globales partagées entre tous les fichiers.
#include "etats_robot.h"

/******************************************************************************
Fonctions
******************************************************************************/

/**
 * Réaction lorsque le robot atteint une station de bus.
 * 
 * Le robot met à jour l'écran du nom du prochain arrêt.
 * Si l'arrêt à cette station a été demandé, il attend 5 secondes.
 * Sinon, il ne s'arrête pas (-> SUIVRE_LIGNE).
 */
void reagirStation() {
    // Si l'arrêt à cette station a été demandé ET que ça fait 5 secondes que le robot est arrêté à la station de bus.
    // OU
    // Si l'arrêt à cette station n'a pas été demandé.
    if (
        !isArreterProchaineStation ||
        (isArreterProchaineStation && (millis() - tempsDebutTimerEtatRobot >= ARRET_STATION_DELAY))
    ) {
        // Mettre à jour l'écran avec le nom du prochain arrêt.
        // changerTexteEcran();

        // Réinitialiser le bouton d'arrêt demandé.
        isArreterProchaineStation = false;

        // À la fin, on enregistre que l'état qu'on vient de terminer est STATION_BUS.
        previousEtat = STATION_BUS;

        // Retourner à suivre la ligne.
        currentEtat = SUIVRE_LIGNE;
    }
}