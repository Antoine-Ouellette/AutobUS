
#include "detecteur_IR.h"
#include <Arduino.h>
#include "LibRobus.h"
#include "variables_globales.h" // Inclure les variables globales partagées entre tous les fichiers.

float lireDistance_quille()
{
    uint16_t bit = ROBUS_ReadIR(0);
    float volt = (bit * 5.0) / 1023.0;
    float distance = 29.988 * pow(volt, -1.173);
    return distance;
}


// FONCTION distance roue:
float lireDistance_roue()
{
    ENCODER_Reset(0);
    float distance = ENCODER_Read(0) * (PI * 7.62) / 3200; // conversion des pulses en cm

    float degrer = distance / cmToPulse; // conversion des cm en degr
    return degrer;
}