/**
 * Projet: AutobUS
 * Equipe: P-12
 * Auteurs: Aimeric Bouillon, Mathieu Cabana, Samuel Houle, William Larouche,
 * Alexi Ledoux, Antoine Ouellette, Adam Turcotte, Samy Yamouni Description:
 * Boucle principale du robot. S'occupe des états du robot (avancer, arrêt,
 * tourner). Date: 2025-10-02
 */
#include "actions_stations.h" // Inclure les actions à faire pour chaque station.
#include "detecteur_couleur.h" // Inclure les fonctions en lien avec le détecteur de couleurs.
#include "moteur.h" // Inclure les fonctions en lien avec les moteurs des roues.
#include "suiveur_ligne.h" // Inclure les fonctions en lien avec le suiveur de ligne.
#include "variables_globales.h" // Inclure les variables globales partagées entre tous les fichiers.
#include <LibRobus.h> // Essentielle pour utiliser RobUS.
unsigned long lastUpdatePID = 0;
int losange = 0;
/**
 * Fonction d'initialisation (Setup)
 * Exécutée une seule fois lorsque le robot est allumé.
 * Initialise les capteurs et prépare ce qui doit être prêt avant la loop().
 */
void setup() {
    BoardInit();         // Initialisation de la carte RobUS.
    SUIVEUR_init();      // Initialisation du suiveur de ligne.
    COLOR_SENSOR_init(); // Initialisation du détecteur de couleur.

    // Initialisation de la communication série pour le débogage.
    Serial.begin(9600);

    // Réinitialiser les moteurs pour ne pas que le robot parte
    // à cause de la mise sous tension précédente.
    arreter();

    // Tant que le bouton arrière n'est pas appuyé, vérifier si le bouton
    // arrière est appuyé.
    // TODO: Remplacer par la detection du sifflet.
    while (!ROBUS_IsBumper(REAR))
        ;
    // currentEtat = SUIVRE_LIGNE; // Définir l'état initial du robot.
    // setGoal(0.4, RECULE, 45);
}

/**
 * Fonctions de boucle infinie
 * Se fait appeler perpétuellement après que le setup() soit terminé.
 * Quand la fonction atteint la fin, elle recommence au début.
 * @note: Ne pas ajouter de delay() dans cette boucle.
 */
void loop() {

    if (isMoving) {
        ajusteVitesse();
    }
    // isGoal();

    switch (losange) {
    case 0:
        setGoal(0.2, AVANCE, 20);
        losange++;
        break;

    case 1:
        if (isGoal()) {
            losange++;
        }
        break;

    case 2:
        setGoal(0.2, TOUR_DROIT, 45);
        losange++;
        break;

    case 3:
        if (isGoal()) {
            losange++;
        }
        break;

    case 4:
        setGoal(0.2, RECULE, 20);
        losange++;
        break;

    case 5:
        if (isGoal()) {
            losange++;
        }
        break;

    case 6:
        setGoal(0.2, TOUR_GAUCHE, 90);
        losange++;
        break;

    case 7:
        if (isGoal()) {
            losange++;
        }
        break;

    case 8:
        setGoal(0.2, RECULE, 20);
        losange++;
        break;

    case 9:
        if (isGoal()) {
            losange++;
        }
        break;

    case 10:
        setGoal(0.2, TOUR_DROIT, 90);
        losange++;
        break;

    case 11:
        if (isGoal()) {
            losange++;
        }
        break;

    case 12:
        setGoal(0.2, AVANCE, 20);
        losange++;
        break;

    case 13:
        if (isGoal()) {
            losange++;
        }
        break;

    case 14:
        setGoal(0.2, TOUR_GAUCHE, 90);
        losange++;
        break;

    case 15:
        if (isGoal()) {
            losange++;
        }
        break;

    case 16:
        setGoal(0.2, AVANCE, 20);
        losange++;
        break;

    case 17:
        if (isGoal()) {
            losange++;
        }
        break;
    };

    delay(10); // Délai pour décharger le CPU.
}
