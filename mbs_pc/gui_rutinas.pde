float escala_multiplicador=1;
float escala_base=0.00001; //valor para q se vea simul
float escala_valor=1;
 
void iniciaGui(ControlP5 gui) { //assuming Arduino is only transfering active channels

  gui.addRadioButton("Senal")
   .setPosition(910,300)
   .setSize(10,27)
   .setValue(1)
   .addItem("senal",1)
   .addItem("test",2)
   .addItem("simul",3)
   
   ;

  gui.addTextlabel("Signalino")
   .setText("Signalino visor 0.2")
   .setPosition(10,2)
   .setSize(10,27)
   .setColorValue(0xffffff00)
   .setFont(createFont("Georgia",20))
   ;

   
  gui.addSlider("VisorGanancia")
   .setPosition(960,10)
   .setRange(1, 20)
   .setSize(10, 60)
   .setNumberOfTickMarks(10)
   .setValue(10)
     ;

  gui.addRadioButton("Escala")
   .setPosition(900,30)
   .setSize(27,10)
   .setValue(1)
   .addItem("x1",1)
   .addItem("x10",2)
   .addItem("x100",3)
   
   ;
 
  gui.addSlider("Tiempo")
   .setPosition(910,90)
   .setRange(1, 20)
   .setSize(80, 10)
   .setNumberOfTickMarks(10)
   .setValue(1)
     ;

  gui.addTextlabel("lab1")
   .setText("<esc>    exit")
   .setPosition(910,500)
   .setSize(10,10)
   .setColorValue(0x00000000)
   .setFont(createFont("Georgia",10))
   ;

  gui.addRadioButton("ADSGanancia")
                .setPosition(910, 120)
                .setColorForeground(color(120))
                .setColorActive(color(255))
                .setColorLabel(color(255))
                .setSize(8,10)
                .addItem("1", 0)
                .addItem("2", 1)
                .addItem("3", 2)
                .addItem("4", 3)
                .addItem("6", 4)
                .addItem("8", 5)
                .addItem("24", 6)
                
                ;

  gui.addRadioButton("Protocolo")
                .setPosition(950, 120)
                .setColorForeground(color(120))
                .setColorActive(color(255))
                .setColorLabel(color(255))
                .setSize(8,10)
                .addItem("p1", 0)
                .addItem("p2", 1)
                .addItem("p3", 2)
                .addItem("p4", 3)
                .addItem("p5", 4)
                .addItem("p6", 5)
                
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
   float valor = gui.getController("VisorGanancia").getValue();;
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

public void Protocolo(int value){
  println("cambiando protocolo senal a "+value);
  if(value>0 && value<7){
       sendComando("frm"+(value),port); 
       println("frm"+(value+1));
  }
}