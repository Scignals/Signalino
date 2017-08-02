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
//  Copyright © 2016 JA Barios
//  This file is part of project: SCIGNALS, a chart recorder.
//
float escala_multiplicador=1;
float escala_base=0.00001; //valor para q se vea simul
float gCteTiempo=64*4;
float escala_valor=1;
String nombre_archivo="scignals_raw";

DropdownList d1;
 StringList l;

void iniciaGui(ControlP5 gui) { //assuming Arduino is only transfering active channels

  gui.addRadioButton("Senal")
   .setPosition(910,300)
   .setSize(10,27)
   .setValue(3)
   .addItem("senal",3)
   .addItem("test",2)
   .addItem("simul",1)
   
   ;
  gui.addRadioButton("BT")
   .setPosition(350,1)
   .setSize(27,10)
   .setValue(0)
   .addItem("BT on",1)
   .addItem("off",0)
   
   ;

  gui.addTextlabel("Signalino")
   .setText("Scignals   0.3")
   .setPosition(10,2)
   .setSize(10,27)
   .setColorValue(0xffffff00)
   .setFont(createFont("Georgia",20))
   ;

   
  gui.addSlider("VisorGanancia")
   .setPosition(910,10)
   .setRange(1, 20)
   .setSize(10, 60)
   .setNumberOfTickMarks(10)
   .setValue(10)
   .setCaptionLabel("Ganancia")
     ;

  gui.addRadioButton("Escala")
   .setPosition(950,30)
   .setSize(27,10)
   .addItem("x1",1)
   .addItem("x10",2)
   .addItem("x100",3)
   .setValue(1)
   
   ;

 
  gui.addSlider("Tiempo")
   .setPosition(910,100)
   .setRange(1, 20)
   .setSize(80, 10)
   .setNumberOfTickMarks(10)
   .setValue(1)
   .setCaptionLabel("barrido")
     ;

  gui.addTextlabel("lab1")
   .setText("<esc> exit")
   .setPosition(10,580)
   .setSize(10,10)
   .setColorValue(0x00000000)
   .setFont(createFont("Georgia",10))
   ;

  gui.addTextlabel("lab2")
   .setText("<+/-> Ganancia")
   .setPosition(90,580)
   .setSize(10,10)
   .setColorValue(0x00000000)
   .setFont(createFont("Georgia",10))
   ;

  gui.addTextlabel("lab3")
   .setText("<space> Pausa")
   .setPosition(170,580)
   .setSize(10,10)
   .setColorValue(0x00000000)
   .setFont(createFont("Georgia",10))
   ;
   

 gui.addTextfield("NombreArchivo")
   .setPosition(905,520)
   .setSize(90,20)
//   .setFocus(true)
   .setFont(createFont("Georgia",12))
   .setColor(color(255,255,255))
   .setAutoClear(false)
   .setCaptionLabel("")
   ;

  gui.addToggle("grabando")
     .setCaptionLabel("Grabando")
     .setPosition(925,550)
     .setSize(50,20)
     .setValue(gGrabando)
     .setMode(ControlP5.SWITCH)
     .setColorActive(color(64,64,64))
     .setColorBackground(color(122,122,122))
     .setColorForeground(color(122,122,122))
     ;
     

  gui.addRadioButton("ADSGanancia")
                .setPosition(910, 150)
                .setColorForeground(color(120))
                .setColorActive(color(255))
                .setColorLabel(color(255))
                .setSize(8,10)
                .addItem("1", 0)
                .addItem("2", 1)
                .addItem("4", 2)
                .addItem("6", 3)
                .addItem("8", 4)
                .addItem("12", 5)
                .addItem("24", 6)
                
                ;

  gui.addRadioButton("Protocolo")
                .setPosition(950, 120)
                .setColorForeground(color(120))
                .setColorActive(color(255))
                .setColorLabel(color(255))
                .setSize(8,10)
                .setVisible(false)
                .addItem("p1", 0)
                .addItem("p2", 1)
                .addItem("p3", 2)
                .addItem("p4", 3)
                .addItem("p5", 4)
                .addItem("p6", 5)
                
                ;
  
   l=new StringList();
   l.append("Cyberamp 80"); //SRB2 min ganancia
   l.append("EMG");         //bipolar min gaancia
   l.append("EEG");         //bipolar max ganancia
   l.append("PSG");
   gui.addScrollableList("Entrada")
       .setPosition(200, 1)
      // .setSize(200, 20)
       .setBarHeight(20)
       .setItemHeight(20)
       .addItems(l.array())
       .setValue(0)
       .setType(ScrollableList.DROPDOWN) // currently supported DROPDOWN and LIST
       ;

   
 }
    

