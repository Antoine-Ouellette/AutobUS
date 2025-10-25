/**
 * Projet: AutobUS
 * Equipe: P-12
 * Auteurs: Aimeric Bouillon, Mathieu Cabana, Samuel Houle, William Larouche, Alexi Ledoux, Antoine Ouellette, Adam Turcotte, Samy Yamouni
 * Description: Boucle principale du robot. S'occupe des états du robot (avancer, arrêt, tourner).
 * Date: 2025-10-02
 */
#include <LibRobus.h> // Essentielle pour utiliser RobUS.
#include "variables_globales.h" // Inclure les variables globales partagées entre tous les fichiers.
#include "actions_stations.h" // Inclure les actions à faire pour chaque station.
#include "moteur.h" // Inclure les fonctions en lien avec les moteurs des roues.

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
    // currentMillis = millis(); // Mettre à jour le temps actuel en millisecondes.

    // Si l'état a changé.
    if (previousEtat != currentEtat) {
        switch (currentEtat) {
            case ARRET:
                // arreter();
                break;
            case SUIVRE_LIGNE:
                // suivreLigne();
                break;
            case CONTOURNER_OBSTACLE:
                // contournerObstacle();
                break;
            case QUILLE:
                // renverserQuille();
                break;
            case DANSE:
                // danserLosange();
                break;
            case PAS_LIGNE:
                // avancerTrouverLigne();
                break;
        }

        // À la fin, enregistrer le nouvel état précédent.
        previousEtat = currentEtat;
    } else { }
    
    delay(10); // Délai pour décharger le CPU.
}