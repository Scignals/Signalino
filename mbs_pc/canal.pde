class Canal {
  
  int x;
  int y;
  int ancho;
  ventana v;
  float escala;  
  Canal(int x,int y,int w,int h) {
        v=new ventana(x,y,w,h);
        escala=0.1;
  }
  void pinta(int[] x1,float escala) {
      v.pintaVector(x1,escala);
  }
}  