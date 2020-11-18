#pragma once
#include <Wire.h>
#include "sgI2C.h"

#define I2C_LUX 0x44
// opt3001

class luxometro : i2c {
public:
    void iniciar() ;
    luxometro(){luz=0;luz_calibrada=0;};
    byte get_luz(){return luz;};
    float get_luz_calibrada(){leer();return luz_calibrada;};
    float get_ultima_luz_calibrada(){return luz_calibrada;};
    float convert(uint16_t raw_data);

private:
    int16_t luz; 
    float luz_calibrada;   
    void leer();
};


