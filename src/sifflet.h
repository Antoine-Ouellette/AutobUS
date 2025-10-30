/**
Projet: AutobUS
Equipe: P-12
Auteurs: Aimeric Bouillon
Description: Fonction de détecion du sifflet
Date: 7 octobre 2025
*/
#ifndef SIFFLET_H
#define SIFFLET_H


extern float freq_moy; //Valeur d'intensité à 5khz moyenne
extern float noise_moy; //Valeur d'intensité ambiante

#define mean_size 5 //Nombre d'objet de la moyenne

extern float freq_moy_array[mean_size]; //Tableau de moyennage de 5khz
extern float noise_moy_array[mean_size]; //Tableau de moyennage ambiant

extern float freq_sum; //Somme pour le moyennage à 5khz
extern float noise_sum; //Somme pour le moyennage ambiant

extern int index; //Objet présent du tableau

extern unsigned long previousMillis; 
#define interval_mean 10 //Interval de moyennage
#define seuil 90 //Seuil de détection du sifflet


/**
 * Détecte si le sifflet est sonné
 */
bool detection_sifflet();

/**
 * Initialise les tableaux de moyenne à 0.
 */
void init_sifflet();

#endif

