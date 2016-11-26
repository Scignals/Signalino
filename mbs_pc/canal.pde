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
 //     v.pintaVector(new arr_int((new arr_int(x1)).offset()).mov_avg(30),escala);
        v.pintaVector(x1,escala);

}
  
  void pinta(arr_int x1) {
      v.pintaVector(x1.mov_avg(30),escala);
  }
}  