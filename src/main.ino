/**
 * Projet: AutobUS
 * Equipe: P-12
 * Auteurs: Aimeric Bouillon, Mathieu Cabana, Samuel Houle, William Larouche, Alexi Ledoux, Antoine Ouellette, Adam Turcotte, Samy Yamouni
 * Description: Boucle principale du robot. S'occupe des états du robot (avancer, arrêt, tourner).
 * Date: 2025-10-02
 */
#include <LibRobus.h>           // Essentielle pour utiliser RobUS.
#include "variables_globales.h" // Inclure les variables globales partagées entre tous les fichiers.
#include "actions_stations.h"   // Inclure les actions à faire pour chaque station.
#include "moteur.h"             // Inclure les fonctions en lien avec les moteurs des roues.
#include "detecteur_couleur.h"  // Inclure les fonctions en lien avec le détecteur de couleurs.
#include "suiveur_ligne.h"      // Inclure les fonctions en lien avec le suiveur de ligne.
#include "sifflet.h"

unsigned long lastUpdatePID = 0;
int moves = 0;
/**
 * Fonction d'initialisation (Setup)
 * Exécutée une seule fois lorsque le robot est allumé.
 * Initialise les capteurs et prépare ce qui doit être prêt avant la loop().
 */
void setup()
{
    BoardInit();         // Initialisation de la carte RobUS.
    SUIVEUR_init();      // Initialisation du suiveur de ligne.
    COLOR_SENSOR_init(); // Initialisation du détecteur de couleur.
    init_sifflet();

    Serial.begin(9600); // Initialisation de la communication série pour le débogage.

    for (int i = 0; i < 4; i++)
    {
        pinMode(leds[i], OUTPUT);
    }

    // Réinitialiser les moteurs pour ne pas que le robot parte
    // à cause de la mise sous tension précédente.
    arreter();

    // Tant que le bouton arrière n'est pas appuyé, vérifier si le bouton arrière est appuyé.
    // TODO: Remplacer par la detection du sifflet.
    while (true) {
        if (ROBUS_IsBumper(REAR) || detection_sifflet()) {
            break;
        }
    };
    currentEtat = SUIVRE_LIGNE; // Définir l'état initial du robot.
}

/**
 * Fonctions de boucle infinie
 * Se fait appeler perpétuellement après que le setup() soit terminé.
 * Quand la fonction atteint la fin, elle recommence au début.
 * @note: Ne pas ajouter de delay() dans cette boucle.
 */
void loop()
{
    currentMillis = millis(); // Mettre à jour le temps actuel en millisecondes.

    if (isMoving)
    {
        ajusteVitesse();
        lastUpdatePID = currentMillis;
    }

    if (isGoal())
    {
        // Si on veut faire quelque chose quand il a fini.
    }
    if (currentEtat == SUIVRE_LIGNE) {
        switch (COLORSENSOR_Read()) {
            case ROUGE:
                digitalWrite(leds[1], HIGH);
                currentEtat = QUILLE;
                break;
            case VERT:
                digitalWrite(leds[2], HIGH);
                currentEtat = PAS_LIGNE;
                break;
            case BLEU:
                digitalWrite(leds[0], HIGH);
                currentEtat = DANSE;
                break;
            case JAUNE:
                digitalWrite(leds[3], HIGH);
                currentEtat = CONTOURNER_OBSTACLE;
                break;
            default:
                for (int i = 0; i < 4; i++) {
                    digitalWrite(leds[i], LOW);
                }
                break;
        }
    }

    // Si l'état a changé.
    if (previousEtat != currentEtat)
    {
        arreter(); // Arrêter le robot avant de changer d'état.
    }

    switch (currentEtat)
    {
    case ARRET:
        // arreter();
        break;
    case SUIVRE_LIGNE:
        suivreLigne();
        break;
    case CONTOURNER_OBSTACLE:
        contournerObstacle();
        break;
    case QUILLE:
        renverserQuille();
        break;
    case DANSE:
        danserLosange();
        break;
    case PAS_LIGNE:
        avancerTrouverLigne();
        break;
    case RETROUVER_LIGNE:
        retrouverLigne();
        break;
    }

    // À la fin, enregistrer le nouvel état précédent.
    previousEtat = currentEtat;

    delay(10); // Délai pour décharger le CPU.
}
