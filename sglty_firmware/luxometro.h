#pragma once
#include <Wire.h>
#include "util.h"

#define OPT 0x44

class luxometro {
public:
    void iniciar() ;
    luxometro(){luz=0;luz_calibrada=0;};
    byte get_luz(){return luz;};
    float get_luz_calibrada(){leer();return luz_calibrada;};
    float get_ultima_luz_calibrada(){return luz_calibrada;};
    
private:
    byte luz; 
    float luz_calibrada;   
    void leer();
};

