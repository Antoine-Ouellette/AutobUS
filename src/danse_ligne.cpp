#include <LibRobus.h>          // Essentielle pour utiliser RobUS.
#include <Servo.h>
#include "variables_globales.h"


#include <Servo.h>

// Déclaration des servos
Servo brasG;
Servo brasD;

// Broches selon tes indications
const int GAUCHE = 4;   // Servo bras gauche
const int DROITE = 7;   // Servo bras droit
const int BLEUE = 10;
const int ROOUGE = 11;
const int VERTE = 12;
const int JAUNE = 13;

// Positions des bras
const int POS_HAUT = 0;
const int POS_BAS = 90;
const int POS_DEVANT = 45;

// Délai entre les étapes

void setup() {
  // Attache des servos
  brasG.attach(GAUCHE);
  brasD.attach(DROITE);

  // Configuration des DELs
  pinMode(ROOUGE, OUTPUT);
  pinMode(JAUNE, OUTPUT);
  pinMode(VERTE, OUTPUT);
  pinMode(BLEUE, OUTPUT);

  // Initialisation : bras bas et DELs éteintes
  brasG.write(POS_BAS);
  brasD.write(POS_BAS);
  allLedsOff();
  delay(tempo);
}

void loop() {
  // --- Séquence des bras ---
  brasG.write(POS_HAUT);
  delay(tempo);

  brasD.write(POS_HAUT);
  delay(tempo);

  brasG.write(POS_BAS);
  delay(tempo);

  brasD.write(POS_BAS);
  delay(tempo);

  // --- Séquence des DELs ---
  allLedsOff();
  digitalWrite(ROOUGE, HIGH);
  delay(tempo);

  allLedsOff();
  digitalWrite(JAUNE, HIGH);
  delay(tempo);

  allLedsOff();
  digitalWrite(VERTE, HIGH);
  delay(tempo);

  allLedsOff();
  digitalWrite(BLEUE, HIGH);
  delay(tempo);

  // --- Combinaisons DEL + bras ---
  // Rouge + bras devant
  allLedsOff();
  digitalWrite(ROOUGE, HIGH);
  brasG.write(POS_DEVANT);
  brasD.write(POS_DEVANT);
  delay(tempo);

  // Jaune + bras en haut
  allLedsOff();
  digitalWrite(JAUNE, HIGH);
  brasG.write(POS_HAUT);
  brasD.write(POS_HAUT);
  delay(tempo);

  // Verte + bras devant
  allLedsOff();
  digitalWrite(VERTE, HIGH);
  brasG.write(POS_DEVANT);
  brasD.write(POS_DEVANT);
  delay(tempo);

  // Bleue + bras en bas
  allLedsOff();
  digitalWrite(BLEUE, HIGH);
  brasG.write(POS_BAS);
  brasD.write(POS_BAS);
  delay(tempo);

  // Fin de séquence — pause avant de recommencer
  allLedsOff();
  delay(2000);
}

// Fonction pour éteindre toutes les DELs
void allLedsOff() {
  digitalWrite(ROOUGE, LOW);
  digitalWrite(JAUNE, LOW);
  digitalWrite(VERTE, LOW);
  digitalWrite(BLEUE, LOW);
}