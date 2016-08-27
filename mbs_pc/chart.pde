class Chart { 
  int ancho, alto, cx, cy;
  ArrayList<Canal> canales;
  int n_canales,ancho_canal, alto_canal;
  int puntero_canales;
  ventana v;
  int[][] datos;
  Buffer bf;

  Chart (int x, int y, int w, int h, int n, float escala) {  
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
      canales.add(new Canal(cx,v.mrgS()+3+alto_canal/2+(alto_canal)*i,ancho-10, alto_canal, escala));
    }
    
    bf=new Buffer(n,w*1);
  
   }
   
   void set(int [] valores){
      bf.apunta(valores);
   }
 
  void update() {
     for(int i=0;i<n_canales;i++){
        Canal chx = canales.get(i);
        chx.pinta(bf.lee_canal(i,ancho));
     }
  } 
  void setEscala(float e) {
     for(int i=0;i<n_canales;i++){
        Canal chx = canales.get(i);
        chx.escala=e;
     }
  } 

} 