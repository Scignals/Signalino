#ifndef SIGNALINO_H
#define SIGNALINO_H

enum class formatos_eeg {
    HEXAD=1,DECIM,DRIVER,DRIVER_HEX
    };

#include "Arduino.h"
#include "serialPC.h"




enum class Footer {
    EEG = 0xC0,
    IMU = 0xC1
};

namespace TRAMA {
    const unsigned char EEG_header = 0xA0;
    const unsigned char EEG_footer = 0xC0;
    const int longitud = 33;
}




// sacado de vteensy
//?? cambiar esto a pin_... como abajo
#define pin_reset1 2
#define pin_pwdn 3
#define pin_drdy 4
#define pin_blkey 5
#define pin_start1 6
#define pin_tx 7
#define pin_rx 8
#define pin_key 9 /// Teensy and Bluetooth key pin for enable or disable comunication bluetooth
#define pin_cs 10   //10

#define pin_blue 21
#define pin_green 22
#define pin_red 23





#define cs_low      digitalWrite(pin_cs, LOW)
#define cs_high     digitalWrite(pin_cs, HIGH)
#define start_off   digitalWrite(pin_start1, LOW)
#define start_on    digitalWrite(pin_start1, HIGH)
#define reset_off   digitalWrite(pin_reset1, LOW)
#define reset_on    digitalWrite(pin_reset1, HIGH)
#define pwdn_off    digitalWrite(pin_pwdn, LOW)
#define pwdn_on     digitalWrite(pin_pwdn, HIGH)

#define bluetooth_on    digitalWrite(pin_blkey, HIGH);  // write low to enable bluetooth
#define bluetooth_off   digitalWrite(pin_blkey, LOW);  // write high to disable bluetooth

#define ledred_on       digitalWrite(pin_red, LOW);  
#define ledred_off      digitalWrite(pin_red, HIGH);  
#define ledgreen_on     digitalWrite(pin_green, LOW);  
#define ledgreen_off    digitalWrite(pin_green, HIGH);  
#define ledblue_on      digitalWrite(pin_blue, LOW);  
#define ledblue_off     digitalWrite(pin_blue, HIGH);  




class Signalino {
    unsigned char ultimaLecturaEEG[80];
    String buffer_linea;
    
    const int num_canales=8;
    const int num_bytes=25;
  
public:
    Footer footer;
    formatos_eeg formatoSerial;
    bool usando_driver=true;

public:
    void setFormatoSerial(formatos_eeg p);
    void setUsandoDriver(bool p){usando_driver=p;};
    bool getUsandoDriver(){return usando_driver;};
    void iniciar();
    void apuntarLecturaEEG(volatile unsigned char *bytes);
    unsigned char *getDireccionEEG(){return &ultimaLecturaEEG[0];};
    String *getLinea();
    void leeComandoSerial();

  
private:
    void imprimirLecturaEEG( unsigned char *numero, formatos_eeg formato);
    long toInt32(  volatile unsigned char* byteArray);    

    

};





#endif

