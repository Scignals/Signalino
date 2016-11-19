// signalino, firmware v 02 rama master
// juan Barios, agosto 2016 - enero 2017
// No warranty: use it "as is"


#include "ads1298.h"
#include <stdlib.h>     /* strtoul */
#include "adsCMD.h"
#include "util.h"
#include <SPI.h>  // include the SPI library:

#include "firmware_mbs_01.h"
#include "version.h"
//#include "serial_parser.h"

long ultima_lectura[8];
long contador_muestras=0;

#define TABLE_SIZE 48
#define TWO_PI (3.14159 * 2)
#define AMPLITUD_SENAL 1500
float samples [TABLE_SIZE];
float phaseIncrement = TWO_PI/TABLE_SIZE;

void crea_seno(void){
  float currentPhase = 0.0;
  int i;
  for (i = 0; i < TABLE_SIZE; i ++){
      samples[i] = AMPLITUD_SENAL*sin(currentPhase)+AMPLITUD_SENAL;
      currentPhase += phaseIncrement;
      }
}      


      

void inicia_hw() {
  using namespace ADS1298;



  pinMode(IPIN_CS, OUTPUT);
  pinMode(PIN_START, OUTPUT);
  pinMode(IPIN_DRDY, INPUT);
  pinMode(kPIN_LED, OUTPUT);
  pinMode(kPIN_RESET, OUTPUT);
  pinMode(kPIN_CLKSEL, OUTPUT);

    //al empezar, reset
   delay(800); //desde Power up, esperar 1 segundo para mover nada
   digitalWrite(kPIN_RESET, LOW);
   delay(100);
   digitalWrite(kPIN_RESET, HIGH);
   delay(260); //deberia bastar, tiempo en datasheet es 240 ms

  digitalWrite(PIN_START, LOW);
  digitalWrite(IPIN_CS, HIGH);
  digitalWrite(kPIN_CLKSEL, HIGH); // el reloj sea el interno
    
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE1);

  adc_send_command(SDATAC); // dejamos el modo READ para emitir comandos
  delay(10);

  // Determine model number and number of channels available
  gIDval = adc_rreg(ID); //lower 5 bits of register 0 reveal chip type
  switch (gIDval & B00011111 ) { //least significant bits reports channels
          case  B10000: //16
            gMaxChan = 4; //ads1294
            break;
          case B10001: //17
            gMaxChan = 6; //ads1296
            break; 
          case B10010: //18
            gMaxChan = 8; //ads1298
            break;
          case B11110: //30
            gMaxChan = 8; //ads1299
            break;
          default: 
            gMaxChan = 0;
  }

  // aqui podia ir una secuencia de leds para ver cuanto vale gmaxchan

}

/* 
 *  MODE_SENAL_TEST .- ads saca senal test
 *  MODE_SENAL_REAL_12x -- ads saca senal real a 24x
 *  MODE_SENAL_REAL_1x -- ads saca senal real a 1x
*/

