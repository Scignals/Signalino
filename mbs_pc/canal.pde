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

  void fondo() {
  //    v.pintaVector(new arr_int((new arr_int(x1)).mov_avg(3)).zscore(),escala);
        v.dibuja_fondo();
    //   v.pintaVector((new arr_int(x1)).mov_avg(3),escala);
 
}
  
  void xpinta(arr_int x1) {
      v.pintaVector(x1.mov_avg_r(3),escala);
  }
}  