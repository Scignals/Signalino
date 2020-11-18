#pragma once
#include <Wire.h>
#include "sgI2C.h"

#define I2C_ACC 0x68

#define ACC_SENSIB_G 0x1C // los g   24-16g; 16-8g; 8-4g; 0-2g;
#define ACC_SENSIB_GRADOS 0x1B // los grados   24-250º/s; 16-500º/s; 8-1000º/s; 0-2000º/s;
#define ACC_SENSIB_algoquenosequees 0x6B 


// scl 19 sda 18
class acelerometro : i2c {
public:
    void iniciar() ;
    void leer();

public:
    int16_t  AcX; 
    int16_t  AcY; 
    int16_t  AcZ;
    int16_t  temperature;
    int16_t  GyX; 
    int16_t  GyY;
    int16_t  GyZ;

};



