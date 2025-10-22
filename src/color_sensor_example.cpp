/**
 * Projet: AutobUS
 * Equipe: P-12
 * Auteur: Adam Turcotte
 * Description: Boucle principale du robot. S'occupe des états du robot (avancer, arrêt, tourner).
 * Date: 2025-10-02
 */
#include <LibRobus.h> // Essentielle pour utiliser RobUS.
#include <Adafruit_TCS34725.h>

//Variable pour connaitre le sensor de couleur
Adafruit_TCS34725 colorSensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

/**
 * Fonction d'initialisation (Setup)
 * Exécutée une seule fois lorsque le robot est allumé.
 * Initialise les capteurs et prépare ce qui doit être prêt avant la loop().
 */
void setup() {
    BoardInit(); // Initialisation de la carte RobUS.
    Serial.begin(9600);
    //Met la led à on pour la lire
    colorSensor.setInterrupt(false);
}

/**
 * Fonctions de boucle infinie
 * Se fait appeler perpétuellement après que le setup() soit terminé.
 * Quand la fonction atteint la fin, elle recommence au début.
 * @note: Ne pas ajouter de delay() dans cette boucle.
 */
void loop() {

    //Variables pour enregistrer les valeurs de couleurs lues
    uint16_t r,g,b,clear;

    // //Met la led à on pour la lire
    // colorSensor.setInterrupt(false);

    delay(60);  // prend 50 ms pour lire

    //Lire les valeurs
    colorSensor.getRawData(&r,&g,&b,&clear);

    // //on remet la led à off après
    // colorSensor.setInterrupt(true);


    //Afficher les valeurs lues
    Serial.print("R:\t"); Serial.print(r);
    Serial.print("\tG:\t"); Serial.print(g);
    Serial.print("\tB:\t"); Serial.println(b);

    delay(10); // délai pour décharger le CPU.
}