#include "luxometro.h"
#include <math.h>

#define I2C_LUX 0x44


void luxometro::iniciar() {
  Wire.begin();
  Wire.beginTransmission(I2C_LUX);
    Wire.write(0x01);
    Wire.write(0xc6);
    Wire.write(0x01);
  Wire.endTransmission();
  delay(10);
  luz=0;luz_calibrada=0;
}

void luxometro::leer() {
  prepare_registro(I2C_LUX,0x00,2);
  get_twobytes(luz);
  luz_calibrada=convert(luz);

/*    Wire.beginTransmission(I2C_LUX);
      Wire.write(0x00);
    Wire.endTransmission(false);
    Wire.requestFrom(I2C_LUX, 2, true);
*/    
/*    byte a=Wire.read();
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
*/ 

}


/*---------------------------------------------------------------------------*/
/**
 * \brief Convert raw data to a value in lux
 * \param raw_data data Pointer to a buffer with a raw sensor reading
 * \return Converted value (lux)
 */
float luxometro::convert(uint16_t raw_data)
{
  uint16_t e, m;

  m = raw_data & 0x0FFF;
  e = (raw_data & 0xF000) >> 12;

  return m * (0.01 * exp2(e));
}
