class ventana {
  int _ctro_x;int cx(){return(_ctro_x);}
  int _ctro_y;int cy(){return(_ctro_y);}
  int _mrgI;int mrgI(){return(_mrgI);}
  int _mrgD;int mrgD(){return(_mrgD);}
  int _mrgS; int mrgS(){return(_mrgS);}
  int _mrgF;int mrgF(){return(_mrgF);}
  int _ancho;int ancho(){return(_ancho);}
  int _alto;int alto(){return(_alto);}
  color _fondo;color fondo(){return(_fondo);}
  

  // constructor, calcula coordenadas y pinta fondo de color
  ventana(int p_cx, int p_cy, int p_ancho, int p_alto) {
    _ctro_x=p_cx; _ctro_y=p_cy;
    _ancho=p_ancho;_alto=p_alto;
    _mrgI=_ctro_x-(_ancho/2);
    _mrgD=_ctro_x+_ancho/2;
    _mrgS=_ctro_y-_alto/2;
    _mrgF=_ctro_y+_alto/2;
    _fondo = color(255, 253, 248);

    dibuja_fondo();
    }
  
  // dibuja de color _fondo la ventana
  void dibuja_fondo(){
      noStroke();
      fill(_fondo);  // color for the window background
      rect(_ctro_x, _ctro_y, _ancho, _alto);
      
  }
  // pinta cursor, si n<0, con valor relativo (-0.5, justo en la mitas), si n>0 en cx=n 
  void cursor(float n){
     fill(255, 0, 0);  // color for the window background
     if(n<0){
       float cx=(float)_ancho*n;
       rect((int)cx,_mrgI, 1,_alto );
     } else {
       rect(n,_ctro_y, 1,_alto );
     }
     
  }
  
    // 4 funciones, cambiando el tipo de los parametros de entrada, que pintan un vector en la ventana
    // incluyen limpiar el fondo antes de dbujar
    void pintaVector(int[] x1,float escala) {
      float cx1,cy1;
      dibuja_fondo();
 //     rectMode(CORNER);
      stroke(250, 0, 0);     //red                          
      noFill();
      beginShape();        
      cx1=0;
      for (int i = 0; i < x1.length; i++) {
        cx1=(float)map(i,0,x1.length,mrgI(),mrgD());
        cy1=cy()+escala*x1[i];
        cy1=cy1<mrgS()?mrgS():cy1;
        vertex(cx1,  cy1>mrgF()?mrgF():cy1);
        cy1=cy1<mrgS()?mrgS():cy1;
        vertex(cx1,  cy1);

    
  }
      endShape();
//      rectMode(CENTER);

  }



}