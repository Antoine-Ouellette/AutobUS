/**
 * Fonction en lien avec le détecteur de couleurs
 * Inclue la lecture du capteur et la conversion vers une valeur de l'enum COULEURS
 */

#ifndef AUTOBUS_DETECTEUR_COULEUR_H
#define AUTOBUS_DETECTEUR_COULEUR_H
#include <Adafruit_TCS34725.h>

extern Adafruit_TCS34725 colorSensor;

// Nombre de couleurs de l'enum COULEURS
constexpr int nbCouleurs = 7;


/**
 * @enum COULEURS
 * @brief Enum des couleurs que le capteur pourra détecter :
 * NOIR, ROUGE, VERT, BLEU, BLANC
 */
enum COULEURS { NOIR, VERT, BRUN, ROUGE, JAUNE, BLEU, BLANC };

constexpr COULEURS couleurs[nbCouleurs] = {NOIR, VERT, BRUN, ROUGE, JAUNE, BLEU, BLANC}; //Tableau pour la conversion

typedef struct {
    uint16_t r;
    uint16_t g;
    uint16_t b;
    uint16_t c;
} RGBC;


constexpr int nbSamples = 5;
extern int sampleIndex;
extern RGBC couleurSample[nbSamples];

constexpr int incertitude_DC = 30; // Écart accepté (incertitude détecteur couleur)

// TODO : ajuster pour les vraies valeurs des couleurs
constexpr RGBC couleursDef[nbCouleurs] = {
    {96, 126, 101, 336}, // Noir
    {124 ,180, 117, 443}, // Vert
    {149 ,143, 107, 419}, //Brun
    {265 ,126, 112, 507}, // Rouge
    {472 ,422, 201, 1140}, // Jaune
    { 93 ,146, 169, 423}, // Bleu
};


/**
 * Initialise le détecteur de couleurs
 */
void COLOR_SENSOR_init();

/**
 * Fait une nouvelle lecture du capteur pour mettre à jour la moyenne de la couleur lue.
 * @remark Doit être appelé dans le loop
 */
void COLOR_SENSOR_update();

/**
 * @brief Lit la valeur du capteur et retourne une valeur selon COULEURS
 * @return Une couleur selon COULEURS
 * @remarks Retourne NOIR s'il ne reconnait aucune couleur
 */
COULEURS COLOR_SENSOR_Read();

/**
 * @brief Traduit un int vers COULEURS
 * @param n valeur en int de la couleur
 * @return valeur équivalent dans l'enum, retourne NOIR par defaut
 */
COULEURS intToColor(int n);

#endif //AUTOBUS_DETECTEUR_COULEUR_H
