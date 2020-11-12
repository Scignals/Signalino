#pragma once
 // include the SD library:
#include <SD.h>
#include <SPI.h>

class sdtarjeta {

Sd2Card card;
SdVolume volume;
SdFile root;
bool operativa;


public: 
    File archivo;

const int chipSelect = BUILTIN_SDCARD;

public:
    int info() ;
    bool abre_archivo(const char *) ;
    bool cierra_archivo() ;
    
    void leer();

    sdtarjeta():operativa(false){};//iniciar vars};
    //byte get_luz(){return luz;};
    //float get_luz_calibrada(){leer();return luz_calibrada;};
    
public:

};

