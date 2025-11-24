/**
 * @file Une fonction pour chaque état possible du robot.
 * Exécute les actions associées à chaque état.
 */
#include <LibRobus.h> // Essentielle pour utiliser RobUS.
#include "variables_globales.h" // Inclure les variables globales partagées entre tous les fichiers.
#include "etats_robot.h"



Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

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
void affichage_ecran(Arret arret) {
    display.clearDisplay();
    display.setTextSize(1);      // Normal 1:1 pixel scale
    display.setTextColor(SH110X_WHITE); // Draw white text
    display.setCursor(0, 28);    // Start at top-left corner
    display.println(arret.nom_arret);
    display.display();
}

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
        affichage_ecran(arrets[numero_arret]);
        
        

        // Réinitialiser le bouton d'arrêt demandé.
        isArreterProchaineStation = false;

        // À la fin, on enregistre que l'état qu'on vient de terminer est STATION_BUS.
        previousEtat = STATION_BUS;

        // Retourner à suivre la ligne.
        currentEtat = SUIVRE_LIGNE;
    }
    numero_arret++;
}