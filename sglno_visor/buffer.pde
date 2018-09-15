// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 3
// of the License, or (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//  
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, 
// MA  02110-1301, USA.
// 
//
//  Copyright © 2016 JA Barios
//  This file is part of project: SCIGNALS, a chart recorder.
//
class Buffer {
  
  int [][] datos;
  int num_canales;
  int max_buffer;
  int puntero;
  int cte_tiempo;
  int p_cte_tiempo;
  int [] umbral_max;
  int [] umbral_min;
  int [][] offset;
  int [] sum_offset;
  
  
  
  Buffer(int nc , int mb ) {
        datos=new int [nc][mb] ;
        umbral_max=new int[nc];
        umbral_min=new int[nc];
        cte_tiempo=(int)gCteTiempo;  //numero magico, de momento. es mas o menos 1 segundo.
        offset=new int[nc][cte_tiempo];
        sum_offset=new int[nc];
        
        
        
        puntero=0;
        p_cte_tiempo=0;
        
        num_canales=nc;
        max_buffer=mb;        

        for(int i=0;i<nc;i++)umbral_max[i]=1000000000;
        for(int i=0;i<nc;i++)sum_offset[i]=0;
        
   
}
  
  void graba(int[] x1) {
      
             int i=0;
             outputfile.print(millis());             
             for (i = 0; i < numCanales-1; i++){
               outputfile.print(x1[i]);
               outputfile.print(',');
             }          
             outputfile.println(x1[i]);
  }  
  
  void apunta(int[] x1) {
   
    if((++puntero)>=max_buffer)puntero=0;
    if((++p_cte_tiempo)>=cte_tiempo)p_cte_tiempo=0;
    int previo_offset;
    
    for(int i=0;i<num_canales;i++){
               //calculamos offset de los ultimos cte_tiempo puntos
               previo_offset=offset[i][p_cte_tiempo];               
               offset[i][p_cte_tiempo]=x1[i];
               sum_offset[i]+=(x1[i]-previo_offset);
               //apuntamos el dato con el offset ya restado
               datos[i][puntero]=x1[i]-sum_offset[i]/cte_tiempo;
             }      
   }


  int [] lee_canal(int canal,int longitud, int decimando) {

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
    
    //moving average de 5 puntos (despues de decimar)
    double suma,suma2;
    int npuntos=5;
    suma=0;suma2=0;
    
      //algoritmo recursivo, operativo. muy chulo. 
      // adaptado de http://www.dspguide.com/CH15.PDF
      // el mio va marcha atras, y asi no hace falta un segundo buffer
      // i99=i98+i97+i96+i5 (moving avr de 4 puntos) --> i=((i+1)*n-i+(i-npuntos))/npuntos;
      // una formula para calcular frecuencia de corte aproximada es frec=fm*0.443/sqr(n²-1)
        int ii;
        for (ii=longitud-2; ii>longitud-npuntos-2; ii--){
             suma+=d[ii];
           }
        d[longitud-1]=(int)((suma)/npuntos);
        for (ii=longitud-3; ii>npuntos; ii--){
           d[ii+1]=(int)((npuntos*d[ii+2]-d[ii]+d[ii-npuntos+1])/npuntos);
           suma+=d[ii+1];
        }    
    
    return(d);    
     
  }

  void limpia(){
        datos      = new int [num_canales][max_buffer] ;
        umbral_max = new int[num_canales];
        umbral_min = new int[num_canales];
        sum_offset = new int[num_canales];
        
        for(int i=0;i<num_canales;i++)sum_offset[i]=0;
        for(int i=0;i<num_canales;i++)for(int j=0;j<cte_tiempo;j++)offset[i][j]=0;
        
 }

  
    
}  