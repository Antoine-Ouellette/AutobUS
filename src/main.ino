/**
 * Projet: AutobUS
 * Equipe: P-12
 * Auteurs: Aimeric Bouillon, Mathieu Cabana, Samuel Houle, William Larouche, Alexi Ledoux, Antoine Ouellette, Adam Turcotte, Samy Yamouni
 * Description: Boucle principale du robot. S'occupe des états du robot (avancer, arrêt, tourner).
 * Date: 2025-10-02
 */
#include <LibRobus.h> // Essentielle pour utiliser RobUS.
#include <Adafruit_TCS34725.h> // Bibliothèque pour le capteur de couleur.
#include "variables_globales.h" // Inclure les variables globales partagées entre tous les fichiers.
#include "lecture_capteurs.h"   // Inclure les fonctions de lecture des capteurs.
#include "etats_robot.h"        // Inclure les actions à effectuer pour chaque état du robot.
#include "moteur.h"
#include "actions.h"
#include "arret_bus.h"
#include "ecran.h"
#include "capteurs/detecteur_IR.h"
#include "capteurs/manette_IR.h"
#include "capteurs/suiveur_ligne.h"

int nbLoop = 0;

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
    if (currentEtat != STATION_BUS &&
        isArret() &&
        tempsDebutTimerEtatRobot + 2000 < millis() &&
        currentEtat != CONTOURNER_OBSTACLE) {
#if CONSOLE_DEBUG
        Serial.println("[ARRET] début de la detection");
#endif
        // Débuter l'état STATION_BUS.
        currentEtat = STATION_BUS;

        //Faire son stop pour l'arret de bus
        arreter();

        // Enregistrer quand le robot est arrivé à la station.
        tempsDebutTimerEtatRobot = millis();
    }
    // Vérifier s'il y a un obstacle devant le robot.
    else if (currentEtat != CONTOURNER_OBSTACLE && IR_ReadDistanceCm(FRONT) <= DistanceObstacle) {
#if CONSOLE_DEBUG
        Serial.println("[CONTOURNE] arret et attente");
#endif

        // Débuter l'état CONTOURNER_OBSTACLE.
        currentEtat = CONTOURNER_OBSTACLE;
    }
    // Vérifier si le bouton Arrêt demandé est appuyé.
    else if (!isArreterProchaineStation) {
        lireBoutonArretDemande();
    }
}

/**
 * Fonction d'initialisation (Setup)
 * Exécutée une seule fois lorsque le robot est allumé.
 * Initialise les capteurs et prépare ce qui doit être prêt avant la loop().
 */
void setup() {
#if CONSOLE_DEBUG
    Serial.begin(9600); // Initialisation de la communication série pour le débogage.
#endif
    BoardInit(); // Initialisation de la carte RobUS.


    // Réinitialiser les moteurs pour ne pas que le robot parte
    // à cause de la mise sous tension précédente.
    arreter();

    CLIGNOTANTS_init();
    SUIVEUR_init();
    COLOR_SENSOR_init(); // Initialisation du détecteur de couleur.
    LumiereArret_init();
    ECRAN_init();

    afficherProchainArret();

    pinMode(PIN_BUTTON, INPUT_PULLUP); // Définir la pin du bouton d'arrêt comme entrée.
    pinMode(10, OUTPUT); // LED bleu
    pinMode(11, OUTPUT); // LED rouge
    pinMode(12, OUTPUT); // LED vert
    pinMode(13, OUTPUT); // LED jaune

    //Fait qu'il suit la ligne
    mouvementMoteurs(VitesseSuivreLigne, SUIVRE_LA_LIGNE);


#if CONSOLE_DEBUG
    Serial.println("[Setup] finished");
#endif

    // Tant que le bouton arrière n'est pas appuyé, vérifier si le bouton arrière est appuyé.
    while (!ROBUS_IsBumper(REAR) && REMOTE_read() == 0 && digitalRead(PIN_BUTTON));

#if CONSOLE_DEBUG
    Serial.println("[Setup] starting loop");
#endif
    delay(1000);
    // mouvementMoteurs(0.3, AVANCE, 100);
}

/**
 * Fonctions de boucle infinie
 * Se fait appeler perpétuellement après que le setup() soit terminé.
 * Quand la fonction atteint la fin, elle recommence au début.
 * @note: Ne pas ajouter de delay() dans cette boucle.
 */
void loop() {
    nbLoop++;

    //*** Lire les capteurs *********
    // Doit être effectué à toutes les loops.
    lireCapteurs();

    // Fait les lectures du sensor de couleur pour faire la moyenne.
    COLOR_SENSOR_update();

    //*** Update l'état des clignotants du bus *********
    if (nbLoop % 4 == 0) {
        updateClignotant();
    }

    // Réagi à la lecture fait par la manette
    if (nbLoop % 2 == 0) {
        reagirManetteIR();
    }

    // Met a jour l'état de la lumière de demande d'arret
    if (nbLoop % 6 == 0) {
        updateLumiereArret();
    }

    //*** Ajuster la vitesse pour le mouvement *********
    if (isMoving) {
        isGoal(); // Vérifie s'il a fini le mouvement pour stopper
        ajusteVitesse(); // Met à jour les ajustement du PID/Suiveur de ligne
    }

    //*** Effectuer les actions de l'état actuel *********
    if (currentEtat == ARRET && previousEtat != ARRET) {
        arreter();
    }

    switch (currentEtat) {
        case SUIVRE_LIGNE:
            // Ne rien faire de plus le PID s'en occupe.
            if (!isMoving)
                suivreLigne(VitesseSuivreLigne);
            break;

        case CONTOURNER_OBSTACLE:
            //contournerObstacle();
            break;

        case STATION_BUS:
            reagirStation();
            break;
        case ARRET:
            //Il est déjà géré plus haut.
            break;
        default:
            break;
    }

    delay(10); // délai pour décharger le CPU.
}
