
#ifndef DETECTEUR_IR_H
#define DETECTEUR_IR_H

/**
 * Lit la distance en cm du capteur infrarouge du ROBUS
 * @param id Numéro du capteur aàlire
 * @return La distance en cm lu par le capteur.
 */
float IR_ReadDistanceCm(int id);

#endif //DETECTEUR_IR_H