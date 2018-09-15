// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//  
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, 
// MA  02110-1301, USA.
// 
//
//  Copyright © 2016 JA Barios, ILSB technologies, Spain.
//  This file is part of project: SIGNALINO, a ADS1299-based bioamplifier
//
#include "serieCMD.h"

#define MAX_LEN_PACKET 33        //32+1, openbci V3
unsigned char txBuf[MAX_LEN_PACKET];  //17 en openeeg   32 en openbci V3, 8 canales+3acelerometros
unsigned long indice_paquete=0;

// velocidad de los puertos, 0 si cerrados
int gWired_speed=0;
int gBT_speed=0;

char buffer_comentaserial[MAX_COMENTARIO_SERIAL];
char *gLetra; // buffer usado en to_hex, inicializado en inicia_serial()

int minComando=1;
int maxComando=8;
int ultimo_modo=8;


void inicia_serial_pc(){
  gLetra=new char[80];  
  WiredSerial.begin(SERIAL_SPEED); 
  while (WiredSerial.read() >= 0) {} ;
  delay(200);  // Catch Due reset problem
  gWired_speed=SERIAL_SPEED;
  
  HC06.begin(SERIAL_SPEED_BT); 
  while (HC06.read() >= 0) {} ;
  delay(200);  // Catch Due reset problem
  gBT_speed=SERIAL_SPEED_BT;
 
}

void comentaSerial(String texto)
{
   if(gserialVerbose){
     WiredSerial.print('#');
     WiredSerial.println(texto);
  }
}


/* PROTOCOLOS POR PUERTO SERIE   */
//--------------------------------
// cambio a Stream y referencia porque ahora funciona bien con los dos tipos de puertos
// leido en http://forum.arduino.cc/index.php?topic=155961.0

void imprime_linea2( int modo_hex, Stream &port_serial ){
   for (int i = 1; i < numSerialBytes; i+=3)
   {
    long numero = to_Int32(serialBytes+i);
     if(modo_hex==2) {
                  port_serial.print(numero,HEX);
     } else {
                  port_serial.print(numero);
                  if(i<numSerialBytes-3){
                      port_serial.print(SEPARADOR_SERIAL );
                  }
            }      
    }
    //añadimos un ; al final 
    port_serial.println(FINLINEA);    
}


// protocolo simple, solo numeros
void imprime_linea( int modo_hex){
  if(modo_hex){
//   imprime_linea2( modo_hex, &WiredSerial );
   imprime_linea2( modo_hex, Serial );
   if(gBluetooth) imprime_linea2( modo_hex, HC06 ); //ponia modo_hex
  }
}



//protocolo openEEG, no funciona aun bien
void imprime_openEEG_p2(int modo_openeeg_protocolo){
// modo 1; numeros. Modo 2: bytes . Modo 3; hexadecimal
   int ind;
   int indj=0;
   static int count = -1;
   unsigned long ul;
   dos_int dosnum;

   ind=0;
   txBuf[ind++]=0xA5;
   txBuf[ind++]=0x5A;
   txBuf[ind++]=2;
   txBuf[ind++]=indice_paquete++;
      
   for (int i = 1; i < numSerialBytes-6; i+=3)
   {
     long numero = to_Int32(serialBytes+i) ;
     numero >>= 13; 
     //LSB - HSB
     txBuf[ind++] = (byte) (numero & 0x000000FF)  ;     
     txBuf[ind++] = (byte) ((numero & 0x0000FF00) >> 8)  ;
   }
   byte switches = 0x07;
   count++; if (count >= 18) count=0;
   if (count >= 9) {
      switches = 0x0F;
   } 
   txBuf[ind]=switches; 
   ind=0;
   if(modo_openeeg_protocolo==1){    
     for(int m=0;m<4;m++){
        WiredSerial.print(txBuf[m]);
        if(gBluetooth)HC06.print(txBuf[m]);
        WiredSerial.print(SEPARADOR_SERIAL );
        if(gBluetooth)HC06.print(SEPARADOR_SERIAL );
        ind++;
     }
     for(int m=0;m<6;m++){
        int vv=((int)txBuf[ind++] << 8);
        vv += (int)txBuf[ind++] ;
        WiredSerial.print(vv );
        if(gBluetooth)HC06.print(vv );
        WiredSerial.print(SEPARADOR_SERIAL );
        if(gBluetooth)HC06.print(SEPARADOR_SERIAL );
     }
     WiredSerial.print(txBuf[ind]  );
     if(gBluetooth)HC06.print(txBuf[ind]  );
//añadimos un ; al final 
    WiredSerial.println(";");
    if(gBluetooth)HC06.println(";");

   } else {
        if(modo_openeeg_protocolo==2) {
               WiredSerial.write(txBuf,17);
               
   } else {
   // protocolo open_eeg , pero en hexadecimal, a implementar, ahora sale en dec
     for(int m=0;m<4;m++){
        WiredSerial.print(txBuf[m]);
        if(gBluetooth)HC06.print(txBuf[m]);
        WiredSerial.print(SEPARADOR_SERIAL );
        if(gBluetooth)HC06.print(SEPARADOR_SERIAL );
        ind++;
     }
     for(int m=0;m<6;m++){
        int vv=((int)txBuf[ind++] << 8);
        vv += (int)txBuf[ind++] ;
        WiredSerial.print(vv );
        if(gBluetooth)HC06.print(vv );
        WiredSerial.print(SEPARADOR_SERIAL );
        if(gBluetooth)HC06.print(SEPARADOR_SERIAL );
     }
     WiredSerial.print(txBuf[ind]  );
     if(gBluetooth)HC06.print(txBuf[ind]  );
   //añadimos un ; al final 
    WiredSerial.println(";");
    if(gBluetooth)HC06.println(";");

   }
 }
}

