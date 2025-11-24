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
#include "arret.h"

#include "../build\piolibdeps\megaatmega2560\Adafruit GFX Library\Fonts\FreeSans18pt7b.h"
#include "capteurs/detecteur_couleur.h"

#include "capteurs/detecteur_IR.h"
#include "capteurs/suiveur_ligne.h"

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
void lireCapteurs()
{
    // Vérifier si on est sur une station de bus.
    if (currentEtat != STATION_BUS && lireCapteurCouleur())
    {
        // Débuter l'état STATION_BUS.
        currentEtat = STATION_BUS;
        // Enregistrer quand le robot est arrivé à la station.
        tempsDebutTimerEtatRobot = millis();

        arreter();
    }
    // Vérifier s'il y a un obstacle devant le robot.
    else if (currentEtat != CONTOURNER_OBSTACLE && IR_ReadDistanceCm(FRONT) <= DistanceObstacle) {
        if (isMoving) {
            arreter();
            tempsDebutTimerContourner = millis();
        }
        // Si durant tout le delai d'attente d'obstacle il est resté là, on le contourne
        else if (tempsDebutTimerContourner + contourner_delay < millis()) {
            // Débuter l'état CONTOURNER_OBSTACLE.
            currentEtat = CONTOURNER_OBSTACLE;
        }
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
void setup()
{
    BoardInit(); // Initialisation de la carte RobUS.
        //     COLOR_SENSOR_init(); // Initialisation du détecteur de couleur.
        Serial.begin(9600); // Initialisation de la communication série pour le débogage.
        Serial.println("start");
        SUIVEUR_init();
    display.begin(0x3c, true); // Address 0x3C for 128x64
    display.setFont(&FreeSans18pt7b);

    // Réinitialiser les moteurs pour ne pas que le robot parte
    // à cause de la mise sous tension précédente.
    arreter();
    
    //     COLOR_SENSOR_init(); // Initialisation du détecteur de couleur.
    CLIGNOTANTS_init();
    SUIVEUR_init();
    
    pinMode(PIN_BUTTON, INPUT_PULLUP); // Définir la pin du bouton d'arrêt comme entrée.
    // mouvementMoteurs(0.3,SUIVRE_LA_LIGNE);
    pinMode(10, OUTPUT); // LED bleu
    pinMode(11, OUTPUT); // LED rouge
    pinMode(12, OUTPUT); // LED vert
    pinMode(13, OUTPUT); // LED jaune
    
    // Instanciation du capteur de couleur.
    ColorSensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
    ColorSensor.begin();
    ColorSensor.setInterrupt(false);

    // Tant que le bouton arrière n'est pas appuyé, vérifier si le bouton arrière est appuyé.
    while (!ROBUS_IsBumper(REAR)){
        currentEtat = SUIVRE_LIGNE;
    }
}

/**
 * Fonctions de boucle infinie
 * Se fait appeler perpétuellement après que le setup() soit terminé.
 * Quand la fonction atteint la fin, elle recommence au début.
 * @note: Ne pas ajouter de delay() dans cette boucle.
 */
void loop()
{
    //*** Lire les capteurs *********
    // Doit être effectué à toutes les loops.
    lireCapteurs();

    //*** Update l'état des clignotants du bus *********
    updateClignotant();

    //*** Ajuster la vitesse pour le mouvement *********
    if (isMoving) {
        isGoal(); // Vérifie s'il a fini le mouvement pour stopper
        ajusteVitesse(); // Met à jour les ajustement du PID/Suiveur de ligne
    }

    //*** Effectuer les actions de l'état actuel *********
    if (currentEtat == ARRET && previousEtat != ARRET)
    {
        arreter();
    }

    switch (currentEtat)
    {
    case SUIVRE_LIGNE:
        // Ne rien faire de plus le PID s'en occupe.
        if (!isMoving)
            suivreLigne(0.2);
        break;

    case CONTOURNER_OBSTACLE:
        contournerObstacle();
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
