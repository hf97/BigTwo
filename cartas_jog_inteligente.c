#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char MAO_CHAR[100];
typedef long long int MAO;
typedef struct state ESTADO;

struct state{
      MAO m;
      MAO ant;
      MAO_CHAR M;
      MAO_CHAR ANT;
      MAO_CHAR JOGADA;
      int passagens;
      char comand[8][100];
};

#define NAIPES		"DCHS"

#define VALORES		"3456789TJQKA2"

int indice(int naipe, int valor) {
	return naipe * 13 + valor;
}

long long int add_carta(long long int ESTADO, int naipe, int valor) {
	int idx = indice(naipe, valor);
	return ESTADO | ((long long int) 1 << idx);
}

long long int rem_carta(long long int ESTADO, int naipe, int valor) {
	int idx = indice(naipe, valor);
	return ESTADO & ~((long long int) 1 << idx);
}

int carta_existe(long long int ESTADO, int naipe, int valor) {
	int idx = indice(naipe, valor);
	return (ESTADO >> idx) & 1;
}

ESTADO inicia_ESTADO(){
   ESTADO e;
   int i,j;
   e.m=0;
   e.ant=0;
   e.passagens=0;
   for(i=0;i<100;i++){
      e.M[i]=' ';
      e.ANT[i]=' ';
      e.JOGADA[i]=' ';
      for(j=0;j<8;j++){
         e.comand[j][i]=' ';
      }
   }
   return e;
}

int contar_char(MAO_CHAR m){
   char a,b;
   int r,i;
   r=0;i=0;
   a=m[i]; 
   i++;
   b=a;
   a=m[i];
   i++;
   if((a!=' ')||(b!=' ')) r=2;
   while((a!=' ')||(b!=' ')){
      r++;
      b=a;
      a=m[i];
      i++;
   }
   return r;
}

int contar_mao(MAO_CHAR m){
   char a,b;
   int r,i;
   r=0;i=0;
   a=m[i];
   i++;
   b=a;
   a=m[i];
   i++;
   if((a!=' ')||(b!=' ')) r=2;
   while((a!=' ')||(b!=' ')){
      r++;
      b=a;
      a=m[i];
      i++;
   }
   return r;
}

int contar(MAO m){
        int c=0,n,v;
        for(n=0;n<4;n++){
           for(v=0;v<13;v++){
              if(carta_existe(m,n,v)) c++;
           }
        }
        return c;           
}

int carta_maior_valor(MAO m){
    int n,v;
    for(v=12;v>=0;v--){
       for(n=3;n>=0;n--){
          if(carta_existe(m,n,v)){
            return v;
          }
       }
    } 
    return v;
}

int carta_maior_naipe(MAO m){
    int n,v;
    for(n=3;n>=0;n--){
       for(v=12;v>=0;v--){
          if(carta_existe(m,n,v)) {
            return n;
            }
       }
    } 
    return n;
}

int carta_menor_valor(MAO m){
    int n,v;
    for(n=0;n<4;n++){
       for(v=0;v<13;v++){
          if(carta_existe(m,n,v)){
            return v;
          }
       }
    }
    return v;
}

int carta_menor_naipe(MAO m){
    int n,v;
    for(n=0;n<4;n++){
       for(v=0;v<13;v++){
          if(carta_existe(m,n,v)){
            return n;
          }
       }
    }    
    return n;
}

int flush (MAO m){
   int n,v;
   MAO aux=0;
   if(contar(m)!=5) return 0;
   for(n=0;n<4;n++){
      for(v=0;v<13;v++){
         if(((carta_existe(m,n,v))&&(contar(aux)<5))) aux=add_carta (aux,n,v);      
      }
      if(contar(aux)==5) break;
      else aux=0;
   }
   if(contar(aux)==5) return 1;
   return 0;
}

int four_of_a_kind (MAO m){
   int n,v;
   MAO aux=0;
   if(contar(m)!=5) return 0;
   for(v=0;v<13;v++){
      for(n=0;n<4;n++){
         if((carta_existe(m,n,v))&&(contar(aux)<4)) aux=add_carta(aux,n,v);
      }
      if(contar(aux)==4) break;
      else aux=0;
   }
   if((contar(aux)==4)&&(contar(m)==5)) return 1;
   else return 0;
}

int straight (MAO m) {
   int n,v;
   int vals[13]={0};
   if(contar(m)!=5) return 0;
   for(v=0;v<13;v++){
      for(n=0;n<4;n++){
         if(carta_existe(m,n,v)){
           vals[v]++;
           break;
         }
      }
   }
   if(vals[11] && vals[10] && vals[9] && vals[8] && vals[7]) {return 10;}
   if(vals[10] && vals[9] && vals[8] && vals[7] && vals[6]) {return 9;}
   if(vals[9] && vals[8] && vals[7] && vals[6] && vals[5]) {return 8;}
   if(vals[8] && vals[7] && vals[6] && vals[5] && vals[4]) {return 7;}
   if(vals[7] && vals[6] && vals[5] && vals[4] && vals[3]) {return 6;}
   if(vals[6] && vals[5] && vals[4] && vals[3] && vals[2]) {return 5;}
   if(vals[5] && vals[4] && vals[3] && vals[2] && vals[1]) {return 4;}
   if(vals[4] && vals[3] && vals[2] && vals[1] && vals[0]) {return 3;}
   if(vals[3] && vals[2] && vals[1] && vals[0] && vals[12]) {return 2;}
   if(vals[2] && vals[1] && vals[0] && vals[12] && vals[11]) {return 1;}
   return 0;
}

