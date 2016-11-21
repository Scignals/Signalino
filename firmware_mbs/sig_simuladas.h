/* sig_simuladas.h */
#ifndef SIG_SIMULADAS_H
#define SIG_SIMULADAS_H

#include "Arduino.h"

extern float samples_seno[];
extern float phaseIncrement;


#define TABLE_SIZE 48*8*4
#define AMPLITUD_SENAL 500
#define TWO_PI (3.14159 * 2)

#define TABLA_SENO 3 
#define TABLA_LINEAL 4
#define TABLA_RAMPA 5


void crea_tabla_seno(void);

#endif 



