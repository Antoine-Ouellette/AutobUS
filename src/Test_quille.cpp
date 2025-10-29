// #include "LibRobus.h"
// #include <Arduino.h>
// #include "variables_globales.h" // Inclure les variables globales partagées entre tous les fichiers.
// #include "moteur.h"
// //


// // FONCTION distance quille:
// float lireDistance_quille(){
// 	uint16_t bit=ROBUS_ReadIR(0);
// 	float volt= (bit*5.0)/1023.0;
//     float distance = 29.988*pow(volt,-1.173);
// 	return distance;}
// // FONCTION distance roue:
// float lireDistance_roue(){
//     ENCODER_Reset(0);
//     float distance = ENCODER_Read(0) * (PI * 7.62) / 3200; // conversion des pulses en cm
    
//     float degrer = distance/cmToPulse; // conversion des cm en degr
//     return degrer;

// }

// //  float degrer;
// //  float distance;
// //  enum Etat {
// //    etat_tours,
// //    etat_recherche_quille,
// //    etat_avanceQuille,
// //    etat_reculer,
// //    etat_revenir
// //  };
//  /*switch (etat) {
  
//      case etat_tours :
//      tourner(LEFT, 360,0.6);
//      degrer= lireDistance_roue();
//      etat= etat_recherche_quille;
//      break;
//      case etat_recherche_quille:
//      distance = lireDistance_quille();
//      if (distance > 25){
//        arreter();
//          etat = etat_avanceQuille;
//      }
//      break;
//      case etat_avanceQuille :
//      setGoal(0.5, avancer, distance);
//      etat=etat_reculer;
//      break;
  
//      case etat_reculer :
//      setGoal(0.5,reculer, distance);
//      etat = etat_revenir;
//      break;
//      case etat_revenir :
//      tourner(RIGHT,degrer,0.6);
//      break;
//  }
// */




