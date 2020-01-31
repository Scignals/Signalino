#include <Wire.h>
#define I2C_ACC 0x68

// scl 19 sda 18
class acelerometro {
public:
    void iniciar() ;
    acelerometro(){//iniciar vars};
    //byte get_luz(){return luz;};
    //float get_luz_calibrada(){leer();return luz_calibrada;};
    

    
private:
//    byte luz; 
//    float luz_calibrada;  
int16_t AcX; 
int16_t AcY; 
int16_t AcZ;
int16_t GyX; 
int16_t GyY;
int16_t GyZ;

    void leer();
};



