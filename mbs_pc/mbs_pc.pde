///////////////////////////////////////////////
//
// GUI for controlling the ADS1299-based signalino
//
// Created: Juan Barios, august 2016 - january 2017
// Modified: 
//
// Requires ControP5 library
//
// No warranty.  Use at your own risk.  Use for whatever you'd like.
// 
///////////////////////////////////////////////

import controlP5.*;

import java.util.ArrayList;
import java.util.Arrays;
import java.io.FileWriter;
 
import processing.serial.*;
import javax.swing.JOptionPane;//For user input dialogs

int BAUD_RATE = 57600;//230400;//921600;//460800;//921600; //921600 for Teensy2/Teensy3/Leonardo 460800
int numCanales = 8; //number of channels to report
int serialPortNumber = 0; //set to 0 for automatic detection


Serial port;      // Create object from Serial class
int[]  lectura;              // Datos leidos en el puerto serie



int anchoPantalla=1000;
int altoPantalla=600;
Chart ADS4ch;


boolean modo_conectado = false;
boolean modo_test = false;


ControlP5 gui;


//create a file first    
File file ;
FileWriter fw;
PrintWriter outputfile;
    
    
void setup() {
  size(1000, 600);
  rectMode(CENTER);

  println("Signalino, signal visor 0.1 (c) 2017 ILSB ");

  lectura=new int[numCanales];

  modo_conectado=false;
  if (!modo_test){
     serie_inicia();
     //pone el ads en modo 6, bytes openBCI
     sendComando("frm6",port);
     sendComando("sim3",port);     
  }   
  ADS4ch = new Chart(anchoPantalla/2-50,altoPantalla/2,anchoPantalla-100,altoPantalla-50,numCanales,0.01);
  gui = new ControlP5(this);
try {
  file = new File("./datos_simula2.txt");
  fw = new FileWriter(file);
  outputfile = new PrintWriter(fw);
} catch (IOException e) {
  println("It Broke");
  e.printStackTrace();
} 
//replace your System.out.print("your output");
  iniciaGui(gui);
}

void draw() {
     if(modo_conectado)serDecode(ADS4ch.bf);
     else serRand();
   //  serEco();
     ADS4ch.set(lectura);
     ADS4ch.update();
}