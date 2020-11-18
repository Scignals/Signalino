#include "acelerometro.h"
#include "sgI2C.h"



void acelerometro::iniciar() {
  Wire.begin();
  Wire.beginTransmission(I2C_ACC);
    Wire.write(ACC_SENSIB_G);
    Wire.write(24);      // los g   24-16g; 16-8g; 8-4g; 0-2g;
  Wire.endTransmission(true);
  delay(10);

  Wire.beginTransmission(I2C_ACC);
    Wire.write(ACC_SENSIB_GRADOS);
    Wire.write(0);      // los grados   24-250º/s; 16-500º/s; 8-1000º/s; 0-2000º/s;
  Wire.endTransmission(true);
  delay(10);
  
  Wire.beginTransmission(I2C_ACC);
    Wire.write(ACC_SENSIB_algoquenosequees);
    Wire.write(0);
  Wire.endTransmission(true);
  delay(10);

//  comentaSerial("Configurado Acelerometro ACX ACY ACZ TEM GYX GYY GYZ");

}


void acelerometro::leer() {
  prepare_registro(I2C_ACC,0x3B,3);
  get_twobytes(AcX);
  get_twobytes(AcY);
  get_twobytes(AcZ);
  prepare_registro(I2C_ACC,0x41,1);
  get_twobytes(temperature);
  prepare_registro(I2C_ACC,0x43,3);
  get_twobytes(GyX);
  get_twobytes(GyY);
  get_twobytes(GyZ);
}


