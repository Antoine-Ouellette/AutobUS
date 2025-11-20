/**
 * Projet: AutobUS
 * Equipe: P-12
 * Auteurs: Aimeric Bouillon, Mathieu Cabana, Samuel Houle, William Larouche, Alexi Ledoux, Antoine Ouellette, Adam Turcotte, Samy Yamouni
 * Description: Boucle principale du robot. S'occupe des états du robot (avancer, arrêt, tourner).
 * Date: 2025-10-02
 */
#include <LibRobus.h> // Essentielle pour utiliser RobUS.
#include <Adafruit_TCS34725.h>

/******************************************************************************
Variables globales et #define
L'ensemble des fonctions y ont accès.
******************************************************************************/
#define PIN_BUTTON 49

/******************************************************************************
Fonctions
Les fonctions doivent être déclarées avant d'être utilisées.
******************************************************************************/

/**
 * Fonction d'initialisation (Setup)
 * Exécutée une seule fois lorsque le robot est allumé.
 * Initialise les capteurs et prépare ce qui doit être prêt avant la loop().
 */
void setup() {
    Serial.begin(9600);
    BoardInit(); // Initialisation de la carte RobUS.

    pinMode(PIN_BUTTON, INPUT_PULLUP);

    delay(50); // Délai d'initialisation du capteur de couleur.
}

/**
 * Enregistre l'état du bouton d'arrêt demandé.
 * L'état initial est tension à HIGH (pas appuyé).
 */
int buttonState = HIGH;

/**
 * Enregistre l'état précédent du bouton d'arrêt demandé.
 */
int lastButtonState = HIGH;

/**
 * Enregistre le temps au dernier changement d'état du bouton.
 */
long lastChangeTime = 0;

/**
 * Indique si le robot doit s'arrêter à la prochaine station de bus.
 */
bool isArreterProchaineStation = false;

/**
 * Fonctions de boucle infinie
 * Se fait appeler perpétuellement après que le setup() soit terminé.
 * Quand la fonction atteint la fin, elle recommence au début.
 * @note: Ne pas ajouter de delay() dans cette boucle.
 */
void loop() {
    // Read current state of button pin.
    int nowButtonState = digitalRead(PIN_BUTTON);
    Serial.print("Button state: "); Serial.println(nowButtonState);

    // If button pin state has changed, record the time point.
    if (nowButtonState != lastButtonState) {
        lastChangeTime = millis();
    }

    // If button state changes, and stays stable for a while, then it should have skipped the bounce area.
    if (millis() - lastChangeTime > 10) {
        if (buttonState != nowButtonState) { // Only proceed if button state has changed.
            buttonState = nowButtonState;
            if (buttonState == LOW) { // Low level indicates the button is pressed.
                isArreterProchaineStation = true;
                Serial.println("Arrêt demandé.");
            }
        }
    }

    // Save the latest state for next time.
    lastButtonState = nowButtonState;

    delay(200); // Délai pour décharger le CPU.
}