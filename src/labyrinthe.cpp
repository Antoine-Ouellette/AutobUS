/**
Projet: AutobUS
Equipe: P-12
Auteurs: Adam Turcotte
Description: Fonction d'action et de contrôl dans le labyrinthe
Date: 7 octobre 2025
*/
#include <LibRobus.h>

#include "constantes.h"
#include "variablesGlobales.h"
#include "moteur.h"
#include "labyrinthe.h"

constexpr int longueurX = 3;
constexpr int longueurY = 10;
int posX = 1;
int posY = longueurY - 1;

/*État d'une case
 * {visité, nord, est, sud, ouest}
 */
uint8_t maze[longueurY][longueurX] = {
    {0b00101, 0b00101, 0b00101},
    {0b00001, 0b00010, 0b00100},
    {0b00101, 0b01111, 0b00101},
    {0b00001, 0b01010, 0b00100},
    {0b00101, 0b01111, 0b00101},
    {0b00001, 0b01010, 0b00100},
    {0b00101, 0b01111, 0b00101},
    {0b00001, 0b01010, 0b00100},
    {0b00101, 0b01111, 0b00101},
    {0b00011, 0b11010, 0b00110}
};
uint8_t orientation = NORD;
uint8_t currentCell = maze[posY][posX];

void endMouvement() {
    resetEncoders();
    resetGoals();
    resetMultiplicateur();

    switch (currentEtat) {
        case 1: //deplacement
            for (int i = 0; i < nbCaseAvance; i++) {
                maze[posY][posX] |= VISITE;
                switch (orientation) {
                    case NORD:
                        posY--;
                        break;
                    case EST:
                        posX++;
                        break;
                    case SUD:
                        posY++;
                        break;
                    case OUEST:
                        posX--;
                        break;
                    default: break;
                }
            }
            break;
        case 2: //tourne droite
            switch (orientation) {
                case NORD:
                case EST:
                case SUD:
                    orientation >>= 1;
                    break;
                case OUEST:
                    orientation = NORD;
                    break;
                default: break;
            }
            break;
        case 3: //tourne gauche
            switch (orientation) {
                case NORD:
                    orientation = OUEST;
                    break;
                case EST:
                case SUD:
                case OUEST:
                    orientation <<= 1;
                    break;
                default: break;
            }
            break;
        case 4: //180°
            switch (orientation) {
                case NORD:
                case EST:
                    orientation >>= 2;
                    break;
                case SUD:
                    break;
                case OUEST:
                    orientation <<= 2;
                    break;
                default: break;
            }
            break;
        default: break;
    }
}

void ajouteMurEnFace() {
    switch (orientation) {
        case NORD:
            if (posY < 1) return;
            maze[posY - 1][posX] |= SUD;
            break;
        case EST:
            if (posX >= longueurX - 1) return;
            maze[posY][posX + 1] |= OUEST;
            break;
        case SUD:
            if (posY >= longueurY - 1) return;
            maze[posY + 1][posX] |= NORD;
            break;
        case OUEST:
            if (posX < 1) return;
            maze[posY][posX - 1] |= EST;
            break;
        default: break;
    }
}

void updateWall() {
    if (!digitalRead(pinVert) || !digitalRead(pinRouge)) {
        // si devant un mur
        maze[posY][posX] |= orientation; //Ajoute le mur dans la matrice du labyrinthe
        ajouteMurEnFace(); //Ajoute le mur équivalent dans la case en face
    }
}

void findNextMove() {
    if (posY == -1) {
        //Le robot est rendu à la fin.
        if (posX == 0) {
            allerEst();
        } else if (posX == 2) {
            allerOuest();
        } else {
            allerSud();
        }
        return;
    }
    if (posY == 0) {
        //Le robot est rendu à la fin. Dépasser la dernière case.
        currentEtat = AVANCER;
        calculateGoals();
        return;
    }

    // //Si on regarde une case vide y aller, vu que y'a jamais de cus de sac
    // if (!(maze[posY][posX] & orientation) && !(maze[posY][posX] & VISITE)) {
    //     currentEtat = AVANCER;
    //     nbCaseAvance = 1;
    //     calculateGoals();
    //     return;
    // }

    // S'il n'y a pas de mur au nord.
    if (!(maze[posY][posX] & NORD)) {
        allerNord();
        return;
    }

    // S'il n'y a pas de mur à l'est et que la case à l'est n'est pas visité.
    if (!(maze[posY][posX] & EST) && !(maze[posY][posX + 1] & VISITE)) {
        allerEst();
        return;
    }

    //**************
    //Pas nécessaire pour le parcours
    //*************
    // // Si la case à droite ne monte pas et la case du milieu est déjà visitée
    // // avance de deux case à gauche.
    // if (!(maze[posY][posX] & OUEST) && maze[posY][posX - 1] & VISITE) {
    //     if (orientation == NORD) {
    //         currentEtat = TOURNER_GAUCHE;
    //     } else if (orientation == EST) {
    //         currentEtat = TOURNER_180;
    //     } else {
    //         currentEtat = AVANCER;
    //     }
    //     nbCaseAvance = 2; //On avance de deux case parce qu'on connait déjà la case du centre
    //     calculateGoals();
    //     return;
    // }

    // S'il n'y a pas de mur à l'ouest.
    if (!(maze[posY][posX] & OUEST)) {
        allerOuest();
        return;
    }
}

void allerNord() {
    if (orientation == OUEST) {
        currentEtat = TOURNER_DROITE;
    } else if (orientation == EST) {
        currentEtat = TOURNER_GAUCHE;
    } else {
        currentEtat = AVANCER;
    }
    if (posY > 1 && posY % 2 == 1) {
        //Si la case est impaire, on va être vis-à-vis un tape, donc avancer une autre case
        nbCaseAvance = 2;
    } else {
        nbCaseAvance = 1;
    }

    calculateGoals();
    return;
}

void allerEst() {
    if (orientation == NORD) {
        currentEtat = TOURNER_DROITE;
    } else if (orientation == OUEST) {
        currentEtat = TOURNER_180;
    } else {
        currentEtat = AVANCER;
    }

    if (posY > 1 && (posY < longueurY - 1 || posX != 1)) nbCaseAvance = 2;
    else nbCaseAvance = 1;

    calculateGoals();
    return;
}

void allerOuest() {
    if (orientation == NORD) {
        currentEtat = TOURNER_GAUCHE;
    } else if (orientation == EST) {
        currentEtat = TOURNER_180;
    } else {
        currentEtat = AVANCER;
    }

    if (posY > 1 && (posY < longueurY - 1 || posX != 1)) nbCaseAvance = 2;
    else nbCaseAvance = 1;

    calculateGoals();
    return;
}

void allerSud() {
    if (orientation == NORD) {
        currentEtat = TOURNER_180;
    } else if (orientation == EST) {
        currentEtat = TOURNER_DROITE;
    } else if (orientation == OUEST) {
        currentEtat = TOURNER_GAUCHE;
    } else {
        currentEtat = AVANCER;
    }
    nbCaseAvance = 10;
    calculateGoals();
}