int full_house(MAO m){
   int n,v;
   MAO aux=0;
   MAO tmp=0;
   int s;
   if(contar(m)!=5) return 0;
   for(v=0;v<13;v++){
      s=v;
      for(n=0;n<4;n++){
         if(((carta_existe(m,n,v))&&(contar(aux)<3))) aux=add_carta(aux,n,v);
      }
      if(contar(aux)==3) break;
      else aux=0;
   }
   if(contar(aux)==3){
      for(v=0;v<13;v++){
         for(n=0;n<4;n++){
            if((carta_existe(m,n,v))&&(contar(tmp)<2)&&(v!=s)) tmp=add_carta(tmp,n,v);
         }
      if(contar(tmp)==2) break;
      else tmp=0;
      }
   }
   if((contar(aux)==3)&&(contar(tmp)==2)){
     for(v=0;v<13;v++){
        for(n=0;n<4;n++){
           if(carta_existe(tmp,n,v)) aux=add_carta(aux,n,v);
        }
     }
   }else{
     aux=0;
     tmp=0;
     s=0;
     for(v=0;v<13;v++){
        s=v;
        for(n=0;n<4;n++){
           if((carta_existe(m,n,v))&&(contar(aux)<2)) aux=add_carta(aux,n,v);
        }
        if(contar(aux)==2) break;
        else aux=0;
     }
     if(contar(aux)==2){
       for(v=0;v<13;v++){
          for(n=0;n<4;n++){
             if((carta_existe(m,n,v))&&(contar(tmp)<3)&&(v!=s)) tmp=add_carta(tmp,n,v);
          }
          if(contar(tmp)==3) break;
          else tmp=0;
       }
       if((contar(aux)==2)&&(contar(tmp)==3)){
         for(v=0;v<13;v++){
            for(n=0;n<4;n++){
               if(carta_existe(tmp,n,v)) aux=add_carta(aux,n,v);
            }
         }
       }
     }
   }
   if(contar(aux)==5) return 1;
   return 0;
}

int straight_flush(MAO m){
   return((flush(m))&&(straight(m)!=0));
}

int compara_naipe(MAO m1, MAO m2, int m){
   int n,n1=0,n2=0;
   for(n=0;n<4;n++){
      if(carta_existe(m1,n,m)) n1=n;
      if(carta_existe(m2,n,m)) n2=n;
   }
   return (n1>n2);
}

int compara_four_of_a_kind(MAO m1, MAO m2){
   int n,v;
   int vals1[13]={0},vals2[13]={0};
   int vm1=0,vm2=0;
   for(v=0;v<13;v++){
      for(n=0;n<4;n++){
         if(carta_existe(m1,n,v)) vals1[v]++;
         if(carta_existe(m2,n,v)) vals2[v]++;
      }
      if(vals1[v]==4) vm1=v;
      if(vals2[v]==4) vm2=v;
   }
   return(vm1>vm2);
}

int compara_full_house(MAO m1, MAO m2){
   int n,v;
   int vals1[13]={0},vals2[13]={0};
   int vm1=0,vm2=0;
   for(v=0;v<13;v++){
      for(n=0;n<4;n++){
         if(carta_existe(m1,n,v)) vals1[v]++;
         if(carta_existe(m2,n,v)) vals2[v]++;
      }
      if(vals1[v]==3) vm1=v;
      if(vals2[v]==3) vm2=v;
   }
   return(vm1>vm2);
}

int compara_flush(MAO m1, MAO m2){
   int n,n1=0,n2=0;
   int v;
   if(carta_maior_valor(m1)>carta_maior_valor(m2)) return 1;
   if(carta_maior_valor(m1)==carta_maior_valor(m2)){
     for(n=0;n<4;n++){
        v=carta_maior_valor(m1);
        if(carta_existe(m1,n,v)) n1=n;
        if(carta_existe(m2,n,v)) n2=n;
     }
     return (n1>n2);
   }
   return 0;
}

int compara_straight(MAO m1, MAO m2){
   int x,y;
   x=straight(m1);
   y=straight(m2);
   if(x>y) return 1;
   else{
     if(x==y){
        if(x==1){
          return(compara_naipe(m1,m2,2));
        }
        if(x==2){
          return(compara_naipe(m1,m2,3));
        }
        if(x==3){
          return(compara_naipe(m1,m2,4));
        }
        if(x==4){
          return(compara_naipe(m1,m2,5));
        }
        if(x==5){
          return(compara_naipe(m1,m2,6));
        }
        if(x==6){
          return(compara_naipe(m1,m2,7));
        }
        if(x==7){
          return(compara_naipe(m1,m2,8));
        }
        if(x==8){
          return(compara_naipe(m1,m2,9));
        }
        if(x==9){
          return(compara_naipe(m1,m2,10));
        }
        if(x==10){
          return(compara_naipe(m1,m2,11));
        }
     }
   }
   return 0;
}

int compara_straight_flush(MAO m1, MAO m2){
   int x,y;
   int n1,n2;
   x=straight(m1);
   y=straight(m2);
   n1=carta_maior_naipe(m1);
   n2=carta_maior_naipe(m2);
   if(x>y) return 1;
   else{
     if(x==y){
       if(n1>n2) return 1;
     }
   }
   return 0;
}

int tipo_comb_5(MAO m){
   if(straight_flush(m)) return 5;
   if(four_of_a_kind(m)) return 4;
   if(full_house(m)) return 3;
   if(flush(m)) return 2;
   if(straight(m)) return 1;   
   return 0;
}

int mao_valor_igual(MAO m){
    int n,v;
    for(n=3;n>=0;n--){
       for(v=0;v<13;v++){
          if((carta_existe(m,n,v))&&(v!=carta_maior_valor(m))){
            return 0;
          }
       }
    }
    return 1;
} 

int maior_comb(MAO s, MAO uj){
    int t=tipo_comb_5(s);
    int u=tipo_comb_5(uj);
    if(contar(uj)<5){
      if(contar(s)==contar(uj)){
        if((carta_maior_valor(s))>(carta_maior_valor(uj))){
          return 1;
        }else{
          if((carta_maior_valor(s))==(carta_maior_valor(uj))){
            if((carta_maior_naipe(s))>(carta_maior_naipe(uj))){
              return 1;
            }
          }
        } 
      }
    }else{
      if(contar(uj)==5){
        if(contar(s)==contar(uj)){
          if(t==u){
            if(t==5){
              return (compara_straight_flush(s,uj));
            }
            if(t==4){
              return (compara_four_of_a_kind(s,uj));
            }
            if(t==3){
              return (compara_full_house(s,uj));
            }
            if(t==2){
              return (compara_flush(s,uj));
            }
            if(t==1){
              return (compara_straight(s,uj));
            }
          }else{
            if(t>u) return 1;
            if(t<u) return 0;
          }
        }
      } 
    }
    return 0;
}

int comb_val(MAO m){
    int t=tipo_comb_5(m);
    if(m==0) return 0;
    if(contar(m)==1) return 1;
    if((contar(m)==2)||(contar(m)==3)){
      if(mao_valor_igual(m)){
        return 1;
      }
    }
    if(contar(m)==5){
      return (t!=0);
    }
    return 0;
}

MAO cria_pares(MAO m){
   int n,v;
   MAO aux;
   for(v=0;v<13;v++){
      for(n=0;n<4;n++){
         if((carta_existe(m,n,v))&&(contar(aux)<2)) aux=add_carta(aux,n,v);
      }
      if(contar(aux)==2) return aux;
      else aux=0;
   }
   return 0;
}

