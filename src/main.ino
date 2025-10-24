/**
 * Projet: AutobUS
 * Equipe: P-12
 * Auteurs: Aimeric Bouillon, Mathieu Cabana, Samuel Houle, William Larouche, Alexi Ledoux, Antoine Ouellette, Adam Turcotte, Samy Yamouni
 * Description: Boucle principale du robot. S'occupe des états du robot (avancer, arrêt, tourner).
 * Date: 2025-10-02
 */
#include <LibRobus.h> // Essentielle pour utiliser RobUS.
#include "variables_globales.h" // Inclure les variables globales partagées entre tous les fichiers.
#include "lecture_capteurs.cpp" // Inclure les fonctions de lecture des capteurs.
#include "moteurs.cpp" // Inclure les fonctions de contrôle des moteurs des roues.
#include "etats_robot.cpp" // Inclure les actions à effectuer pour chaque état du robot.

/******************************************************************************
Variables et #define
accessibles seulement dans ce fichier.
******************************************************************************/

/******************************************************************************
Fonctions
Les fonctions doivent être déclarées avant d'être utilisées.
******************************************************************************/

/**
 * Lit les capteurs du robot et met à jour les variables globales.
 * Change l'état du robot si nécessaire.
 * L'état n'est pas changé si les capteurs n'en indiquent pas le besoin.
 */
void lireCapteurs() {
    // Vérifier si on est sur une station de bus.
    if (currentEtat != STATION_BUS && lireCapteurCouleur()) {
        // Débuter l'état STATION_BUS.
        currentEtat = STATION_BUS;
        // Enregistrer quand le robot est arrivé à la station.
        tempsDebutTimerEtatRobot = millis();
    }
    // Vérifier s'il y a un obstacle devant le robot.
    else if (currentEtat != CONTOURNER_OBSTACLE && lireCapteurProximite()) {
        // Débuter l'état CONTOURNER_OBSTACLE.
        currentEtat = CONTOURNER_OBSTACLE;
    }
    // Vérifier si le bouton Arrêt demandé est appuyé.
    else if (!isArreterProchaineStation && lireBoutonArretDemande()) {
        // Indiquer qu'il faut s'arrêter quand on va atteindre la prochaine station.
        isArreterProchaineStation = true;
    }
}

/**
 * Fonction d'initialisation (Setup)
 * Exécutée une seule fois lorsque le robot est allumé.
 * Initialise les capteurs et prépare ce qui doit être prêt avant la loop().
 */
void setup() {
    BoardInit(); // Initialisation de la carte RobUS.

    // Réinitialiser les moteurs pour ne pas que le robot parte
    // à cause de la mise sous tension précédente.
    arreter();

    // Tant que le bouton arrière n'est pas appuyé, vérifier si le bouton arrière est appuyé.
    // TODO: Remplacer par la detection du sifflet.
    while (!ROBUS_IsBumper(REAR));
}

/**
 * Fonctions de boucle infinie
 * Se fait appeler perpétuellement après que le setup() soit terminé.
 * Quand la fonction atteint la fin, elle recommence au début.
 * @note: Ne pas ajouter de delay() dans cette boucle.
 */
void loop() {
    //*** Lire les capteurs *********
    // Doit être effectué à toutes les loops.
    lireCapteurs();
    
    //*** Effectuer les actions de l'état actuel *********
    if (currentEtat == ARRET && previousEtat != ARRET) {
        arreter();
    }

    switch (currentEtat) {
        case SUIVRE_LIGNE:
            suivreLigne();
            break;

        case CONTOURNER_OBSTACLE:
            contournerObstacle();
            break;
        
        case STATION_BUS:
            reagirStation();
            break;
    }
    
    delay(10); // délai pour décharger le CPU.
}