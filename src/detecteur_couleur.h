/**
 * Fonction en lien avec le détecteur de couleurs
 * Inclue la lecture du capteur et la conversion vers une valeur de l'enum COULEURS
 */

#ifndef AUTOBUS_DETECTEUR_COULEUR_H
#define AUTOBUS_DETECTEUR_COULEUR_H

// Nombre de couleurs de l'enum COULEURS
constexpr int nbCouleurs = 5;


/**
 * @enum COULEURS
 * @brief Enum des couleurs que le capteur pourra détecter :
 * NOIR, ROUGE, VERT, BLEU, BLANC
 */
enum COULEURS { NOIR, ROUGE, VERT, BLEU, BLANC };

/**
 * Initialise le détecteur de couleurs
 */
void initColorSensor();

/**
 * @brief Lit la valeur du capteur et retourne une valeur selon COULEURS
 * @return Une couleur selon   COULEURS
 * @remarks Retourne NOIR s'il ne reconnait aucune couleur
 */
COULEURS COLORSENSOR_Read();

/**
 * @brief Traduit un int vers COULEURS
 * @param n valeur en int de la couleur
 * @return valeur équivalent dans l'enum, retourne NOIR par defaut
 */
COULEURS intToColor(int n);

#endif //AUTOBUS_DETECTEUR_COULEUR_H
