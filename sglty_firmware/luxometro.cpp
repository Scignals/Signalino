#include "luxometro.h"



void luxometro::iniciar() {
  Wire.begin();
  Wire.beginTransmission(OPT);
    Wire.write(0x01);
    Wire.write(0xc6);
    Wire.write(0x01);
  Wire.endTransmission();
  delay(2);
  luz=0;luz_calibrada=0;
}

void luxometro::leer() {
    Wire.beginTransmission(OPT);
      Wire.write(0x00);
    Wire.endTransmission(false);
    Wire.requestFrom(OPT, 2, true);
    
    byte a=Wire.read();
    byte b=Wire.read(); 
    luz=a>>4;
    int l=(a & 0x0f);
    l=(l<<8)+b;
    float m=0.01;
    for (char aux=1; aux<=luz;aux++)
    {
        m=m*2;
    }
    luz_calibrada=m*l;
      
}

