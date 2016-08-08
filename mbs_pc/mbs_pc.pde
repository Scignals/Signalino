import java.util.ArrayList;
import processing.serial.*;
import javax.swing.JOptionPane;//For user input dialogs


int BAUD_RATE = 57600;//230400;//921600;//460800;//921600; //921600 for Teensy2/Teensy3/Leonardo 460800
int numCanales = 4; //number of channels to report
int serialPortNumber = 0; //set to 0 for automatic detection

Serial port;      // Create object from Serial class
int[]  lectura;              // Data received from the serial port



int anchoPantalla=1000;
int altoPantalla=600;
Chart ADS4ch;


boolean modo_conectado = false;
boolean modo_test = false;




void setup() {
  size(1000, 600);
  rectMode(CENTER);

  lectura=new int[numCanales];

  modo_conectado=false;
  if (!modo_test){
     serie_inicia();
  }   
  ADS4ch = new Chart(anchoPantalla/2,altoPantalla/2,990,550,numCanales);
  
}

void draw() {
     if(modo_conectado)serDecode();
     else {
        for(int i=0;i<numCanales;i++){
           lectura[i]=(int) random(1,100);
        }
     }   
     ADS4ch.set(lectura);
     ADS4ch.update();
 
}