MAO cria_trios(MAO m){
   int n,v;
   MAO aux=0;
   for(v=0;v<13;v++){
      for(n=0;n<4;n++){
         if((carta_existe(m,n,v))&&(contar(aux)<3)) aux=add_carta(aux,n,v);
      }
      if(contar(aux)==3) return aux;
      else aux=0;
   }
   return 0;
}

MAO cria_four_of_a_kind(MAO m){
   int n,v,s;
   MAO aux=0;
   for(v=0;v<13;v++){
      s=v;
      for(n=0;n<4;n++){
         if((carta_existe(m,n,v))&&(contar(aux)<4)) aux=add_carta(aux,n,v);
      }
      if(contar(aux)==4) break;
      else aux=0;
   }
   if(contar(aux)==4){
     for(v=0;v<13;v++){
        for(n=0;n<4;n++){
           if((carta_existe(m,n,v))&&(v!=s)){
             aux=add_carta(aux,n,v);
           }
        }
     }
   }
   if(four_of_a_kind(aux)) return aux;
   else return 0;
}

MAO cria_full_house(MAO m){
   int n,v;
   MAO aux=0;
   MAO tmp=0;
   int s;
   for(v=0;v<13;v++){
      s=v;
      for(n=0;n<4;n++){
         if(((carta_existe(m,n,v))&&(contar(aux)<3))) aux=add_carta(aux,n,v);
      }
      if(contar(aux)==3) break;
      else aux=0;
   }
   if(contar(aux)==3){
     for(v=0;v<13;v++){
        for(n=0;n<4;n++){
           if(((carta_existe(m,n,v))&&(contar(aux)<2)&&(v!=s))) tmp=add_carta(tmp,n,v);
        }
        if(contar(tmp)==2) break;
        else tmp=0;
     }
   }
   if((contar(aux)==3)&&(contar(tmp)==2)){
     for(v=0;v<13;v++){
        for(n=0;n<4;n++){
           if(carta_existe(tmp,n,v)) aux=add_carta(aux,n,v);
        }
     }
   }else{
     aux=0;
     tmp=0;
     s=0;
     for(v=0;v<13;v++){
        s=v;
        for(n=0;n<4;n++){
           if(((carta_existe(m,n,v))&&(contar(aux)<2))) aux=add_carta(aux,n,v);
        }
        if(contar(aux)==2) break;
        else aux=0;
     }
     if(contar(aux)==2){
       for(v=0;v<13;v++){
          for(n=0;n<4;n++){
             if(((carta_existe(m,n,v))&&(contar(aux)<3)&&(v!=s))) tmp=add_carta(tmp,n,v);
          }
          if(contar(tmp)==3) break;
          else tmp=0;
       }
     }
     if((contar(aux)==2)&&(contar(tmp)==3)){
       for(v=0;v<13;v++){
          for(n=0;n<4;n++){
             if(carta_existe(tmp,n,v)) aux=add_carta(aux,n,v);
          }
       }
     }
   }
   if(full_house(aux)) return aux;
   else return 0;
}

MAO cria_flush(MAO m){
   int n,v;
   MAO aux=0;
   for(n=0;n<4;n++){
      for(v=0;v<13;v++){
         if(((carta_existe(m,n,v))&&(contar(aux)<5))) aux=add_carta(aux,n,v);
      }
      if(flush(aux)) break;
      else aux=0;
   }
   if(flush(aux)) return aux;
   else return 0;
}

MAO cria_straight(MAO m){
   int n,v,s=0;
   MAO aux=0;
   int vals[13]={0};
   for(v=0;v<13;v++){
      for(n=0;n<4;n++){
         if(carta_existe(m,n,v)){
           vals[v]++;
           break;
         }
      }
   }
   if((vals[2])&&(vals[1])&&(vals[0])&&(vals[12])&&(vals[11])){
     for(v=11;v<13;v++){
        for(n=0;n<4;n++){
           if((carta_existe(m,n,v))&&(contar(aux)<2)){
             aux=add_carta(aux,n,v);
             break;
           }
        }
     }
     if(contar(aux)==2){
       for(v=0;v<3;v++){
          for(n=0;n<4;n++){
             if((carta_existe(m,n,v))&&(contar(aux)<5)){
               aux=add_carta(aux,n,v);
               break;
             }
          }
          if(straight(aux)) break; 
       }
     }     
   }else{
     if((vals[3])&&(vals[2])&&(vals[1])&&(vals[0])&&(vals[12])){
       for(n=0;n<4;n++){
          if(carta_existe(m,n,12)){
            aux=add_carta(aux,n,12);
            break;
          }
       }
       for(v=0;v<4;v++){
          for(n=0;n<4;n++){
             if((carta_existe(m,n,v))&&(contar(aux)<5)){
               aux=add_carta(aux,n,v);
               break;
             }
          }
          if(straight(aux)) break;
       }
     }else{
       for(v=0;v<13;v++){
          for(n=0;n<4;n++){
             if(((carta_existe(m,n,v))&&(contar(aux)<5))&&(s=v-1)){ 
               aux=add_carta(aux,n,v);
               break;
             }else{
               aux=0;
               if(carta_existe(m,n,v)){
                 aux=add_carta(aux,n,v);
                 break;
               }
              }
          }
          s=v;
          if(straight(aux)) break;
       }
     }
   }
   if(straight(aux)) return aux;
   else return 0;
}

MAO cria_straight_flush(MAO m){
   int n,v,s=0;
   MAO aux=0;
   int vals[13]={0};
   for(v=0;v<13;v++){
      for(n=0;n<4;n++){
         if(carta_existe(m,n,v)){
           vals[v]++;
           break;
         }
      }
   }
   if((vals[2])&&(vals[1])&&(vals[0])&&(vals[12])&&(vals[11])){
     for(n=0;n<4;n++){
        for(v=11;v<13;v++){
           if((carta_existe(m,n,v))&&(contar(aux)<2)) aux=add_carta(aux,n,v);
        }
        if(contar(aux)==2){
          for(v=0;v<3;v++){
             if((carta_existe(m,n,v))&&(contar(aux)<5)) aux=add_carta(aux,n,v);
          }
        }
        if(straight_flush(aux)) break;
        else aux=0;
     }
   }else{
     if((vals[3])&&(vals[2])&&(vals[1])&&(vals[0])&&(vals[12])){
       for(n=0;n<4;n++){
          if(carta_existe(m,n,12)) aux=add_carta(aux,n,12);
          if(contar(aux)==1){
            for(v=0;v<4;v++){
               if((carta_existe(m,n,v))&&(contar(aux)<5)) aux=add_carta(aux,n,v);
            }
          }
          if(straight_flush(aux)) break;
          else aux=0;
       }
     }else{
       for(n=0;n<4;n++){
          for(v=0;v<13;v++){
             if(((carta_existe(m,n,v))&&(contar(aux)<5))&&(s=v-1)) aux=add_carta(aux,n,v);
             else{
               aux=0;
               if(carta_existe(m,n,v)) aux=add_carta(aux,n,v);
             }
             s=v;
             if(straight_flush(aux)) break;
          }
          if(straight_flush(aux)) break;
          else aux=0;
       }
     }
   }
   if(straight_flush(aux)) return aux;
   else return 0;
}

