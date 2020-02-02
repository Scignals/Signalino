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
    Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H) [MPU-6000 and MPU-6050 Register Map and Descriptions Revision 4.2, p.40]
  Wire.endTransmission(false); // the parameter indicates that the Arduino will send a restart. As a result, the connection is kept active.
  
  Wire.requestFrom(I2C_ACC, 7*2, true); // request a total of 7*2=14 registers
  
  // "Wire.read()<<8 | Wire.read();" means two registers are read and stored in the same variable
  AcX = Wire.read();  AcX = (AcX <<8) + Wire.read(); // reading registers: 0x3B (ACCEL_XOUT_H) and 0x3C (ACCEL_XOUT_L)
  AcY = Wire.read();  AcY = (AcY <<8) + Wire.read();// reading registers: 0x3D (ACCEL_YOUT_H) and 0x3E (ACCEL_YOUT_L)
  AcZ = Wire.read();  AcZ = (AcZ <<8) + Wire.read(); // reading registers: 0x3F (ACCEL_ZOUT_H) and 0x40 (ACCEL_ZOUT_L)
  temperature = Wire.read();  temperature = (temperature <<8) + Wire.read();// reading registers: 0x41 (TEMP_OUT_H) and 0x42 (TEMP_OUT_L)
  GyX = Wire.read();  GyX = (GyX <<8) + Wire.read(); // reading registers: 0x43 (GYRO_XOUT_H) and 0x44 (GYRO_XOUT_L)
  GyY = Wire.read();  GyY = (GyY <<8) + Wire.read(); // reading registers: 0x45 (GYRO_YOUT_H) and 0x46 (GYRO_YOUT_L)
  GyZ = Wire.read();  GyZ = (GyZ <<8) + Wire.read(); // reading registers: 0x47 (GYRO_ZOUT_H) and 0x48 (GYRO_ZOUT_L)
  
}


/*

void acelerometro::leer_marco() {
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

*/