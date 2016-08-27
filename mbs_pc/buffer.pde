class Buffer {
  
  int [][] datos;
  int num_canales;
  int max_buffer;
  int puntero;
  int [] umbral_max;
  int [] umbral_min;
  
  Buffer(int nc , int mb ) {
        datos=new int [nc][mb] ;
        umbral_max=new int[nc];
        umbral_min=new int[nc];
        
        puntero=0;
        num_canales=nc;
        max_buffer=mb;        

        for(int i=0;i<nc;i++)umbral_max[i]=1000000000;
   
}
  
  
  void apunta(int[] x1) {
   // println(puntero);
    if((++puntero)>=max_buffer)puntero=0;
    for(int i=0;i<num_canales;i++)
      datos[i][puntero]=x1[i];
       // println(puntero);

   }

  int [] lee(int puntos) {
    int [] salida=new int[num_canales];
    for(int i=0;i<num_canales;i++){
      salida[i]=datos[i][puntero];
    }
    return(salida);
  }

  int [] lee_canal(int canal,int longitud) {
   // print((datos[canal].length));
   // print("puntero");
   // println( puntero);
   // println (longitud);

    int [] c=new int [longitud];
  
    if(puntero>longitud)
        System.arraycopy(datos[canal], puntero-longitud, c, 0, longitud );
    else{
        System.arraycopy(datos[canal], max_buffer+puntero-longitud, c, 0, longitud-puntero );
        System.arraycopy(datos[canal], 0, c, longitud-puntero, puntero );
    }
    return(c);    
     
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