MAO cria_pares_inicio(MAO m){
   int n;
   MAO aux=0;
   for(n=0;n<4;n++){
      if((carta_existe(m,n,0))&&(contar(aux)<2)) aux=add_carta(aux,n,0);
   }
   if(contar(aux)==2) return aux;
   else return 0;
}

MAO cria_trios_inicio(MAO m){
   int n;
   MAO aux=0;
   for(n=0;n<4;n++){
      if((carta_existe(m,n,0))&&(contar(aux)<3)) aux=add_carta(aux,n,0);
   }
   if(contar(aux)==3) return aux;
   else return 0;
}

MAO cria_four_of_a_kind_inicio(MAO m){
   int n,v,s;
   MAO aux=0;
   MAO tmp=0;
   for(v=0;v<13;v++){
      s=v;
      for(n=0;n<4;n++){
         if((carta_existe(m,n,v))&&(contar(aux)<4)) aux=add_carta(aux,n,v);
      }
      if(contar(aux)==4) break;
      else aux=0;
   }
   if(s==0){
     if(contar(aux)==4){
       for(v=0;v<13;v++){
          for(n=0;n<4;n++){
             if((carta_existe(m,n,v))&&(contar(tmp)<1)&&(v!=0)) tmp=add_carta(tmp,n,v);
          }
          if(contar(tmp)==1) break;
          else tmp=0;
       }
     } 
   }else{
     if(contar(aux)==4){
       if((carta_existe(m,0,0))&&(contar(tmp)<1)) tmp=add_carta(tmp,0,0);
     }
   }
   if((contar(aux)==4)&&(contar(tmp)==1)){
     for(v=0;v<13;v++){
        for(n=0;n<4;n++){
           if(carta_existe(tmp,n,v)) aux=add_carta(aux,n,v);
        }
     }
   }else{
     aux=0;
     tmp=0;
     s=0;
     for(v=0;v<13;v++){
        s=v;
        for(n=0;n<4;n++){
           if((carta_existe(m,n,v))&&(contar(aux)<1)) aux=add_carta(aux,n,v);
        }
        if(contar(aux)==1) break;
        else aux=0;
     }
     if(s==0){
       if(contar(aux)==1){
         for(v=0;v<13;v++){
            for(n=0;n<4;n++){
               if((carta_existe(m,n,v))&&(contar(tmp)<4)&&(v!=0)) tmp=add_carta(tmp,n,v);
            }
            if(contar(tmp)==4) break;
            else tmp=0;
         }
       } 
     }else{
       if(contar(aux)==1){
         for(v=0;v<13;v++){
            for(n=0;n<4;n++){
               if((carta_existe(m,n,v))&&(contar(tmp)<4)&&(v!=s)) tmp=add_carta(tmp,n,v);
            }
            if(contar(tmp)==4) break;
            else tmp=0;
         }
       }
     }
     if((contar(aux)==1)&&(contar(tmp)==4)){
       for(v=0;v<13;v++){
          for(n=0;n<4;n++){
             if(carta_existe(tmp,n,v)) aux=add_carta(aux,n,v);
          }
       }
     }
   }
   if(four_of_a_kind(aux)) return aux;
   else return 0;
}

MAO cria_full_house_inicio(MAO m){
   int n,v;
   MAO aux=0;
   MAO tmp=0;
   int s;
   for(v=0;v<13;v++){
      s=v;
      for(n=0;n<4;n++){
         if(((carta_existe(m,n,v))&&(contar(aux)<3))) aux=add_carta(aux,n,v);
      }
      if(contar(aux)==3) break;
      else aux=0;
   }
   if(s==0){
     if(contar(aux)==3){
       for(v=0;v<13;v++){
          for(n=0;n<4;n++){
             if(((carta_existe(m,n,v))&&(contar(tmp)<2)&&(v!=0))) tmp=add_carta(tmp,n,v);
          }
          if(contar(tmp)==2) break;
          else tmp=0;
       }
     }
   }else{
     if(contar(aux)==3){
       for(n=0;n<4;n++){
          if((carta_existe(m,n,0))&&(contar(tmp)<2)) tmp=add_carta(tmp,n,0);
       }
     }
   }
   if((contar(aux)==3)&&(contar(tmp)==2)){
     for(v=0;v<13;v++){
        for(n=0;n<4;n++){
           if(carta_existe(tmp,n,v)) aux=add_carta(aux,n,v);
        }
     }
   }else{
     aux=0;
     tmp=0;
     s=0;
     for(v=0;v<13;v++){
        s=v;
        for(n=0;n<4;n++){
           if(((carta_existe(m,n,v))&&(contar(aux)<2))) aux=add_carta(aux,n,v);
        }
        if(contar(aux)==2) break;
        else aux=0;
     }
     if(s==0){
       if(contar(aux)==2){
         for(v=0;v<13;v++){
            for(n=0;n<4;n++){
               if(((carta_existe(m,n,v))&&(contar(tmp)<3)&&(v!=0))) tmp=add_carta(tmp,n,v);
            }
            if(contar(tmp)==3) break;
            else tmp=0;
         }
       }
     }else{
       if(contar(aux)==2){
         for(n=0;n<4;n++){
            if((carta_existe(m,n,0))&&(contar(tmp)<3)) tmp=add_carta(tmp,n,0);
         }
       }
     }
     if((contar(aux)==2)&&(contar(tmp)==3)){
       for(v=0;v<13;v++){
          for(n=0;n<4;n++){
             if(carta_existe(tmp,n,v)) aux=add_carta(aux,n,v);
          }
       }
     }
   }
   if(full_house(aux)) return aux;
   else return 0;
}

