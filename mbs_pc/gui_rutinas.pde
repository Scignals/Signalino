
void iniciaGui(ControlP5 gui) { //assuming Arduino is only transfering active channels

  gui.addRadioButton("Senal")
   .setPosition(910,300)
   .setSize(10,27)
   .setValue(1)
   .addItem("senal",1)
   .addItem("test",2)
   .addItem("simul",3)
   .addItem("g=1",4)
   
   ;

  gui.addTextlabel("Memboost")
   .setText("Memboost Signal visor")
   .setPosition(10,2)
   .setSize(10,27)
   .setColorValue(0xffffff00)
   .setFont(createFont("Georgia",20))
   ;

   
  gui.addSlider("Escala")
   .setPosition(910,30)
   .setRange(1, 20)
   .setSize(80, 10)
   .setNumberOfTickMarks(10)
   .setValue(10)
     ;
 
  gui.addSlider("Tiempo")
   .setPosition(910,80)
   .setRange(1, 20)
   .setSize(80, 10)
   .setNumberOfTickMarks(10)
   .setValue(1)
     ;

  gui.addTextlabel("lab1")
   .setText("<esc>    exit")
   .setPosition(910,300)
   .setSize(10,27)
   .setColorValue(0x00000000)
   .setFont(createFont("Georgia",10))
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
     case 4:
       sendComando("sim4;",port);
       break;  

  }
}

public void Escala(float value){
  float escala_base=0.00001;
  ADS4ch.setEscala(value*escala_base);
}

public void Tiempo(int value){
  ADS4ch.setDecimando(value);
}