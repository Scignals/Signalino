#include "acelerometro.h"



void acelerometro::iniciar() {
  Wire.begin();
  Wire.beginTransmission(I2C_ACC);
    Wire.write(0x1C);
    Wire.write(24);      // los g   24-16g; 16-8g; 8-4g; 0-2g;
  Wire.endTransmission(true);
  delay(10);

  Wire.beginTransmission(I2C_ACC);
    Wire.write(0x1B);
    Wire.write(0);      // los g   24-250º/s; 16-500º/s; 8-1000º/s; 0-2000º/s;
  Wire.endTransmission(true);
  delay(10);
  
  Wire.beginTransmission(I2C_ACC);
    Wire.write(0x6B);
    Wire.write(0);
  Wire.endTransmission(true);
  delay(10);

  Serial.println("ACX ACY ACZ TEM GYX GYY GYZ");

}

void acelerometro::leer() {
  int i;

  Wire.beginTransmission(I2C_ACC);
    Wire.write(0x3B);
    Wire.endTransmission(false);
    Wire.requestFrom(I2C_ACC, 14, true);
    for (char j=0;j<7;j++)
    {
      dato[j]=Wire.read();
      dato[j]=(dato[j]<<8)+Wire.read();
     //dato[j]=(dato[j]<<16);
    }

    for (i=0;i<6; i++)
    {
      if ((dato[i]&0x8000)==0x8000)
      {
        dato[i]=(dato[i]|0xffff0000);
      }
      Serial.print(dato[i]);
      Serial.print(" ");
    }
    
    i=6;
    if ((dato[i]&0x8000)==0x8000)
      {
        dato[i]=(dato[i]|0xffff0000);
      }
      
    Serial.println(dato[i]);      
}