MAO cria_flush_inicio(MAO m){
   int v;
   MAO aux=0;
   for(v=0;v<13;v++){
      if(((carta_existe(m,0,v))&&(contar(aux)<5))) aux=add_carta(aux,0,v);
   }
   if(flush(aux)) return aux;
   else return 0;
}

MAO cria_straight_inicio(MAO m){
   int n,v;
   MAO aux=0;
   int vals[13]={0};
   for(v=0;v<13;v++){
      for(n=0;n<4;n++){
         if(carta_existe(m,n,v)){
           vals[v]++;
           break;
         }
      }
   }
   if((vals[2])&&(vals[1])&&(vals[0])&&(vals[12])&&(vals[11])){
     for(v=11;v<13;v++){
        for(n=0;n<4;n++){
           if((carta_existe(m,n,v))&&(contar(aux)<2)){
             aux=add_carta(aux,n,v);
             break;
           }
        }
     }
     if(contar(aux)==2){
       for(v=0;v<3;v++){
          for(n=0;n<4;n++){
             if((carta_existe(m,n,v))&&(contar(aux)<5)){
               aux=add_carta(aux,n,v);
               break;
             }
          }
          if(straight(aux)) break;
          else aux=0; 
       }
     }     
   }else{
     if((vals[3])&&(vals[2])&&(vals[1])&&(vals[0])&&(vals[12])){
       for(n=0;n<4;n++){
          if(carta_existe(m,n,12)){
            aux=add_carta(aux,n,12);
            break;
          }
       }
       for(v=0;v<4;v++){
          for(n=0;n<4;n++){
             if((carta_existe(m,n,v))&&(contar(aux)<5)){
               aux=add_carta(aux,n,v);
               break;
             }
          }
          if(straight(aux)) break;
          else aux=0;
       }
     }else{
       if((vals[4])&&(vals[3])&&(vals[2])&&(vals[1])&&(vals[0])){
         for(v=0;v<5;v++){
            for(n=0;n<4;n++){
               if((carta_existe(m,n,v))&&(contar(aux)<5)){
                 aux=add_carta(aux,n,v);
                 break;
               }
            }
            if(straight(aux)) break;
         }
       }
     }
   }
   if(straight(aux)) return aux;
   else return 0;
}

MAO cria_straight_flush_inicio(MAO m){
   int n,v;
   MAO aux=0;
   int vals[13]={0};
   for(v=0;v<13;v++){
      for(n=0;n<4;n++){
         if(carta_existe(m,n,v)){
           vals[v]++;
           break;
         }
      }
   }
   if((vals[2])&&(vals[1])&&(vals[0])&&(vals[12])&&(vals[11])){
     for(n=0;n<4;n++){
        for(v=11;v<13;v++){
           if((carta_existe(m,n,v))&&(contar(aux)<2)) aux=add_carta(aux,n,v);
        }
        if(contar(aux)==2){
          for(v=0;v<3;v++){
             if((carta_existe(m,n,v))&&(contar(aux)<5)) aux=add_carta(aux,n,v);
          }
        }
        if(straight_flush(aux)) break;
        else aux=0;
     }
   }else{
     if((vals[3])&&(vals[2])&&(vals[1])&&(vals[0])&&(vals[12])){
       for(n=0;n<4;n++){
          if(carta_existe(m,n,12)) aux=add_carta(aux,n,12);
          if(contar(aux)==1){
            for(v=0;v<4;v++){
               if((carta_existe(m,n,v))&&(contar(aux)<5)) aux=add_carta(aux,n,v);
            }
          }
          if(straight_flush(aux)) break;
          else aux=0;
       }
     }else{
       if((vals[4])&&(vals[3])&&(vals[2])&&(vals[1])&&(vals[0])){
         for(n=0;n<4;n++){
            for(v=0;v<5;v++){
               if((carta_existe(m,n,v))&&(contar(aux)<5)) aux=add_carta(aux,n,v);
            }
            if(straight_flush(aux)) break;
            else aux=0;
         }
       }
     }
   }
   if(straight_flush(aux)) return aux;
   else return 0;
}

MAO reage_a_four_of_a_kind(MAO m,MAO a,int t){
   int n,v,s,p;
   MAO aux=0;
   for(p=0;p<13;p++){ 
      for(v=0;v<13;v++){
         s=v;
         for(n=0;n<4;n++){
            if(carta_existe(m,n,v)) aux=add_carta(aux,n,v);
         }
         if(contar(aux)==4) break;
         else aux=0;
      }
      if(contar(aux)==4){
        for(v=0;v<13;v++){
           for(n=0;n<4;n++){
              if((carta_existe(m,n,v))&&(v!=s)){
                aux=add_carta(aux,n,v);
              }
           }
        }
      }
      if((four_of_a_kind(aux))&&((compara_four_of_a_kind(aux,a))||(t<4))) break;
      else aux=0;
   }
   if((four_of_a_kind(aux))&&((compara_four_of_a_kind(aux,a))||(t<4))) return aux;
   else return 0;
}

MAO reage_a_full_house(MAO m,MAO a,int t){
   int n,v,p;
   MAO aux=0;
   MAO tmp=0;
   int s;
   for(p=0;p<13;p++){
      for(v=0;v<13;v++){
         s=v;
         for(n=0;n<4;n++){
            if(((carta_existe(m,n,v))&&(contar(aux)<3))) aux=add_carta(aux,n,v);
         }
         if(contar(aux)==3) break;
         else aux=0;
      }
      if(contar(aux)==3){
        for(v=0;v<13;v++){
           for(n=0;n<4;n++){
              if(((carta_existe(m,n,v))&&(contar(tmp)<2)&&(v!=s))) tmp=add_carta(tmp,n,v);
           }
           if(contar(tmp)==2) break;
           else tmp = 0;
        }
      }
      if((contar(aux)==3)&&(contar(tmp)==2)){
        for(v=0;v<13;v++){
           for(n=0;n<4;n++){
              if(carta_existe(tmp,n,v)) aux=add_carta(aux,n,v);
           }
        }
        if((full_house(aux))&&((compara_full_house(aux,a))||(t<3))) break;
        else{
          aux=0;
          tmp=0;
          s=0;
        }
      }else{
        aux=0;
        tmp=0;
        s=0;
        for(v=0;v<13;v++){
           s=v;
           for(n=0;n<4;n++){
              if(((carta_existe(m,n,v))&&(contar(aux)<2))) aux=add_carta(aux,n,v);
           }
           if(contar(aux)==2) break;
           else aux=0;
        }
        if(contar(aux)==2){
          for(v=0;v<13;v++){
             for(n=0;n<4;n++){
                if(((carta_existe(m,n,v))&&(contar(tmp)<3)&&(v!=s))) tmp=add_carta(tmp,n,v);
             }
             if(contar(tmp)==3) break;
             else tmp=0;
          }
        }
        if((contar(aux)==2)&&(contar(tmp)==3)){
          for(v=0;v<13;v++){
             for(n=0;n<4;n++){
                if(carta_existe(tmp,n,v)) aux=add_carta(aux,n,v);
             }
          }
        }
      }
      if((full_house(aux))&&((compara_full_house(aux,a))||(t<3))) break;
      else{
        aux=0;
        tmp=0;
        s=0;
      }
   }
   if((full_house(aux))&&((compara_full_house(aux,a))||(t<3))) return aux;
   else return 0;
}

