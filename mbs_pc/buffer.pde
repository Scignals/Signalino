class Buffer {
  
  int [][] datos;
  int num_canales;
  int max_buffer;
  int puntero;
  int [] umbral_max;
  int [] umbral_min;
  int [] offset;
  
  
  Buffer(int nc , int mb ) {
        datos=new int [nc][mb] ;
        umbral_max=new int[nc];
        umbral_min=new int[nc];
        offset=new int[nc];
        
        
        puntero=0;
        num_canales=nc;
        max_buffer=mb;        

        for(int i=0;i<nc;i++)umbral_max[i]=1000000000;
        for(int i=0;i<nc;i++)offset[i]=0;
        
   
}
  
  
  void apunta(int[] x1) {
    if((++puntero)>=max_buffer)puntero=0;
    for(int i=0;i<num_canales;i++){
               datos[i][puntero]=x1[i];
      }
      
   }

  
  int [] lee_canal(int canal,int longitud, int decimando) {
  //print((datos[canal].length));
  //print("puntero");
  //println( puntero);
  //println (longitud);

    int longitud2=longitud*decimando;
    int [] c=new int [longitud2];
    int [] d=new int [longitud];
    
  
    if(puntero>longitud2)
        System.arraycopy(datos[canal], puntero-longitud2, c, 0, longitud2 );
    else{
        System.arraycopy(datos[canal], max_buffer+puntero-longitud2, c, 0, longitud2-puntero );
        System.arraycopy(datos[canal], 0, c, longitud2-puntero, puntero );
    }

    for(int i=0;i<longitud;i++)d[i]=c[i*decimando];
    
    //moving average de 3 puntos (despues de decimar)
    double suma,suma2;
    int npuntos=3;
    suma=0;suma2=0;
    for (int i=longitud-npuntos; i>npuntos; i--){
       for (int j=0; j<npuntos; j++){
            suma+=d[i-j];
       }
       d[i]=(int)((suma)/npuntos);
       suma2+=d[i];
       suma=0;
    }

    //quitamos offset 
    for(int i=0;i<longitud;i++){
       d[i]-=suma2/longitud;
    } 
    
    return(d);    
     
  }

  void limpia(){
        datos      = new int [num_canales][max_buffer] ;
        umbral_max = new int[num_canales];
        umbral_min = new int[num_canales];
        offset     = new int[num_canales];
        
        for(int i=0;i<num_canales;i++)offset[i]=0;
 }

  void calcula_umbrales() {
    for(int i=0;i<num_canales;i++){
       for(int j=0;j<max_buffer;j++){
           umbral_max[i]+=abs(datos[i][j]);
      }
    }
    
    for(int i=0;i<num_canales;i++){
           umbral_max[i]/=max_buffer;
    }
  }
    
}  