/**
Projet: AutobUS
Equipe: P-12
Auteurs: Aimeric Bouillon
Description: Fonction de détecion du sifflet
Date: 7 octobre 2025
*/
#include "sifflet.h"
#include "Arduino.h"

unsigned long previousMillis = 0;
int index = 0;
float noise_sum = 0;
float freq_sum = 0;
float freq_moy;
float noise_moy; 
float freq_moy_array[mean_size]; 
float noise_moy_array[mean_size]; 

bool detection_sifflet() {
    if (millis() - previousMillis >= interval_mean) {
        previousMillis = millis();

        int freq = analogRead(A6);
        int noise = analogRead(A7);

        freq_sum -= freq_moy_array[index];
        noise_sum -= noise_moy_array[index];

        freq_moy_array[index] = freq;
        noise_moy_array[index] = noise;

        freq_sum += freq;
        noise_sum += noise;

        freq_moy = freq_sum / mean_size;
        noise_moy = noise_sum / mean_size;
    }

    index++;

    if (index >= mean_size) {
        index = 0;
    }

    if ((freq_moy - seuil) > noise_moy) {
        return true;
    } else {
        return false;
    }
}

void init_sifflet() {
    for (int i = 0; i < mean_size; i++) {
        freq_moy_array[i] = 0;
        noise_moy_array[i] = 0;
    }
}

