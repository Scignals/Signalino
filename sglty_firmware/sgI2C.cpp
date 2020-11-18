#include "sgI2C.h"



void i2c::get_twobytes( int16_t &mivar){
  mivar = Wire.read() << 8;
  mivar |= Wire.read() ; // reading registers: 0x43 (GYRO_XOUT_H) and 0x44 (GYRO_XOUT_L)
  
}

void i2c::prepare_registro(int dispositivo, int registro, int num_variables_request){
  Wire.beginTransmission(dispositivo);
  Wire.write(registro); 
  Wire.endTransmission(false); 
  Wire.requestFrom(dispositivo, num_variables_request*2, true); // request a total of num*2 registers
  while(!Wire.available()) {};
}
