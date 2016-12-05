///////////////////////////////////////////////
//
// GUI for controlling the ADS1299-based signalino
//
//
/*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.

* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.

* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

* Author: JABarios, agosto 2016 - enero 2017
* Modified:
* Company: ILSB, Spain. 
* ---------------------------------------------
* Requires ControP5 library
*/

///////////////////////////////////////////////

import controlP5.*;


import java.util.*;
import java.util.ArrayList;
import java.util.Arrays;
import java.io.FileWriter;

 
import processing.serial.*;
import javax.swing.JOptionPane;//For user input dialogs

int BAUD_RATE = 115200;//57600 115200 230400;//921600;//460800;//921600; //921600 for Teensy2/Teensy3/Leonardo 460800
int numCanales = 8; //number of channels to report
int anchoPantalla=1000;
int altoPantalla=600;

boolean modo_conectado = false;
boolean modo_test      = false;
boolean gGrabando      = false;
boolean gui_running    = true;

int serialPortNumber = 0; //set to 0 for automatic detection


// global vars containing the gui
ControlP5 gui;
// and the chart recorder
Chart ADS4ch;
// and the serial port
Serial port;      


//create a file first for recording signal    
File file;
FileWriter fw;
PrintWriter outputfile;

//global, usada por varias funciones (creo) 
int[]  lectura;   // Datos leidos en el puerto serie
  
    
void setup() {
  size(1000, 600);
  rectMode(CENTER);

  println("Signalino, signal visor 0.3 (c) 2017 ILSB ");

  lectura=new int[numCanales];

  modo_conectado=false;
  if (!modo_test){
     serie_inicia();
     //pone el ads en modo 6 ( bytes openBCI ). antes manda un "oka" xq si no el arduino no responde
     sendComando("oka",port);
     sendComando("frm6",port);
  }   
  ADS4ch = new Chart(anchoPantalla/2-50,altoPantalla/2,anchoPantalla-100,altoPantalla-50,numCanales,0.01);
  gui = new ControlP5(this);
  try {
    file = new File("./signalino_rawdata.txt");
    fw = new FileWriter(file);
    outputfile = new PrintWriter(fw);
  } catch (IOException e) {
    println("File error. Sorry...");
    e.printStackTrace();
  } 
  iniciaGui(gui);
  sendComando("sim1",port);
  println("llegamos");
  
}

void draw() {
     if(modo_conectado)serDecode(ADS4ch.bf);
     else serRand(ADS4ch.bf);
     if(gui_running)ADS4ch.update();
     
     // esto hay q meterlo como metodo de gui2
       fill(0,0,0);
       rect(810,585,60,20);
       fill(255,255,255);
       text(fm_calculada+"  ",800,590); 
      
     
}