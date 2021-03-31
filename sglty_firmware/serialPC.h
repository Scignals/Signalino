#ifndef SERIALPC_H
#define SERIALPC_H

#include "Arduino.h"


#define SERIAL_SPEED 115200  //57600 460800
#define SERIAL_SPEED_BT 115200
#define SEPARADOR_SERIAL ','
#define FINLINEA ";"


#define Bluetooth Serial3  
#define WiredSerial Serial


class SerialPC {

//    Stream &bluetooh;
//    Stream &wired;

    char buffer_serie[80];  
    bool VerboseWired;
    bool VerboseBluetooth;



  
public:
    void iniciar();
    void comentar(String texto);
    void setVerbose(bool wired,bool blue){VerboseWired=wired;VerboseBluetooth=blue;};

};



#endif