// protocolo openbci V3, el que usbamos antes por defecto, modo 2;
void imprime_openBCI_V3(int modo_bci_protocolo){
// protocolo interesante, descrito en 
// https://github.com/OpenBCI/OpenBCI-V2hardware-DEPRECATED/wiki/Data-Format-for-OpenBCI-V3 
// el ultimo byte es 0xCX, y según X se reinterpretan los últimos 6 bytes (acelerometro, fecha, o user defined)
// modo 1; numeros. Modo 2: bytes . Modo 3; hexadecimal
  
   int ind;
   int indj=0;
   static int count = -1;
   unsigned long ul;
   dos_int dosnum;
   byte timestamp[3];


   ind=2;
   txBuf[0]=0xA0;
   txBuf[1]=indice_paquete++;
   for (int i = 1; i < numSerialBytes; i+=3)
   {
     txBuf[ind++] = serialBytes[i];
     txBuf[ind++] = serialBytes[i+1];
     txBuf[ind++] = serialBytes[i+2];
   }
     //los 2 acelerometros, no los tenemos asi que lo dejamos a cero
     //pero metemos un timestamp en el primero
     to_3bytes(micros(),timestamp);
                
     txBuf[ind++] = timestamp[0];
     txBuf[ind++] = timestamp[1];
     txBuf[ind++] = timestamp[2];
     txBuf[ind++] = 0;
     txBuf[ind++] = 0;
     txBuf[ind++] = 0;
     
     //este ultimo es C0 para los acelerometros, pero podia ser otro
     txBuf[ind]=0xC0; 

   ind=0;
   if(modo_bci_protocolo==1){ 
    // protocolo open_bci V3, pero con mumeros ascii   
     for(int m=0;m<2;m++){
        WiredSerial.print(txBuf[ind]);
        WiredSerial.print(SEPARADOR_SERIAL );
        ind++;
     }
     for(int m=0;m<10;m++){
        long vv = to_Int32(txBuf+ind);
        WiredSerial.print(vv );
        if(m<9)WiredSerial.print(SEPARADOR_SERIAL );
        ind+=3;
     }

    //añadimos un ; al final 
    WiredSerial.println(";");
    if(gBluetooth)HC06.println(";");

  } else {
    if(modo_bci_protocolo==2){
    // protocolo open_bci V3, pero con bytes (es el que se usa en el visor_pc)
        WiredSerial.write(txBuf,33);
        if(gBluetooth)HC06.write(txBuf,33);
//        if(gBluetooth)HC06.println(" y probando...");
        
       
    } else {
    // protocolo open_bci V3, pero en hexadecimal
         char letras[5];
         for(int jj=0;jj<33;jj++){
          sprintf(letras, "%02X",txBuf[jj]);
          WiredSerial.print(letras);
          if(gBluetooth)HC06.print(letras);
         }
         WiredSerial.println("");
         if(gBluetooth)HC06.println("");

    }
  }
}



