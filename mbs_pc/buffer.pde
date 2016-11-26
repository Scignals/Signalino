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
    int ultima_lectura=0;
    if((++puntero)>=max_buffer)puntero=0;
    for(int i=0;i<num_canales;i++){
      // eliminamos lecturas con un cambio muy brusco respecto al ultimo valor
      if(puntero==0)ultima_lectura=datos[i][max_buffer-1];
      else ultima_lectura=datos[i][puntero-1];
      if((abs(x1[i]-ultima_lectura))<500000/8)
         datos[i][puntero]=x1[i];
      else
         datos[i][puntero]=ultima_lectura;
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
    int suma=0;
    float sumasq=0;
    for(int i=0;i<longitud;i++)d[i]=c[i*decimando];
    //quitamos offset 
    for(int i=0;i<longitud;i++){
      suma+=d[i];
      sumasq+=(d[i]*d[i]);
    } 
      for(int i=0;i<longitud;i++)d[i]-=suma/longitud;
      float misd=sqrt((sumasq-(suma*suma)/(longitud+1))/(longitud+1));
//      for(int i=0;i<longitud;i++)d[i]/=misd;
//      for(int i=0;i<longitud;i++)d[i]*=1000;
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