void ads_misetup_ADS1299(int opciones) {
   using namespace ADS1298;
   adc_send_command(SDATAC); // dejamos el modo READ para emitir comandos
   delay(10);

   switch(opciones){
     case MODE_SENAL_REAL_1x:
       adc_wreg(GPIO, char(0));
       adc_wreg(CONFIG1, LOW_POWR_250_SPS);
       adc_wreg(CONFIG2, INT_TEST_4HZ_2X);  // generate internal test signals
       adc_wreg(CONFIG3,char(PD_REFBUF | CONFIG3_const)); //PD_REFBUF used for test signal, activa la referencia interna
       delay(150);
       for (int i = 1; i <= gMaxChan; i++){
               adc_wreg(char(CHnSET + i), char(ELECTRODE_INPUT | GAIN_1X )); //report this channel with x12 gain
        } 
        break;       
     case MODE_SENAL_TEST:
       adc_wreg(GPIO, char(0));
       adc_wreg(CONFIG1, LOW_POWR_250_SPS);
       adc_wreg(CONFIG2, INT_TEST_4HZ_2X);  // generate internal test signals
       adc_wreg(CONFIG3,char(PD_REFBUF | CONFIG3_const)); //PD_REFBUF used for test signal, activa la referencia interna
       delay(150);
       for (int i = 1; i <= gMaxChan; i++){
            adc_wreg(char(CHnSET + i), char(TEST_SIGNAL | GAIN_1X ));
        } 
        break;
      case MODE_SENAL_REAL_12x:
       adc_wreg(GPIO, char(0));
       adc_wreg(CONFIG1, LOW_POWR_250_SPS);
       adc_wreg(CONFIG2, INT_TEST_4HZ_2X);  // generate internal test signals
       adc_wreg(CONFIG3,char(PD_REFBUF | CONFIG3_const)); //PD_REFBUF used for test signal, activa la referencia interna
       delay(150);
       for (int i = 1; i <= gMaxChan; i++){
               adc_wreg(char(CHnSET + i), char(ELECTRODE_INPUT | GAIN_12X )); //report this channel with x12 gain
        } 
        break;       
            
   }
  //start streaming data
    detectActiveChannels(); 
    isRDATAC = true;
    adc_send_command(RDATAC); 
    adc_send_command(START); 
}

void ads_setupGanancia(int valor) {
       using namespace ADS1298;
       adc_send_command(SDATAC); // dejamos el modo READ para emitir comandos
       delay(10);
       adc_wreg(GPIO, char(0));
       adc_wreg(CONFIG1, LOW_POWR_250_SPS);
       adc_wreg(CONFIG2, INT_TEST_4HZ_2X);  // generate internal test signals
       adc_wreg(CONFIG3,char(PD_REFBUF | CONFIG3_const)); //PD_REFBUF used for test signal, activa la referencia interna
       delay(150);
       for (int i = 1; i <= gMaxChan; i++){
           switch(valor){
               case 1:  adc_wreg(char(CHnSET + i), char(ELECTRODE_INPUT | GAIN_1X )); break;
               case 2:  adc_wreg(char(CHnSET + i), char(ELECTRODE_INPUT | GAIN_2X )); break;
               case 3:  adc_wreg(char(CHnSET + i), char(ELECTRODE_INPUT | GAIN_3X )); break;
               case 4:  adc_wreg(char(CHnSET + i), char(ELECTRODE_INPUT | GAIN_4X )); break;
               case 5:  adc_wreg(char(CHnSET + i), char(ELECTRODE_INPUT | GAIN_2X )); break;
               case 6:  adc_wreg(char(CHnSET + i), char(ELECTRODE_INPUT | GAIN_6X )); break;
               case 7:  adc_wreg(char(CHnSET + i), char(ELECTRODE_INPUT | GAIN_8X )); break;
               case 8:  adc_wreg(char(CHnSET + i), char(ELECTRODE_INPUT | GAIN_12X ));break;
           }
        } 
        detectActiveChannels(); 
        isRDATAC = true;
        adc_send_command(RDATAC); 
        adc_send_command(START); 
}

void detectActiveChannels() {  
  //actualiza gActiveChan y gNumActiveChan
  //
  using namespace ADS1298; 
  gNumActiveChan = 0;
  for (int i = 1; i <= gMaxChan; i++) {
     delayMicroseconds(1); 
     int chSet = adc_rreg(CHnSET + i);
     gActiveChan[i] = ((chSet & 7) != SHORTED);
     if ( (chSet & 7) != SHORTED) gNumActiveChan ++;   
  }
}

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
   WiredSerial.println(F("hlp -- ayuda "));
   WiredSerial.println(F("simN -- N1, señal normal. N2, test. N3 simulada")); 
   WiredSerial.println(F("rec -- chorro datos on/off")); 
   WiredSerial.println(F("sim -- señal simulada on/off"));    
   WiredSerial.println(F("frmN -- formatos de salida: N=0,va cambiando. Ahora hay 8")); 
   
}

