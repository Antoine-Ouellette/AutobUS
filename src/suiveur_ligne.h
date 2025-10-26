/**
 * @file Fonction en lien avec le suiveur de ligne
 * Inclue la lecture du capteur et la conversion vers une valeur binaire
 */

#ifndef AUTOBUS_SUIVEUR_LIGNE_H
#define AUTOBUS_SUIVEUR_LIGNE_H
#include <stdint.h>

/**
 * Ajuste les seuils de détections selon ce qu'il voit sous lui
 */
void initialisation_seuils();

/**
 *
 * {gauche, centre, droite} => \n
 * (ligne, ligne, plancher) = 0b110 \n
 * (plancher, plancher, ligne) = 0b001

 * @return Un nombre binaire du résultat de chaque capteur. \n
 * De 0b000 à 0b111 (=> (int) de 0 à 7)
 */
uint8_t SUIVEUR_Read() ;

#endif //AUTOBUS_SUIVEUR_LIGNE_H