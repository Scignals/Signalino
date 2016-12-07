///////////////////////////////////////////////
// SIGNALINO: a bioamplifier
// firmware for controlling a ADS1299-based shield for Arduino Due
//
//
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
//  This file is part of project: SIGNALINO, a ADS1299-based bioamplifier.
//
/*
* Author: JA Barios, ILSB technologies, Spain., agosto 2016 - enero 2017
* Modified:
* Company: ILSB technologies, Spain. 
* ---------------------------------------------
* Requires ControP5 library
*/


#include <stdlib.h>     /* strtoul */
#include <SPI.h>  // include the SPI library:
#include "ads1298.h"
#include "adsCMD.h"
#include "util.h"

#include "firmware_mbs.h"
#include "sig_simuladas.h"
#include "version.h"


long contador_muestras=0;


#define HC06 Serial3  
#define WiredSerial Serial
#define INTERVALO_LEESERIAL 256*16

// variables modificaables  durante debugging
// protocolo openEEG firmware P2, util para openvibe
// solo registra 6 canales

int     gSenal_obtenida=TABLA_SENO;
boolean gtestSignal=false;
boolean gtestHEX=false;
boolean gtestCONTINUO=true;
boolean gserialVerbose=true;
boolean gBluetooth=false;


// 1-hex 2-numeros 3-openeeg-hex 4-openeeg-bytes
// 5-openbci-numeros 6-openbci-bytes 7-openbci-hex 8-no imprime nada      
// define el modo de salida por defecto, 
// está en 2 porque lo lee el monitor del IDE de arduino
int modo_salida=2;



int minComando=1;
int maxComando=8;
int ultimo_modo=8;
unsigned long tick=0;



// globales
int gMaxChan = 0; //maximum number of channels supported by ads1299 = 8
int gIDval = 0; //Device ID : lower 5 bits of  ID Control Register 
int gNumActiveChan = 0;
boolean gActiveChan [9]; // reports whether channels 1..9 are active
boolean isRDATAC = false;


char *gLetra; // buffer usado en to_hex, inicializado en setup()
int numSerialBytes=0;
unsigned char serialBytes[80];
char buffer_comentaserial[50];


unsigned char txBuf[33];  //17 en openeeg   32 enopenbci
unsigned long indice_paquete=0;





void inicia_serial_pc(){
  gLetra=new char[80];  
  WiredSerial.begin(SERIAL_SPEED); 
  while (WiredSerial.read() >= 0) {} ;
  delay(200);  // Catch Due reset problem
  HC06.begin(SERIAL_SPEED_BT); //use native port on Due
  while (HC06.read() >= 0) {} ;
  delay(200);  // Catch Due reset problem
}


void imprime_linea( boolean modo){
   for (int i = 1; i < numSerialBytes; i+=3)
   {
    long numero = to_Int32(serialBytes+i);
     if(modo) {
                  WiredSerial.print(numero,HEX);
                  if(gBluetooth)HC06.print(numero,HEX);
     } else {
                  WiredSerial.print(numero);
                  if(gBluetooth)HC06.print(numero);
                  if(i<numSerialBytes-3){
                      WiredSerial.print(SEPARADOR_SERIAL );
                      if(gBluetooth)HC06.print(SEPARADOR_SERIAL );
                  }
                  
            }      
    }
//añadimos un ; al final 
    WiredSerial.println(FINLINEA);
    if(gBluetooth)HC06.println(FINLINEA);
}

void no_imprime_nada( boolean modo){
   return;
}


void imprime_openEEG_p2(boolean modo){
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
   if(modo){    
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
     WiredSerial.write(txBuf,17);
     }
}

void imprime_openBCI_V3(int modo_bci_protocolo){
// protocolo interesante, descrito en 
// https://github.com/OpenBCI/OpenBCI-V2hardware-DEPRECATED/wiki/Data-Format-for-OpenBCI-V3 
// el ultimo byte es 0xCX, y según X se reinterpretan los últimos 6 bytes (acelerometro, fecha, o user defined)
  
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

void comentaSerial(String texto)
{
   if(gserialVerbose){
     WiredSerial.print('#');
     WiredSerial.println(texto);
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
         comentaSerial("cambiado modo senyal");
        return;
     
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
        return;
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
                break;
                case 2:
                gSenal_obtenida=SENAL_REAL;
                ads9_misetup_ADS1299(MODE_SENAL_REAL_1x);
                comentaSerial("cambiado a modo emg");
                break;
                case 3:
                gSenal_obtenida=SENAL_REAL;
                ads9_misetup_ADS1299(MODE_SENAL_REAL_12x);
                comentaSerial("cambiado a modo eeg");
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
          return;
      }       
               
      return;

}

void leeSerial(){
    if(WiredSerial.available()==0)return;
    
     String comando;
     while( (comando = WiredSerial.readStringUntil(';')).length()>0){
         comando.toLowerCase();
         comando.trim();
         procesaComando(comando);
     }
}


void setup(){
  crea_tabla_seno();
  inicia_serial_pc();
  due_inicia_hw();

  if(!gtestCONTINUO){
      mensaje_inicio();
      while(1); //nos quedamos colgados para terminar
  }
  
  if( gtestSignal )
      ads9_misetup_ADS1299(MODE_SENAL_TEST);
  else
      ads9_misetup_ADS1299(MODE_SENAL_REAL_1x);
  
}


void loop()
{
  if(digitalRead(IPIN_DRDY) == LOW && gtestCONTINUO && isRDATAC ){
     ads9_lee_datos();
     if(1==1){
         switch(modo_salida){
          case 1: imprime_linea(true);break;
          case 2: imprime_linea(false);break;
          case 3: imprime_openEEG_p2(true);break;
          case 4: imprime_openEEG_p2(false);break;
          case 5: imprime_openBCI_V3(1);break;
          case 6: imprime_openBCI_V3(2);break;
          case 7: imprime_openBCI_V3(3);break;
          case 8: no_imprime_nada(true);break;
         }
     }    
  }
  if(tick%INTERVALO_LEESERIAL)leeSerial();
  tick++;
  
     
}