void leeSerial_signalino(){
    if(WiredSerial.available()==0)return;
    
     String comando;
     while( (comando = WiredSerial.readStringUntil(';')).length()>0){
         comando.toLowerCase();
         comando.trim();
         procesaComando(comando);
     }
}


void procesaComando(String texto){
     String parametro;
     comentaSerial(texto);
     comentaSerial("-->ok");

     // 1 normal  2 test 3 simulada 4 max ganancia
     if(texto.startsWith("sim")){ 
         parametro=texto.substring(3,4);
         int p1=parametro.toInt();
         switch(p1){
            case 1:
              gSenal_obtenida=TABLA_SENO;
              gtestSignal=false;
              break;
            case 3:
              gSenal_obtenida=SENAL_REAL;
              gtestSignal=false;
              ads9_misetup_ADS1299(MODE_SENAL_REAL_1x);
              break;
            case 2:
              gSenal_obtenida=SENAL_REAL;
              gtestSignal=true;
              ads9_misetup_ADS1299(MODE_SENAL_TEST);
              break;
           case 4:
              gSenal_obtenida=SENAL_REAL;
              gtestSignal=false;
              ads9_misetup_ADS1299(MODE_SENAL_REAL_12x);
              break;
          }       
         sprintf(buffer_comentaserial,"cambiado modo senyal a %d",p1);
         comentaSerial(buffer_comentaserial);
      } else if(texto.startsWith("hlp")){
          mensaje_inicio();
          while(WiredSerial.available()==0);
          return;
     
      } else if(texto.startsWith("frm")){
         parametro=texto.substring(3,4);
         int p1=parametro.toInt();
         if(p1==0){
            if(++modo_salida>maxComando)modo_salida=minComando;
         } else if (p1<=maxComando){
            modo_salida=p1;
         }    
         sprintf(buffer_comentaserial,"cambiado modo_salida a %d",p1);
         comentaSerial(buffer_comentaserial);
   
      } else if(texto.startsWith("rec")){
        // rec deberia apagar y encender el chorro, pero no parece hacer eso
         parametro=texto.substring(3,4);
         int p1=parametro.toInt();
         if(p1==0){
            ultimo_modo=modo_salida;
            modo_salida=8;            
         } else {
            modo_salida=ultimo_modo;
         }
         sprintf(buffer_comentaserial,"cambiado modo_salida a %d. Esto creo que no tiene sentido",p1);
         comentaSerial(buffer_comentaserial);
         // return;

      } else if(texto.startsWith("gan")){ 
          parametro=texto.substring(3,4);
          int p1=parametro.toInt();
          gSenal_obtenida=SENAL_REAL;
          gtestSignal=true;
          ads9_setGanancia(p1);
          sprintf(buffer_comentaserial,"cambiado ganancia a %d",p1);
          comentaSerial(buffer_comentaserial);

      } else if(texto.startsWith("inp")){ 
             parametro=texto.substring(3,4);
             int p1=parametro.toInt();
             switch(p1){
                case 1:
                gSenal_obtenida=SENAL_REAL;
                ads9_misetup_ADS1299(MODE_SENAL_SRB2);
                comentaSerial("cambiado a modo chart");
                // todos a SRB2
                break;
                case 2:
                gSenal_obtenida=SENAL_REAL;
                ads9_misetup_ADS1299(MODE_SENAL_REAL_1x);
                comentaSerial("cambiado a modo emg");
                // bipolares 1x
                break;
                case 3:
                gSenal_obtenida=SENAL_REAL;
                ads9_misetup_ADS1299(MODE_SENAL_REAL_12x);
                comentaSerial("cambiado a modo eeg");
                //bipolares 12x
                break;
             }
        } else if(texto.startsWith("blt")){ 
             parametro=texto.substring(3,4);
             int p1=parametro.toInt();
             switch(p1){
                case 0:
                gBluetooth=false;
                comentaSerial("BT off");
                break;
                case 1:
                gBluetooth=true;
                comentaSerial("BT on");
                break;
             }
      } else if(texto.startsWith("oka")){ 
          mensaje_inicio();
         // return;
      }       
               
      return;

}


