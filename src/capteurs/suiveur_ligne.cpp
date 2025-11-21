/**
 * @file Fonction en lien avec le suiveur de ligne
 * Inclue la lecture du capteur et la conversion vers une valeur binaire
 */

#include "suiveur_ligne.h"
#include <Arduino.h>

constexpr int nbPins = 6;
constexpr int nbPinsPerSensor = 3;
int pins[nbPins] = {A10, A9, A8, A13, A12, A11}; //Pins du suiveur gauche {gauche, centre, droit} et du suiveur droit{gauche, centre, droit}

//TODO : Modifier les valeurs une fois la maquette refaite
//seuils pour sensor gauche (0)
int seuil_ligne_0 = 600;
int seuil_externGauche_0 = 750;
int seuil_externDroit_0 = 700;
//seuils pour sensor droit (1)
int seuil_externGauche_1 = 700; 
int seuil_externDroit_1 = 750; 
int seuil_ligne_1 = 750;

//Choix du détecteur IR sur un des suiveurs de ligne
typedef struct seuilSuiveur {
    float position [3];
} Seuil;

Seuil seuilGauche = {
    {seuil_ligne_0,seuil_externGauche_0, seuil_externDroit_0}
};

Seuil seuilDroit = {
    {seuil_ligne_1, seuil_externGauche_1, seuil_externDroit_1}
};


//Choix du suiveur à utiliser
typedef struct sensor {
    Seuil GaucheOuDroite[2];
}Sensor;

Sensor SuiveurLigne = {
    {seuilGauche, seuilDroit}
};

//initialisation des pins des suiveurs de ligne sur l'Arduino
void SUIVEUR_init() {
    for (int i = 0; i < nbPins; i++) {
        pinMode(pins[i], INPUT);
    }
}


//Lecture du suiveur
uint8_t SUIVEUR_Read(int ID) {
    uint8_t resultat = 0b000; //Résultat du sensor
    for (int i = 0; i < nbPinsPerSensor; i++) {
        const int lecture = analogRead(pins[i]);
        const bool isLine = (SuiveurLigne.GaucheOuDroite[ID].position[i]) <= lecture;
        resultat |= isLine << i;
    }
    return resultat;
}

/*bool detectUsingThreshold(float seuil, float incertitude, int raw) {
    return (raw > (seuil - incertitude) && raw < (seuil + incertitude));
}
*/