void f_proto(int arg_cnt, char **args) {
    if(arg_cnt > 0) {
      modo_salida = atoi(args[0]);
    }
}



void inicia_serial_pc(){
  gLetra=new char[80];  
  WiredSerial.begin(SERIAL_SPEED); 
  while (WiredSerial.read() >= 0) {} ;
  HC06.begin(SERIAL_SPEED_BT); //use native port on Due
  while (HC06.read() >= 0) {} ;
  delay(200);  // Catch Due reset problem
}


void setup(){
  crea_seno();
  inicia_serial_pc();
  inicia_hw();

  if(!gtestCONTINUO){
      mensaje_inicio();
      while(1);
      //nos quedamos colgados para terminar
  }
  
  if( gtestSignal )
      ads_misetup_ADS1299(MODE_SENAL_TEST);
  else
      ads_misetup_ADS1299(MODE_SENAL_REAL_1x);
  
}

void imprime_linea( boolean modo){
   for (int i = 1; i < numSerialBytes; i+=3)
   {
    long numero = to_Int32(serialBytes+i);
     if(modo) {
                  WiredSerial.print(numero,HEX);
//                  WiredSerial.print(SEPARADOR_SERIAL );
                  HC06.print(numero,HEX);
//                  HC06.print(SEPARADOR_SERIAL );
     } else {
                  WiredSerial.print(numero);
                  HC06.print(numero);
                  if(i<numSerialBytes-3){
                      WiredSerial.print(SEPARADOR_SERIAL );
                      HC06.print(SEPARADOR_SERIAL );
                  }
                  
            }      
    }
//añadimos un ; al final 
    WiredSerial.println(FINLINEA);
    HC06.println(FINLINEA);
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
        WiredSerial.print(SEPARADOR_SERIAL );
        ind++;
     }
     for(int m=0;m<6;m++){
        int vv=((int)txBuf[ind++] << 8);
        vv += (int)txBuf[ind++] ;
        WiredSerial.print(vv );
        WiredSerial.print(SEPARADOR_SERIAL );
     }
     WiredSerial.print(txBuf[ind]  );
//añadimos un ; al final 
    WiredSerial.println(";");
    HC06.println(";");

     } else {
     WiredSerial.write(txBuf,17);
     }
}

void imprime_openBCI_V3(int modo_bci_protocolo){
   int ind;
   int indj=0;
   static int count = -1;
   unsigned long ul;
   dos_int dosnum;

   ind=2;
   txBuf[0]=0xA0;
   txBuf[1]=indice_paquete++;
   for (int i = 1; i < numSerialBytes; i+=3)
   {
     txBuf[ind++] = serialBytes[i];
     txBuf[ind++] = serialBytes[i+1];
     txBuf[ind++] = serialBytes[i+2];
   }
      //los acelerometros, no los tenemos...
     txBuf[ind++] = 0;
     txBuf[ind++] = 0;
     txBuf[ind++] = 0;
     txBuf[ind++] = 0;
     txBuf[ind++] = 0;
     txBuf[ind++] = 0;
     txBuf[ind]=0xC0; 

   ind=0;
   if(modo_bci_protocolo==1){    
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
    HC06.println(";");

  } else {
    if(modo_bci_protocolo==2){
        WiredSerial.write(txBuf,33);
    } else {    
         char letras[5];
         for(int jj=0;jj<33;jj++){
          sprintf(letras, "%02X",txBuf[jj]);
          WiredSerial.print(letras);
         }
         WiredSerial.println("");
    }
  }
}


