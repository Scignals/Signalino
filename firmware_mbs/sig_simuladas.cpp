#include "sig_simuladas.h"


float samples_seno[TABLE_SIZE];
float phaseIncrement = TWO_PI/((float)TABLE_SIZE);

void crea_tabla_seno(void){
  float currentPhase = 0.0;
  int i;
  for (i = 0; i < TABLE_SIZE; i ++){
      samples_seno[i] = AMPLITUD_SENAL*sin(currentPhase)+AMPLITUD_SENAL;
      currentPhase += phaseIncrement;
      }
}      


