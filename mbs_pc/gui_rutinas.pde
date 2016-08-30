
void iniciaGui(ControlP5 gui) { //assuming Arduino is only transfering active channels

  gui.addRadioButton("Senal")
   .setPosition(910,300)
   .setSize(10,27)
   .setValue(1)
   .addItem("senal",1)
   .addItem("test",2)
   .addItem("simul",3)
   
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
   .setPosition(910,500)
   .setSize(10,10)
   .setColorValue(0x00000000)
   .setFont(createFont("Georgia",10))
   ;

  gui.addRadioButton("Ganancia")
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
}

public void Escala(float value){
  float escala_base=0.00001;
  ADS4ch.setEscala(value*escala_base);
}

public void Tiempo(int value){
  ADS4ch.setDecimando(value);
}

public void Ganancia(int value){
       sendComando("gan"+(value+1),port); 
       println("gan"+(value+1));
}