MAO reage_a_flush(MAO m,MAO a,int t){
   int n,v;
   MAO aux=0;
   for(n=0;n<4;n++){
      for(v=0;v<13;v++){
         if(((carta_existe(m,n,v))&&(contar(aux)<5))) aux=add_carta(aux,n,v);
      }
      if((flush(aux))&&((compara_flush(aux,a))||(t<2))) break;
      else aux=0;
   }
   if((flush(aux))&&((compara_flush(aux,a))||(t<2))) return aux;
   else return 0;
}

MAO reage_a_straight(MAO m,MAO a,int t){
  int n,v,s=0;
   MAO aux=0;
   int vals[13]={0};
   for(v=0;v<13;v++){
      for(n=0;n<4;n++){
         if(carta_existe(m,n,v)){
           vals[v]++;
           break;
         }
      }
   }
   if((vals[2])&&(vals[1])&&(vals[0])&&(vals[12])&&(vals[11])){
     for(v=11;v<13;v++){
        for(n=0;n<4;n++){
           if((carta_existe(m,n,v))&&(contar(aux)<2)){
             aux=add_carta(aux,n,v);
             break;
           }
        }
     }
     if(contar(aux)==2){
       for(v=0;v<3;v++){
          for(n=0;n<4;n++){
             if((carta_existe(m,n,v))&&(contar(aux)<5)){
               aux=add_carta(aux,n,v);
               break;
             }
          }
          if((straight(aux))&&((compara_straight(aux,a))||(t<1))) break;
       }
     }     
   }
   if((!straight(aux))||(!compara_straight(aux,a))){
     aux=0;
     if((vals[3])&&(vals[2])&&(vals[1])&&(vals[0])&&(vals[12])){
       for(n=0;n<4;n++){
          if(carta_existe(m,n,12)){        
            aux=add_carta(aux,n,12);
            break;
          }
       }
       for(v=0;v<4;v++){
          for(n=0;n<4;n++){
             if((carta_existe(m,n,v))&&(contar(aux)<5)){
               aux=add_carta(aux,n,v);
               break;
             }
          }
          if((straight(aux))&&((compara_straight(aux,a))||(t<1))) break;
       }
     }
   }
   if((!straight(aux))||(!compara_straight(aux,a))){
     aux=0;
     for(v=0;v<13;v++){
        for(n=0;n<4;n++){
           if(((carta_existe(m,n,v))&&(contar(aux)<5))&&(s=v-1)){ 
             aux=add_carta(aux,n,v);
             break;
           }else{
             aux=0;
             if(carta_existe(m,n,v)){
               aux=add_carta(aux,n,v);
               break;
             }
            }
        }
        s=v;
        if((straight(aux))&&((compara_straight(aux,a))||(t<1))) break;
     }     
   }
   if((straight(aux))&&(compara_straight(aux,a))) return aux;
   else return 0;
}

MAO reage_a_straight_flush(MAO m,MAO a,int t){
   int n,v,s=0;
   MAO aux=0;
   int vals[13]={0};
   for(v=0;v<13;v++){
      for(n=0;n<4;n++){
         if(carta_existe(m,n,v)){
           vals[v]++;
           break;
         }
      }
   }
   if((vals[2])&&(vals[1])&&(vals[0])&&(vals[12])&&(vals[11])){
     for(n=0;n<4;n++){
        for(v=11;v<13;v++){
           if((carta_existe(m,n,v))&&(contar(aux)<2)) aux=add_carta(aux,n,v);
        }
        if(contar(aux)==2){
          for(v=0;v<3;v++){
             if((carta_existe(m,n,v))&&(contar(aux)<5)) aux=add_carta(aux,n,v);
          }
        }
        if((straight_flush(aux))&&((compara_straight_flush(aux,a))||(t<5))) break;
        else aux=0;
     }
   }
   if((!straight_flush(aux))||(!compara_straight_flush(aux,a))){
     aux=0;
     if((vals[3])&&(vals[2])&&(vals[1])&&(vals[0])&&(vals[12])){
       for(n=0;n<4;n++){
          if(carta_existe(m,n,12)) aux=add_carta(aux,n,12);
          if(contar(aux)==1){
            for(v=0;v<4;v++){
               if((carta_existe(m,n,v))&&(contar(aux)<5)) aux=add_carta(aux,n,v);
            }
          }
          if((straight_flush(aux))&&((compara_straight_flush(aux,a))||(t<5))) break;
          else aux=0;
       }
     }
   }
   if((!straight_flush(aux))||(!compara_straight_flush(aux,a))){
     aux=0;
     for(n=0;n<4;n++){
        for(v=0;v<13;v++){
           if(((carta_existe(m,n,v))&&(contar(aux)<5))&&(s=v-1)) aux=add_carta(aux,n,v);
           else{
             aux=0;
             if(carta_existe(m,n,v)) aux=add_carta(aux,n,v);
           }
           s=v;
           if((straight_flush(aux))&&((compara_straight_flush(aux,a))||(t<5))) break;
        }
        if((straight_flush(aux))&&((compara_straight_flush(aux,a))||(t<5))) break;
        else aux=0;
     }
   }
   if((straight_flush(aux))&&((compara_straight_flush(aux,a))||(t<5))) return aux;
   else return 0;
}

