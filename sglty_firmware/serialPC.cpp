#include "serialPC.h"

void SerialPC::iniciar(){

  WiredSerial.begin(SERIAL_SPEED);
  while (!WiredSerial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  while (WiredSerial.read() >= 0) {} ;
  delay(200);  // Catch Due reset problem
  
  Bluetooth.begin(SERIAL_SPEED_BT); 
  while (Bluetooth.read() >= 0) {} ;
  delay(200);  // Catch Due reset problem

  comentar("puerto serie iniciado"); 
};

void SerialPC::comentar(String texto){
   if(VerboseWired){
     WiredSerial.print('#');
     WiredSerial.println(texto);
  }
   if(VerboseBluetooth){
     Bluetooth.print('#');
     Bluetooth.println(texto);
  }
 
};

        

