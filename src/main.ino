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

/**
 * Classe pour utiliser le capteur de couleur.
 * Provient de la bibliothèque Adafruit_TCS34725.
 */
Adafruit_TCS34725 ColorSensor;

/**
 * Définition d'une couleur.
 * Contient les seuils de valeurs acceptées pour conclure que le capteur voit cette couleur.
 * @property name: Nom de la couleur.
 * @property redMin: Valeur minimum de rouge pour dire que c'est cette couleur.
 * @property redMax: Valeur maximum de rouge pour dire que c'est cette couleur.
 * @property greenMin: Valeur minimum de vert pour dire que c'est cette couleur.
 * @property greenMax: Valeur maximum de vert pour dire que c'est cette couleur.
 * @property blueMin: Valeur minimum de bleu pour dire que c'est cette couleur.
 * @property blueMax: Valeur maximum de bleu pour dire que c'est cette couleur.
 */
struct ColorDefinition {
    const char* name;
    uint16_t redMin, redMax;
    uint16_t greenMin, greenMax;
    uint16_t blueMin, blueMax;
};

/**
 * Liste des couleurs possibles qui peuvent être détectées par le capteur.
 * Tableau contenant des définitions de couleurs.
 */
ColorDefinition possibleColors[] = {
    {"NOIR", 0, 50, 0, 50, 0, 50},
    {"BLANC", 200, 255, 200, 255, 200, 255},
    {"ROUGE", 200, 255, 0, 100, 0, 100},
    {"VERT", 0, 100, 200, 255, 0, 100},
    {"BLEU", 0, 100, 0, 100, 200, 255},
    {"JAUNE", 200, 255, 200, 255, 0, 100},
    {"ROSE", 200, 255, 0, 100, 200, 255},
    {"MAUVE", 200, 255, 0, 100, 200, 255}
};

/**
 * Valeurs actuelles détectées par le capteur de couleur.
 * Ce sont les valeurs brutes lues du capteur. Elles ne sont pas encore ajustées
 * selon l'éclairage ambiant.
 */
uint16_t colorSensorRawRedValue, colorSensorRawGreenValue, colorSensorRawBlueValue, colorSensorRawBrightnessValue;

/**
 * Valeurs actuelles détectées par le capteur de couleur.
 * Ce sont les valeurs ajustées selon l'éclairage ambiant.
 */
float colorSensorRedValue, colorSensorGreenValue, colorSensorBlueValue;

/**
 * Keeps track of the sum of color values for averaging.
 */
float colorSensorSumR = 0, colorSensorSumG = 0, colorSensorSumB = 0;

/**
 * Valeurs de la couleur calculée après plusieurs échantillons.
 */
float averageRed = 0, averageGreen = 0, averageBlue = 0;

/**
 * Nombre d'échantillons valides pris pour le calcul de la moyenne.
 */
int numberValidSamples = 0;

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

    // Instanciation du capteur de couleur.
    ColorSensor = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);
    ColorSensor.begin();
    ColorSensor.setInterrupt(false);
  
    delay(50); // Délai d'initialisation du capteur de couleur.
}

/**
 * Fonctions de boucle infinie
 * Se fait appeler perpétuellement après que le setup() soit terminé.
 * Quand la fonction atteint la fin, elle recommence au début.
 * @note: Ne pas ajouter de delay() dans cette boucle.
 */
void loop() {
    // Réinitialiser les sommes et le compteur d'échantillons valides.
    colorSensorSumR = 0;
    colorSensorSumG = 0;
    colorSensorSumB = 0;
    numberValidSamples = 0;

    for (int i = 0; i < 10; i++) {
        // Lire les valeurs du capteur de couleur.
        ColorSensor.getRawData(
            &colorSensorRawRedValue,
            &colorSensorRawGreenValue,
            &colorSensorRawBlueValue,
            &colorSensorRawBrightnessValue
        );

        // Calculer les vraies valeurs de couleur en fonction de la luminosité.
        if (colorSensorRawBrightnessValue > 0) { // évite la division par zéro.
            colorSensorRedValue = (float)colorSensorRawRedValue / colorSensorRawBrightnessValue;
            colorSensorGreenValue = (float)colorSensorRawGreenValue / colorSensorRawBrightnessValue;
            colorSensorBlueValue = (float)colorSensorRawBlueValue / colorSensorRawBrightnessValue;

            colorSensorSumR += colorSensorRedValue;
            colorSensorSumG += colorSensorGreenValue;
            colorSensorSumB += colorSensorBlueValue;
            numberValidSamples++;
        }
        delay(20);
    }
    
    // Calculer la couleur moyenne.
    averageRed = colorSensorSumR / numberValidSamples;
    averageGreen = colorSensorSumG / numberValidSamples;
    averageBlue = colorSensorSumB / numberValidSamples;

    // Retourner le nom de la couleur correspondante.
    const char* detectedColor = "Inconnue";
    // foreach color in possibleColors.
    for (auto &color : possibleColors) {
        // Si dans le range de la couleur.
        if (
            averageRed >= color.redMin && averageRed <= color.redMax &&
            averageGreen >= color.greenMin && averageGreen <= color.greenMax &&
            averageBlue >= color.blueMin && averageBlue <= color.blueMax
        ) {
            // C'est cette couleur.
            detectedColor = color.name;
            break;
        }
    }

    delay(10); // Délai pour décharger le CPU.
}