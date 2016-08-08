class Chart { 
  int ancho, alto, cx, cy;
  ArrayList<Canal> canales;
  int n_canales,ancho_canal, alto_canal;
  int puntero_canales;
  ventana v;
  int[][] datos;

  Chart (int x, int y, int w, int h, int n) {  
    ancho = w; 
    alto = h;
    cx=x;
    cy=y;
    v=new ventana(cx,cy,ancho,alto);
    n_canales=n;
    puntero_canales=0;

    canales = new ArrayList<Canal>();
    int alto_canal=alto/n_canales;
    ancho_canal=ancho-10;
    

    for(int i=0;i<n;i++){
      canales.add(new Canal(cx,v.mrgS()+3+alto_canal/2+(alto_canal)*i,ancho-10, alto_canal));
    }
    datos = new int[n_canales][ancho_canal];
  
   }
   void set(int [] valores){
     puntero_canales++;
     if(puntero_canales>=ancho_canal)puntero_canales=0;
     for(int i=0;i<n_canales;i++){
         datos[i][puntero_canales]=valores[i];
     }
   }
 
  void update() {
     for(int i=0;i<n_canales;i++){
      Canal chx = canales.get(i);
      chx.pinta(datos[i],0.1);
     }
  } 
} 