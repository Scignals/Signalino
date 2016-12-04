#include "version.h"

const char * build_version = "0.3";
const char * build_fecha = "dic 4, 2016";



void mensaje_inicio(){
   WiredSerial.println("");
   WiredSerial.print(F("Signalino v "));
   WiredSerial.println(build_version);   
   WiredSerial.print(F("build "));
   WiredSerial.println(build_fecha);   
   WiredSerial.println(F("(c) ILSB Technologies"));
  
   // si hay 8 canales, es q esta vivo...
   WiredSerial.print(F("canales activos:"));
   WiredSerial.println(gMaxChan);
   WiredSerial.println(F("Comandos: (separados por punto y coma)"));
   WiredSerial.println(F("hlp  -- ayuda "));
   WiredSerial.println(F("rec  -- chorro datos on/off")); 
   WiredSerial.println(F("sim  -- señal simulada on/off"));    
   WiredSerial.println(F("simN -- N1, señal normal. N2, test. N3, simulada")); 
   WiredSerial.println(F("ganN -- ganancia del ads1299 N=1..8 --> 1,2,4,4,6,8,12,24x"));
   WiredSerial.println(F("frmN -- formatos de salida: N=0,va cambiando. Ahora hay 8"));
   WiredSerial.println(F("inpN -- parametros de adquisición: N1 "));
    
   
}