MAO reage_a_5_cartas(ESTADO e,int t){
   MAO a=e.ant;
   MAO m=e.m;
   MAO aux=0;
   if(t==5){
      aux=reage_a_straight_flush(m,a,t);      
    }
    if(t==4){
      aux=reage_a_straight_flush(m,a,t);
      if(aux==0) aux=reage_a_four_of_a_kind(m,a,t);
    }
    if(t==3){
      aux=reage_a_straight_flush(m,a,t);
      if(aux==0) aux=reage_a_four_of_a_kind(m,a,t);
      if(aux==0) aux=reage_a_full_house(m,a,t);
    }
    if(t==2){
      aux=reage_a_straight_flush(m,a,t);
      if(aux==0) aux=reage_a_four_of_a_kind(m,a,t);
      if(aux==0) aux=reage_a_full_house(m,a,t);
      if(aux==0) aux=reage_a_flush(m,a,t);
    }
    if(t==1){
      aux=reage_a_straight_flush(m,a,t);
      if(aux==0) aux=reage_a_four_of_a_kind(m,a,t);
      if(aux==0) aux=reage_a_full_house(m,a,t);
      if(aux==0) aux=reage_a_flush(m,a,t);
      if(aux==0) aux=reage_a_straight(m,a,t);
    }
    return aux;
}

MAO melhor_mao(MAO x){
    MAO aux=0;
    int n,v;
    aux=cria_straight_flush(x);
    if(aux==0) aux=cria_four_of_a_kind(x);
    if(aux==0) aux=cria_full_house(x);    
    if(aux==0) aux=cria_flush(x);
    if(aux==0) aux=cria_straight(x);
    if(aux==0) aux=cria_trios(x);
    if(aux==0) aux=cria_pares(x);
    if(aux==0){
      for(v=0;v<13;v++){
         for(n=0;n<4;n++){
            if((carta_existe(x,n,v))&&(contar(aux)<1)) aux=add_carta(aux,n,v);
         }
      }
    }
    return aux;
}

MAO melhor_mao_inicio(MAO x){
   MAO aux=0;
    int n,v;
    aux=cria_straight_flush(x);
    if(!carta_existe(aux,0,0)) aux=cria_four_of_a_kind_inicio(x);
    if(!carta_existe(aux,0,0)) aux=cria_full_house_inicio(x);    
    if(!carta_existe(aux,0,0)) aux=cria_flush_inicio(x);
    if(!carta_existe(aux,0,0)) aux=cria_straight_inicio(x);
    if(!carta_existe(aux,0,0)) aux=cria_trios_inicio(x);
    if(!carta_existe(aux,0,0)) aux=cria_pares_inicio(x);
    if(!carta_existe(aux,0,0)){
      for(v=0;v<13;v++){
         for(n=0;n<4;n++){
            if((carta_existe(x,n,v))&&(contar(aux)<1)) aux=add_carta(aux,n,v);
         }
      }
    }
    if(carta_existe(aux,0,0)) return aux;
    else return 0;
}

MAO melhor_mao_possivel(ESTADO e){
    MAO aux=0;
    MAO tmp;
    MAO m=e.ant;
    int c,n,v;
    int k,w;
    int t=tipo_comb_5(m);
    c=contar(m);
    n=carta_maior_naipe(m);
    v=carta_maior_valor(m);
    if(c==1){
      for(k=v;k<13;k++){
         for(w=n;w<4;w++){
            if(carta_existe(e.m,w,k)){
              aux=add_carta(aux,w,k);
              break;
            }            
         }
         if(contar(aux)==1) return aux;
         n=0;
      }
    }else{
      if(c==2){
        for(k=v;k<13;k++){
           tmp=0;
           for(w=n;w<4;w++){
              if((carta_existe(e.m,w,k))&&(contar(tmp)<2)){
                tmp=add_carta(tmp,w,k);
              }
           }
           if((contar(tmp)==2)&&(mao_valor_igual(tmp))){
             aux=tmp;
             return aux;
           }
           n=0;
        }
      }else{
        if(c==3){
          for(k=v;k<13;k++){
             tmp=0;
             for(w=n;w<4;w++){
                if((carta_existe(e.m,w,k))&&(contar(tmp)<3)&&(mao_valor_igual(tmp))){
                  tmp=add_carta(tmp,w,k);
                }
             }
             if(contar(tmp)==3){
               aux=tmp;
               return aux;
            }
            n=0;
          }
        }else{
          if(c==5){
            aux=reage_a_5_cartas(e,t);
            if((tipo_comb_5(aux))!=0) return aux;
          }
        }
      }
    }
    return 0;
}



MAO letras_a_numeros(MAO_CHAR m){
   char a,b;
   int n,v;
   int i,c,w;
   MAO res;
   n=0;v=0;
   res=0;
   w=0;
   c=contar_mao(m);
   a=m[w];
   w++;
   b=a;
   a=m[w];
   w++;
   for(i=0;i<c;i++){
     if((a!=' ')&&(b!=' ')){
       if(a=='D') n=0;
       if(a=='C') n=1;
       if(a=='H') n=2;
       if(a=='S') n=3;
       if(b=='3') v=0; 
       if(b=='4') v=1;
       if(b=='5') v=2;
       if(b=='6') v=3;
       if(b=='7') v=4;
       if(b=='8') v=5;
       if(b=='9') v=6;
       if(b=='T') v=7;
       if(b=='J') v=8;
       if(b=='Q') v=9;
       if(b=='K') v=10;
       if(b=='A') v=11;
       if(b=='2') v=12; 
       res=add_carta(res,n,v);
     }
     b=a;
     a=m[w];
     w++;
   }
   return res;
}

char numeros_a_letras(MAO m){
   int n,v,i,p;
   char a,b;
   char res[100];
   for(p=0;p<100;p++){
      res[p]=' ';
   }
   i=0;
   for(n=0;n<4;n++){
      for(v=0;v<13;v++){
         if(carta_existe(m,n,v)){
           if(n==0) a='D';
           if(n==1) a='C';
           if(n==2) a='H';
           if(n==3) a='S';
           if(v==0) b='3';
           if(v==1) b='4';
           if(v==2) b='5';
           if(v==3) b='6';
           if(v==4) b='7';
           if(v==5) b='8';
           if(v==6) b='9';
           if(v==7) b='T';
           if(v==8) b='J';
           if(v==9) b='Q';
           if(v==10) b='K';
           if(v==11) b='A';
           if(v==12) b='2';
           res[i]=b;
           res[i+1]=a;
           res[i+2]=' ';
           i+=3;
         }
      }
   }
   return *res;
}

void atualiza_mao_char(MAO m,MAO_CHAR M){
    *M=numeros_a_letras(m);
}

int testa_inicio(ESTADO e){
   int l,c,i;
   char a,b;
   i=0;
   c=contar_char(e.M);
   a=e.M[i];
   i++;
   b=a;
   a=e.M[i];
   i++;
   for(l=0;l<c;l++){
      if((b=='3')&&(a=='D')) return 1;
      b=a;
      a=e.M[i];
      i++;
   }
   return 0;
}

