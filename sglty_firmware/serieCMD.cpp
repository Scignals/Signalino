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
// This software has been inspired by the work of many people:
// Chris Rorden (original ADS1298 driver), see http://www.mccauslandcenter.sc.edu/crnl/open-source-eegecgemg 
// Adam Feuer (Author of HackEEGShield), see https://github.com/adamfeuer 
// JA Barios, ILSB technologies, Spain.
// And, of course, the Arduino team
//
// This file is part of project: SIGNALINO, a ADS1299-based bioamplifier
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
int gFormatoSerial; 


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
    switch(modo_hex) {
    case 0:
    	break;
    case 2:
    	port_serial.print(numero,HEX);
    	break;
    default:
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
     imprime_linea2( modo_hex, WiredSerial );
     if(gBluetooth) imprime_linea2( modo_hex, HC06 ); //ponia modo_hex
  }
}



//protocolo openEEG, no funciona aun bien
void imprime_openEEG_p2(int modo_openeeg_protocolo){
// modo 1; numeros. Modo 2: bytes . Modo 3; hexadecimal
   int ind;
   static int count = -1;

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

// protocolo openbci V3, el que usamod por defecto, modo 2;
void imprime_openBCI_V3(int modo_bci_protocolo){
// protocolo interesante, descrito en 
// https://github.com/OpenBCI/OpenBCI-V2hardware-DEPRECATED/wiki/Data-Format-for-OpenBCI-V3 
// el ultimo byte es 0xCX, y según X se reinterpretan los últimos 6 bytes (acelerometro, fecha, o user defined)
// modo 1; numeros. Modo 2: bytes . Modo 3; hexadecimal
  
   int ind;
   byte timestamp[3];

   //for debug
   // gBluetooth=true;
   // HC06.println("estoy vivo en imprimeserial BT!");

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
     
     //este ultimo es C0 para los acelerometros
     // en su logar podian ir por ejemplo un registro de  booleanos: luz, bluetooh si/no,
     // pero entonces hay q cambiar el C0 por CX

     txBuf[ind]=0xC0; 

   ind=0;
   if(modo_bci_protocolo==1){ 
    // protocolo open_bci V3, pero con mumeros ascii   
     for(int m=0;m<2;m++){
        WiredSerial.print(txBuf[ind]);
        if(gBluetooth)HC06.print(txBuf[ind]);
        WiredSerial.print(SEPARADOR_SERIAL );
        if(gBluetooth)HC06.print(SEPARADOR_SERIAL );
        ind++;
     }
     for(int m=0;m<10;m++){
        long vv = to_Int32(txBuf+ind);
        WiredSerial.print(vv );
        if(gBluetooth)HC06.print(vv );
        if(m<9){
           WiredSerial.print(SEPARADOR_SERIAL );
           if(gBluetooth)HC06.print(SEPARADOR_SERIAL );
        }
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
	if(WiredSerial.available()!=0){
		String comando;
		while( (comando = WiredSerial.readStringUntil(';')).length()>0){
			comando.toLowerCase();
			comando.trim();
			procesaComando(comando);
		}
	}
	if(HC06.available()!=0){
		String comando;
		while( (comando = HC06.readStringUntil(';')).length()>0){
			comando.toLowerCase();
			comando.trim();
			procesaComando(comando);
		}
	}

}


// Comandos de control:
// gan: ganancia del amplificador
// inp: input mode (CHART, EEG, EMG)
// sim: señal de salida del signalino (real 1x, onda cuadrada del ads, señal simulada por software, real 12x )
// frm: formato de salida de datos ()
// rec: rec0 apaga salida, rec1 la restaura
// hlp: mensaje de inicio y espera un enter por wiredsignal
// oka: mensaje de inicio y sigue corriendo
// blt: bluetooth 1:on/0:off



void procesaComando(String texto){
     String parametro;
     int p1=0;
     comentaSerial(texto);
     comentaSerial("-->ok");


     // 1 normal  2 test 3 simulada 4 max ganancia
     if(texto.startsWith("sim")){ 
         p1=extrae_parametro(texto);
         switch(p1){
            case 1:
              gSenal_obtenida=SENAL_REAL;
              ads9_misetup_ADS1299(MODE_SENAL_REAL_1x);
              break;
            case 2:
              gSenal_obtenida=SENAL_REAL;
              ads9_misetup_ADS1299(MODE_SENAL_TEST);
              break;
            case 3:
              gSenal_obtenida=TABLA_SENO;
              /* no activamos ads9_misetup porque... (vamos, que ni idea) */
              break;
           case 4:
              gSenal_obtenida=SENAL_REAL;
              ads9_misetup_ADS1299(MODE_SENAL_REAL_12x);
              break;
          }       
         sprintf(buffer_comentaserial,"Signal mode changed to %d",p1);
         comentaSerial(buffer_comentaserial);
      } else if(texto.startsWith("hlp")){
          mensaje_inicio();          
          while(WiredSerial.available()==0 && HC06.available()==0 );
          return;
     
      } else if(texto.startsWith("frm")){
         p1=extrae_parametro(texto);
         if(p1==0){
            if(++gFormatoSerial>maxComando)gFormatoSerial=minComando;
         } else if (p1<=maxComando){
            gFormatoSerial=p1;
         }    
         sprintf(buffer_comentaserial,"Data format changed to %d",p1);
         comentaSerial(buffer_comentaserial);
   
      } else if(texto.startsWith("rec")){
         p1=extrae_parametro(texto);
         if(p1==0){
            // rec0 calla la salida de numeros por puerto serie
            ultimo_modo=gFormatoSerial;
            gFormatoSerial=8;            
         } else {
            gFormatoSerial=ultimo_modo;
         }
         sprintf(buffer_comentaserial,"Data format restored to  %d",p1);
         comentaSerial(buffer_comentaserial);
         // return;

      } else if(texto.startsWith("gan")){ 
         p1=extrae_parametro(texto);
         gSenal_obtenida=SENAL_REAL;
         ads9_setGanancia(p1);
         sprintf(buffer_comentaserial,"Gain changed to %d",p1);
         comentaSerial(buffer_comentaserial);

      } else if(texto.startsWith("inp")){ 
            p1=extrae_parametro(texto);
             switch(p1){
                case 1:
                gSenal_obtenida=SENAL_REAL;
                ads9_misetup_ADS1299(MODE_SENAL_SRB2);
                comentaSerial("CHART mode (all channels to SRB2)");
                break;
                case 2:
                gSenal_obtenida=SENAL_REAL;
                ads9_misetup_ADS1299(MODE_SENAL_REAL_1x);
                comentaSerial("EMG mode (bipolar channels, 1x)");
                // bipolares 1x
                break;
                case 3:
                gSenal_obtenida=SENAL_REAL;
                ads9_misetup_ADS1299(MODE_SENAL_REAL_12x);
                comentaSerial("EEG mode (bipolar channels, 1x)");
                //bipolares 12x
                break;
             }
        } else if(texto.startsWith("blt")){ 
               p1=extrae_parametro(texto);
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
      }       
               
      return;

}

int extrae_parametro( String texto){
         String parametro=texto.substring(3,4);
         int p1=parametro.toInt();
         return(p1);
}



void procesaComando_estilo_openBCI(String texto){
      // no implementado, tal vez algun dia...
      return;

}
