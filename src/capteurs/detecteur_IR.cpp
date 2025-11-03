
#include "detecteur_IR.h"
#include <Arduino.h>
#include "LibRobus.h"

float IR_ReadDistanceCm(const int id)
{
    uint16_t bit = ROBUS_ReadIR(id);
    float volt = (bit * 5.0) / 1023.0;
    float distance = 29.988 * pow(volt, -1.173);
    return distance;
}