void processa_mao_inicio(ESTADO *e,int l){
    int c,i,p;
    i=0;p=4;
    c=contar_char(e->comand[l]);
    while(p<c){
      e->M[i]=e->comand[l][p];
      i++;p++;
    }
    e->m=letras_a_numeros(e->M);
}

void processa_jogada_ext(ESTADO *e,int l){
    int c,i,p,w;
    i=0;p=6;
    c=contar_char(e->comand[l]);
    for(w=0;w<100;w++){
    e->ANT[w]=' ';
    }
    while(p<c){
      e->ANT[i]=e->comand[l][p];
      i++;p++;
    }
    e->ant=letras_a_numeros(e->ANT);
}

void processa_programa_inicio(ESTADO *e){
    MAO aux;
    char AUX;
    int n,v;
    aux=melhor_mao_inicio(e->m);
    for(n=0;n<4;n++){
       for(v=0;v<13;v++){
          if(carta_existe(aux,n,v)) e->m=rem_carta(e->m,n,v);
       } 
    }
    atualiza_mao_char(e->m,e->M);
    AUX=numeros_a_letras(aux);
    printf("%s\n",&AUX);
}

void processa_programa_vitoria(ESTADO *e){
    MAO aux;
    char AUX;
    int n,v;
    aux=melhor_mao(e->m);
    for(n=0;n<4;n++){
       for(v=0;v<13;v++){
          if(carta_existe(aux,n,v)) e->m=rem_carta(e->m,n,v);
       } 
    }
    atualiza_mao_char(e->m,e->M);
    AUX=numeros_a_letras(aux);
    printf("%s\n",&AUX);
}

void processa_programa_reage(ESTADO *e){
    MAO aux;
    char AUX;
    int n,v;
    aux=melhor_mao_possivel(*e);
    for(n=0;n<4;n++){
       for(v=0;v<13;v++){
          if(carta_existe(aux,n,v)) e->m=rem_carta(e->m,n,v);
       } 
    }
    atualiza_mao_char(e->m,e->M);
    AUX=numeros_a_letras(aux);
    printf("%s\n",&AUX);
}

void dump(int c){
    c=c+1;
}

void dump2(char d){
    d=d+d;
}

int main(){
   ESTADO e; 
   int i,l,x;
   int c;
   char *d,f;
   i=0;d=&f;c=0;f=' ';
   e=inicia_ESTADO();
   while((i<8)&&(!EOF)){       
     c=scanf("%c%c",&e.comand[i][0],&e.comand[i][1]);
     if((e.comand[i][0]=='O')&&(e.comand[i][1]=='K')) i++;
     else{
       *d=scanf("%c",&e.comand[i][2]);
       if((e.comand[i][0]=='N')&&(e.comand[i][1]=='A')&&(e.comand[i][2]=='O')) i++;
       else{
         if((e.comand[i][0]=='M')&&(e.comand[i][1]=='A')&&(e.comand[i][2]=='O')){
           c=scanf("%s %s %s %s %s %s %s %s %s %s %s %s %s",&e.comand[i][4],&e.comand[i][7],&e.comand[i][10],&e.comand[i][13],&e.comand[i][16],&e.comand[i][19],&e.comand[i][22],&e.comand[i][25],&e.comand[i][28],&e.comand[i][31],&e.comand[i][34],&e.comand[i][37],&e.comand[i][40]);
           i++;
         }else{
           *d=scanf("%c%c",&e.comand[i][3],&e.comand[i][4]);
           if((e.comand[i][0]=='J')&&(e.comand[i][1]=='O')&&(e.comand[i][2]=='G')&&(e.comand[i][3]=='O')&&(e.comand[i][4]=='U')){
             *d=scanf("%[^\n]",&e.comand[i][6]);
           }else{
             *d=scanf("%c",&e.comand[i][5]);
             if((e.comand[i][0]=='J')&&(e.comand[i][1]=='O')&&(e.comand[i][2]=='G')&&(e.comand[i][3]=='A')&&(e.comand[i][4]=='D')&&(e.comand[i][5]=='A')) i++;
             else{
               if((e.comand[i][0]=='P')&&(e.comand[i][1]=='A')&&(e.comand[i][2]=='S')&&(e.comand[i][3]=='S')&&(e.comand[i][4]=='O')&&(e.comand[i][5]=='U')) i++;
               else{
                 if((e.comand[i][0]=='A')&&(e.comand[i][1]=='C')&&(e.comand[i][2]=='A')&&(e.comand[i][3]=='B')&&(e.comand[i][4]=='O')&&(e.comand[i][5]=='U')) i++;
                 else return 0;
               }
             }
           }
         }
       }
     }
   }
   for(i=0;i<8;i++){
      e.comand[i][99]='\n';
   }
   dump(c);
   dump2(*d);
   x=testa_inicio(e);
   for(l=0;l<8;l++){      
      if((e.comand[l][0]=='M')&&(e.comand[l][1]=='A')&&(e.comand[l][2]=='O')) processa_mao_inicio(&e,l);
      if((e.comand[l][0]=='J')&&(e.comand[l][1]=='O')&&(e.comand[l][2]=='G')&&(e.comand[l][3]=='O')&&(e.comand[l][4]=='U')){ 
        processa_jogada_ext(&e,l);
        e.passagens=0;
      }
      if((e.comand[l][0]=='J')&&(e.comand[l][1]=='O')&&(e.comand[l][2]=='G')&&(e.comand[l][3]=='A')&&(e.comand[l][4]=='D')&&(e.comand[l][5]=='A')){
        if(x==1){
          processa_programa_inicio(&e);
          e.passagens=0;
        }else{
          if(e.passagens==3){
            processa_programa_vitoria(&e);
            e.passagens=0;
          }else{
            processa_programa_reage(&e);
            e.passagens=0;
          }
        }
      }
      if((e.comand[l][0]=='P')&&(e.comand[l][1]=='A')&&(e.comand[l][2]=='S')&&(e.comand[l][3]=='S')&&(e.comand[l][4]=='O')&&(e.comand[l][5]=='U')) e.passagens++;
      if((e.comand[l][0]=='N')&&(e.comand[l][1]=='A')&&(e.comand[l][2]=='O')){
        printf("PASSO");
        return 0;
      }
      if((e.comand[l][0]=='A')&&(e.comand[l][1]=='C')&&(e.comand[l][2]=='A')&&(e.comand[l][3]=='B')&&(e.comand[l][4]=='O')&&(e.comand[l][5]=='U')) return 0;
   }
   return 0;
}

