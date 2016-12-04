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
        decimando=64;
  }
  void pinta(int[] x1) {
  //    v.pintaVector(new arr_int((new arr_int(x1)).mov_avg(3)).zscore(),escala);
        v.pintaVector(x1,escala);
    //   v.pintaVector((new arr_int(x1)).mov_avg(3),escala);
 
}
  
  void xpinta(arr_int x1) {
      v.pintaVector(x1.mov_avg_r(3),escala);
  }
}  