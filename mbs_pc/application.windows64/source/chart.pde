// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
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
//  This file is part of project: SCIGNALS: a chart recorder
//
int MAX_DECIMANDO=64;
class Chart { 
  int ancho, alto, cx, cy;
  ArrayList<Canal> canales;
  int n_canales,ancho_canal, alto_canal;
  int puntero_canales;
  ventana v;
  int[][] datos;
  int[] visibles;

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
    visibles= new int[n_canales];
    for(int i=0;i<n_canales;i++)visibles[i]=1;
    
    int alto_canal=alto/n_canales;
    ancho_canal=ancho-10;
    for(int i=0;i<n;i++){
      canales.add(new Canal(cx,v.mrgS()+3+alto_canal/2+(alto_canal)*i,ancho-10, alto_canal, escala));
    }
    
    bf=new Buffer(n,w*MAX_DECIMANDO);
    
    
  
   }
   
   void set(int [] valores){
      bf.apunta(valores);
   }
 
  void reset(){
      bf.limpia();
   }
 
  void update() {
     for(int i=0;i<n_canales;i++){
        Canal chx = canales.get(i);
        if(visibles[i]==1)
          chx.pinta(bf.lee_canal(i,ancho,chx.decimando));
     }
//     bf.calcula_umbrales();

  } 
  void setEscala(float e) {
     for(int i=0;i<n_canales;i++){
        Canal chx = canales.get(i);
        chx.escala=e;
     }
  }
  void setDecimando(int e) {
     for(int i=0;i<n_canales;i++){
        Canal chx = canales.get(i);
        chx.decimando=e;
     }
  }
  
  void setVisible(int e, int vis){
     visibles[e]=vis;       
  }
  
  
    

} 