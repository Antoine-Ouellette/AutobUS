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
enum COULEURS { NOIR, GRIS, BRUN, ROUGE, JAUNE, BLEU, BLANC };

constexpr COULEURS couleurs[nbCouleurs] = {NOIR, GRIS, BRUN, ROUGE, JAUNE, BLEU, BLANC}; //Tableau pour la conversion

typedef struct {
    uint16_t r;
    uint16_t g;
    uint16_t b;
} RGB;


constexpr int nbSamples = 4;
extern int sampleIndex;
extern RGB couleurSample[nbSamples];

constexpr int incertitude_DC = 20; // Écart accepté (incertitude détecteur couleur)

// TODO : ajuster pour les vraies valeurs des couleurs
constexpr RGB couleursDef[nbCouleurs] = {
    {0, 0, 0}, // Noir
    {258, 314, 259}, // Gris
    {187, 177, 129}, //Brun
    {320, 140, 120}, // Rouge
    {655 ,553 ,248}, // Jaune
    {110, 155, 156},// Bleu
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
