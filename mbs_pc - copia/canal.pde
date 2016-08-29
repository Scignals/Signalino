class Canal {
  
  int x;
  int y;
  int ancho;
  ventana v;
  float escala;  
  int decimando;
  Canal(int x,int y,int w,int h,float e) {
        v=new ventana(x,y,w,h);
        escala=e;
        decimando=2;
  }
  void pinta(int[] x1) {
      v.pintaVector(x1,escala);
  }
}  