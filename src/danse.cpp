/**
 * Projet: AutobUS
 * Equipe: P-12
 * Auteurs: Aimeric Bouillon, Mathieu Cabana, Samuel Houle, William Larouche, Alexi Ledoux, Antoine Ouellette, Adam Turcotte, Samy Yamouni
 * Description: Boucle principale du robot. S'occupe des états du robot (avancer, arrêt, tourner).
 * Date: 2025-10-02
 */
#include <LibRobus.h>           // Essentielle pour utiliser RobUS.
#include "variables_globales.h" // Inclure les variables globales partagées entre tous les fichiers.
#include "actions_stations.h" // Inclure les actions à faire pour chaque station.
#include "moteur.h" // Inclure les fonctions en lien avec les moteurs des roues.
#include "detecteur_couleur.h" // Inclure les fonctions en lien avec le détecteur de couleurs.
#include "suiveur_ligne.h"      // Inclure les fonctions en lien avec le suiveur de ligne.
unsigned long lastUpdatePID = 0;
int losange = 0;
/**
 * Fonction d'initialisation (Setup)
 * Exécutée une seule fois lorsque le robot est allumé.
 * Initialise les capteurs et prépare ce qui doit être prêt avant la loop().
 */
void setup() {
    BoardInit(); // Initialisation de la carte RobUS.
    SUIVEUR_init(); // Initialisation du suiveur de ligne.
    COLOR_SENSOR_init(); // Initialisation du détecteur de couleur.

    Serial.begin(9600); // Initialisation de la communication série pour le débogage.


    // Réinitialiser les moteurs pour ne pas que le robot parte
    // à cause de la mise sous tension précédente.
    arreter();

    // Tant que le bouton arrière n'est pas appuyé, vérifier si le bouton arrière est appuyé.
    // TODO: Remplacer par la detection du sifflet.
    while (!ROBUS_IsBumper(REAR));
    // currentEtat = SUIVRE_LIGNE; // Définir l'état initial du robot.

}

/**
 * Fonctions de boucle infinie
 * Se fait appeler perpétuellement après que le setup() soit terminé.
 * Quand la fonction atteint la fin, elle recommence au début.
 * @note: Ne pas ajouter de delay() dans cette boucle.
 */
void loop() {
    switch (losange){
    case 0:
    setGoal(0.4, AVANCE, 15);
    if(isGoal()){
        losange++;
    }
    break;

    case 1:
    setGoal(0.4, TOUR_DROIT, 45);
    if(isGoal()){
        losange++;
    }
    break;

    case 2:
        setGoal(0.4, RECULE, -10);
    if(isGoal()){
        losange++;
    }
    break;

        case 3:
        setGoal(0.4, TOUR_GAUCHE, 90);
    if(isGoal()){
        losange++;
    }
    break;

        case 4:
        setGoal(0.4, RECULE, 10);
    if(isGoal()){
        losange++;
    }
    break;

        case 5:
        setGoal(0.4, TOUR_DROIT, 90);
    if(isGoal()){
        losange++;
    }
    break;

        case 6:
        setGoal(0.4, AVANCE, 10);
    if(isGoal()){
        losange++;
    }
    break;

        case 7:
        setGoal(0.4, TOUR_GAUCHE, 90);
    if(isGoal()){
        losange++;
    }
    break;

        case 8:
        setGoal(0.4, AVANCE, 10);
    if(isGoal()){
        losange++;
    }
    break;
}


    delay(10); // Délai pour décharger le CPU.
}