void lee_datos_ads1299(void) { 
// hardware puro, 
// lee el ads y lo pone en serialBytes[]--numSerialBytes
    int i = 0;
    int jj=0;
    byte muestra[3];
    numSerialBytes = 1 + (3 * gNumActiveChan); //8-bits header plus 24-bits per ACTIVE channel

      
// cs a 0, empezamos a leer el ads1299	  
      digitalWrite(IPIN_CS, LOW);
      serialBytes[i++] =SPI.transfer(0); //get 1st byte of header
      SPI.transfer(0); //skip 2nd byte of header
      SPI.transfer(0); //skip 3rd byte of header
      for (int ch = 1; ch <= gMaxChan; ch++) {
        if( !gSimuladaSignal ){
              serialBytes[i++] = SPI.transfer(0);
              serialBytes[i++] = SPI.transfer(0);
              serialBytes[i++] = SPI.transfer(0);
              long vlast= to_Int32(serialBytes+i-3);
              long diff= ultima_lectura[ch] - vlast;
              if (abs(diff)>250000  ){
                 to_3bytes(0,serialBytes+i-3);
              } else {
                 ultima_lectura[ch]=vlast;
              }
              
        } else {      
              // señal seno, creada al inicio 
              jj=contador_muestras%TABLE_SIZE;
              to_3bytes(samples[jj]*100,muestra);
              if(ch!=166){
                  serialBytes[i++] = muestra[0];
                  serialBytes[i++] = muestra[1];
                  serialBytes[i++] = muestra[2];
              } else {
                  serialBytes[i++] = 0;
                  serialBytes[i++] = 0;
                  serialBytes[i++] = 0;
              }
        }
      }
// cs a 1, terminamos de leer el ads1299    
      delayMicroseconds(1);
      digitalWrite(IPIN_CS, HIGH);
}

void procesaComando(String texto){
     String parametro;
     WiredSerial.print(texto);
     WiredSerial.println("-->ok");

     if(texto.startsWith("sim")){ // 1 normal  2 test 3 simulada 4 max ganancia
         parametro=texto.substring(3,4);
         int p1=parametro.toInt();
         switch(p1){
            case 1:
              gSimuladaSignal=false;
              gtestSignal=false;
              ads_misetup_ADS1299(MODE_SENAL_REAL_1x);
            break;
            case 2:
              gSimuladaSignal=false;
              gtestSignal=true;
              ads_misetup_ADS1299(MODE_SENAL_TEST);

            break;
            case 3:
              gtestSignal=false;
              gSimuladaSignal=true;

            break;
           case 4:
              gSimuladaSignal=false;
              gtestSignal=false;
              ads_misetup_ADS1299(MODE_SENAL_REAL_12x);

            break;
            
            
         }       
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
            WiredSerial.println("cambiado modo");
         }       
      } else if(texto.startsWith("rec")){
         parametro=texto.substring(3,4);
         int p1=parametro.toInt();
         if(p1==0){
            ultimo_modo=modo_salida;
            modo_salida=8;            
         } else {
            modo_salida=ultimo_modo;
         }
         return;
      } else if(texto.startsWith("gan")){ 
         parametro=texto.substring(3,4);
         int p1=parametro.toInt();
         gSimuladaSignal=false;
         gtestSignal=true;
         ads_setupGanancia(p1);
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

void loop()
{

  leeSerial();
    
  if(gtestCONTINUO && isRDATAC && digitalRead(IPIN_DRDY) == LOW ){
     lee_datos_ads1299();
     contador_muestras++;

     switch(modo_salida){
      case 1: 
         imprime_linea(true);
         break;
      case 2:
         imprime_linea(false);
         break;
      case 3:   
         imprime_openEEG_p2(true);
         break;
      case 4:   
         imprime_openEEG_p2(false);
         break;
      case 5:   
         imprime_openBCI_V3(1);
         break;
      case 6:   
         imprime_openBCI_V3(2);
         break;
      case 7:   
         imprime_openBCI_V3(3);
         break;
      case 8:   
         no_imprime_nada(true);
         break;
         
     }
  }   
}