public void controlEvent(ControlEvent theEvent) {
//Is called whenever a GUI Event happened
}

public void Senal(int value){
  println("cambiando modo senal a "+value);
  switch(value){
     case 1:
       sendComando("sim1;",port); 
       break;
     case 2:
       sendComando("sim2;",port);
       break;
     case 3:
       sendComando("sim3;",port);
       break;  

  }
  ADS4ch.reset();
}

public void BT(int value){
  println("pasando bluetooth a "+value);
  switch(value){
     case 1:
     
       sendComando("blt1;",port); 
       break;
     case 0:
       sendComando("blt0;",port);
       break;
  }
  ADS4ch.reset();
}

public void Escala(int value){
  println("cambiando modo escala a "+value);
  switch(value){
     case 1:
       escala_multiplicador=1;
       break;
     case 2:
       escala_multiplicador=10;
       break;
     case 3:
       escala_multiplicador=100;
       break;  

  }
   float valor = gui.getController("VisorGanancia").getValue();
     ADS4ch.setEscala(valor*escala_base*escala_multiplicador);

}

public void VisorGanancia(float value){
   ADS4ch.setEscala(value*escala_base*escala_multiplicador);
 // ADS4ch.setEscala(value*0.0001);
  // 0.0001 test, /10 simul, 
  
}




public void Tiempo(int value){
  ADS4ch.setDecimando(value);
}

public void ADSGanancia(int value){
       sendComando("gan"+(value+1),port); 
       println("gan"+(value+1));
}

public void Entrada(int value){
  
  println("cambiando Periférico de entrada a "+value);
  switch(value){
     case 0:
      sendComando("inp1",port); 
       break;
     case 1:
      sendComando("inp2",port); 
       break;
     case 2:
      sendComando("inp3",port); 
       break;  

  }
 
}

public void grabando(boolean value){
    gGrabando=value;
    println("grabando:"+gGrabando);
    nombre_archivo = gui.get(Textfield.class,"NombreArchivo").getText();
   
    if(gGrabando){
      //if(nombre_archivo.length()<1)return;
       try {
          file = new File(nombre_archivo);
          fw = new FileWriter(file,true);
          outputfile = new PrintWriter(fw);
       } catch (IOException e) {
          println("File error while creating file. Sorry...");
          //e.printStackTrace();
          gGrabando=false;
          gui.getController("grabando").setColorActive(color(255,0,0));
          gui.getController("grabando").setCaptionLabel("OFF");
          return;
      }
      gui.getController("grabando").setCaptionLabel("Recording ON");
      gui.getController("grabando").setColorActive(color(0,255,0));

    }
    
    else {
       gui.getController("grabando").setColorActive(color(255,0,0));
       gui.getController("grabando").setCaptionLabel("OFF");
       outputfile.flush();  // Writes the remaining data to the file
       outputfile.close();  // Finishes the file
    }
   
  }
  
public void Protocolo(int value){
  println("cambiando protocolo senal a "+value);
  if(value>0 && value<7){
       sendComando("frm"+(value),port); 
       println("frm"+(value+1));
  }
}

void keyPressed() {
 if(key=='+') {
     float value=gui.getController("VisorGanancia").getValue();
     gui.getController("VisorGanancia").setValue(value+3);
 } 
if(key=='-') {
     float value=gui.getController("VisorGanancia").getValue();
     gui.getController("VisorGanancia").setValue(value-3);
 }
 if(key==' ') {
     gui_running=!gui_running;
 
 }
 
 if(key==27) {
   if(gGrabando){ 
     key=0;   
     javax.swing.JOptionPane.showMessageDialog(frame,
            "<html><div align='center'>Scignals v 0.3 (c) 2016</div>"+
           "<p>Recording is ON, please stop it before exit...</p></html>");  
   }
   
 }   
}