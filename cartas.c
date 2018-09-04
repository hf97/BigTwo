#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
/brief Definiçao do tipo ESTADO
/brief Esta definicao define o tipo ESTADO como uma estrutura do tipo state, definida mais abaixo no programa
/brief O ESTADO e usado para definir estruturas state ao longo do programa
*/

typedef struct state ESTADO;  
  
/*
/brief  Definiçao do tipo  MAO
/brief Esta definicao define o tipo MAO como um long long int
/brief A MAO e usada para descrever uma mao de cartas ao longo do programa 
*/

typedef long long int MAO;

int random();
double srandom();

/*
/brief  Definição da estrutura state
@param  mao[4]          Define 4 maos que serao usadas como as maos dos 4 jogadores
@param  cartas[4]       Estes ints servem apenas no inicio para contar as 13 cartas no embaralhar das maos
@param  jogada[4]       Define 4 maos que serao usadas para conter a jogada de cada jogador
@param  selecao         Seleção do jogador, ou seja, aquilo que ele selecionou
@param  jogar           Indica se o jogar jogou
@param  passar          Indica se o jogar passou
@param  selecionar      Permite selecionar uma carta
@param  ultimo_jogador  Guarda quem foi o ultimo jogador
@param  ultima_jogada   Contem as cartas da ultima jogada
@param  jogador_atual   Indica o jogador atual
@param  sugerir         Indica se o botao sugestao foi selecionado
@param  ordem           Varia entre 1 e 0 ao carregar no botao ordem, para os 2 estados em que as cartas estao ordenadas(por naipe ou por valor)
@param  passagem        Indica o número de passagens feitas
@param  termina         Indica se a ronda acabou, impede de carregar nos botoes para alem do novo jogo
@param  score[4]        Score para cada um dos 4 jogadores
@param  novo_jogo       Indica que o jogador comecou um novo jogo
@param  acabou          Indica se o jogo acabou, impedindo de carregar em qualquer botao
*/

struct state{
    MAO mao[4];
    int cartas[4];
    MAO jogada[4];
    MAO selecao;
    int jogar, passar, selecionar, ultimo_jogador;
    MAO ultima_jogada;
    int jogador_atual;
    int sugerir;
    int ordem;
    int passagem;
    int termina;
    int score[4];
    int novo_jogo;
    int acabou;
};

/*
/brief Transforma um estado numa string. É utilizada para alterar a query na página.
@param e   O estado atual a transformar em string
*/

void estadoparastr(ESTADO e){
    printf(
        "%lld_%lld_%lld_%lld"
        "_%d_%d_%d_%d"
        "_%lld_%lld_%lld_%lld"        
        "_%lld"
        "_%d_%d_%d_%d"
        "_%lld"
        "_%d"
        "_%d"
        "_%d"
        "_%d"
        "_%d"
        "_%d_%d_%d_%d"
        "_%d"
        "_%d",
        e.mao[0], e.mao[1], e.mao[2], e.mao[3],
        e.cartas[0], e.cartas[1], e.cartas[2], e.cartas[3],
        e.jogada[0], e.jogada[1], e.jogada[2], e.jogada[3],       
        e.selecao,
        e.jogar, e.passar, e.selecionar, e.ultimo_jogador,
        e.ultima_jogada,
        e.jogador_atual,
        e.sugerir,
        e.ordem,
        e.passagem,
        e.termina,
        e.score[0], e.score[1], e.score[2], e.score[3],
        e.novo_jogo,
        e.acabou
        );
}

/*
/brief  Transforma uma querry num estado. Usada quando o utilizador interage com as cartas ou com os botões.
@param  str    A variavel de ambiente a transformar num estado
@return e      O estado correspondente a str dada
*/

ESTADO strparaestado(char* str){
    ESTADO e;
    sscanf(str, 
        "%lld_%lld_%lld_%lld"
        "_%d_%d_%d_%d"
        "_%lld_%lld_%lld_%lld"        
        "_%lld"
        "_%d_%d_%d_%d"
        "_%lld"
        "_%d"
        "_%d"
        "_%d"
        "_%d"
        "_%d"
        "_%d_%d_%d_%d"
        "_%d"
        "_%d",
        &e.mao[0], &e.mao[1], &e.mao[2], &e.mao[3], 
        &e.cartas[0], &e.cartas[1], &e.cartas[2], &e.cartas[3], 
        &e.jogada[0], &e.jogada[1], &e.jogada[2], &e.jogada[3], 
        &e.selecao, 
        &e.jogar, &e.passar, &e.selecionar, &e.ultimo_jogador,
        &e.ultima_jogada,
        &e.jogador_atual,
        &e.sugerir,
        &e.ordem,
        &e.passagem,
        &e.termina,
        &e.score[0], &e.score[1], &e.score[2], &e.score[3],
        &e.novo_jogo,
        &e.acabou
    );    
    return e;
}

/*
/brief  Declara que a função distribuir retorna um ESTADO.
*/

ESTADO distribuir ();

/*
/brief  URL da CGI
*/

#define SCRIPT		"http://127.0.0.1/cgi-bin/cartas"

/*
/brief  URL da pasta com as cartas
*/

#define BARALHO		"http://127.0.0.1/cards"

/*
/brief  Ordem dos naipes
*/

#define NAIPES		"DCHS"

/*
/brief  Ordem das cartas
*/

#define VALORES		"3456789TJQKA2"

/*
/brief  Devolve o índice da carta
@param  naipe	O naipe da carta (inteiro entre 0 e 3)
@param  valor	O valor da carta (inteiro entre 0 e 12)
@return		O índice correspondente à carta
*/

int indice(int naipe, int valor) {
	return naipe * 13 + valor;
}

/* 
/brief  Adiciona uma carta ao estado
@param  ESTADO	O estado atual
@param  naipe	O naipe da carta (inteiro entre 0 e 3)
@param  valor	O valor da carta (inteiro entre 0 e 12)
@return		O novo estado
*/

long long int add_carta(long long int ESTADO, int naipe, int valor) {
	int idx = indice(naipe, valor);
	return ESTADO | ((long long int) 1 << idx);
}

void jogar_primeiro(ESTADO *e);

/*
/brief  Embaralha as cartas
@return O novo estado(depois das cartas estarem distribuidas aos jogadores)
*/

ESTADO distribuir(){
    int n,v,x;
    ESTADO e={{0},{0},{0},0,0,0,0,0,0,0,0,0,0,0,{0},0,0};
    for (n=0;n<4;n++){
        for (v=0;v<13;v++){
            do{x=random()%4;}
            while(e.cartas[x]==13);
            e.mao[x]=add_carta(e.mao[x], n, v);
            e.cartas[x]++;
        }
    }
    return e;
}

/*
/brief  Permite ao jogo continuar depois de carregar no botão em novo jogo.
/brief  Põe todos os campos a zero exepto a ordem e o score dos jogadores e reedestribui um novo baralho para eles jogarem.
@param  O estado do jogo antes de carregar em novo jogo
*/

void distribuir_continuo(ESTADO *e){
    int n,v,x,i;    
    for(i=0;i<4;i++){
       e->mao[i]=0;
       e->jogada[i]=0;
       e->cartas[i]=0;
    }
    e->selecao=0;
    e->jogar=0;
    e->passar=0;
    e->selecionar=0;
    e->ultimo_jogador=0;
    e->ultima_jogada=0;
    e->jogador_atual=0;
    e->sugerir=0;
    e->termina=0;
    for(n=0;n<4;n++){
       for(v=0;v<13;v++){
          do{x=random()%4;}
          while(e->cartas[x]==13);
          e->mao[x]=add_carta(e->mao[x], n, v);
          e->cartas[x]++;
        }
    }
}

/* 
/brief  Remove uma carta do estado
@param  ESTADO	O estado atual
@param  naipe	O naipe da carta (inteiro entre 0 e 3)
@param  valor	O valor da carta (inteiro entre 0 e 12)
@return		O novo estado
*/

long long int rem_carta(long long int ESTADO, int naipe, int valor) {
	int idx = indice(naipe, valor);
	return ESTADO & ~((long long int) 1 << idx);
}

/*
/brief  Verifica se uma carta pertence ao estado
@param  ESTADO	O estado atual
@param  naipe	O naipe da carta (inteiro entre 0 e 3)
@param  valor	O valor da carta (inteiro entre 0 e 12)
@return		1 se a carta existe e 0 caso contrário
*/

int carta_existe(long long int ESTADO, int naipe, int valor) {
	int idx = indice(naipe, valor);
	return (ESTADO >> idx) & 1;
}

/*
/brief  Conta quantas cartas existem numa MAO
@param  m  mao do jogador
@return c  O numero de cartas da mao dada 
*/

int contar(MAO m){
        int c=0,n,v;
        for(n=0;n<4;n++){
           for(v=0;v<13;v++){
              if(carta_existe(m,n,v)) c++;
           }
        }
        return c;           
}

/*
/brief  Verifica qual a carta de maior valor na mao de um jogador.
/brief  Para isto, procura por cartas começando no naipe mais valioso e na carta mais valiosa, e depois vai reduzindo até encontrar uma carta.
/brief  É usada para comparar as jogadas do ultimo jogador e do jogador atual, na função maior_comb.
@param  m    mao do jogador
@return v    Valor da carta com maior valor
*/

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

/*
/brief  Verifica qual a carta com o maior naipe da mão de um jogador.
/brief  Para isto, procura por cartas começando no valor e naipe maior, e depois vai reduzindo até encontrar uma carta.
@param  m      mao do jogador
@retunr n      Naipe da carta com maior naipe
*/

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

/*
/brief  Verifica qual a carta de valor da mão de um jogador.
/brief  Procura no naipe e valor mais pequeno e vai aumentando até encontrar a carta mais baixa.
@param  m     mao do jogador
@return v     Valor da carta de menor valor
*/

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

/*
/brief  Verifica qual a carta com o menor naipe da mão de um jogador.
/brief  Procura no naipe e valor mais pequeno e vai aumentando até encontrar a carta mais baixa.
@param  m     mao do jogador
@return n     Naipe da carta de menor naipe
*/

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

/*
/brief  Testa a existencia de um flush na mao de um jogador
@param  m      mao do jogador
@return        1 se existe Flush na mao 0 se nao
*/

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

/*
/brief  Testa a existencia de um poker(four-of-a-kind) na mao de um jogador
@param  m      mao do jogador
@return        1 se existe four of a kind na mao, 0 se nao
*/

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

/*
/brief  Testa a existencia de um straight na mao de um jogador
@param  m      mao do jogador
@return        0 se nao existe straight e 1 a 10 de acordo com o valor da sequencia, sendo 10 a mais alta e 1 a mais baixa
*/

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

/*
/brief  Testa a existencia de um fullhouse na mao de um jogador
@param  m      mao do jogador
@return        1 se existe full house na mao, 0 se nao
*/

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

/*
/brief  Testa a existencia de um straight flush
@param  m      mao do jogador
@return        1 se é um straight flush, 0 caso contrario
*/

int straight_flush(MAO m){
   return((flush(m))&&(straight(m)!=0));
}

/*
/brief  Compara, dadas 2 maos e um valor de uma carta, o naipe da carta com esse valor
@param  m1     mao de um jogador
@param  m2     mao de um segundo jogador
@param  m      valor da carta cujo naipe se vai verificar que é maior
@return        1 se o valor do naipe da primeira mao for maior que o da segunda, 0 caso contrario.
*/

int compara_naipe(MAO m1, MAO m2, int m){
   int n,n1=0,n2=0;
   for(n=0;n<4;n++){
      if(carta_existe(m1,n,m)) n1=n;
      if(carta_existe(m2,n,m)) n2=n;
   }
   return (n1>n2);
}

/*
/brief  Compara 2 four of a kind e ve se o primeiro e maior que o segundo
@param  m1     mao de um jogador
@param  m2     mao de um segundo jogador
@return        1 se o valor das cartas da primeira mao for maior que o da segunda, 0 caso contrario.
*/

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

/*
/brief  Compara 2 full house e ve se o primeiro e maior que o segundo
@param  m1     mao de um jogador
@param  m2     mao de um segundo jogador
@return        1 se o valor das cartas do trio da primeira mao for maior que o da segunda, 0 caso contrario.
*/

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

/*
/brief  Compara 2 flush e ve se o primeiro e maior que o segundo
@param  m1     mao de um jogador
@param  m2     mao de um segundo jogador
@return        1 se o valor do naipe da primeira mao for maior que o da segunda, 0 caso contrario.
*/

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

/*
/brief  Compara 2 straight e ve se o primeiro e maior que o segundo
@param  m1     mao de um jogador
@param  m2     mao de um segundo jogador
@return        1 se o straight de m1 é maior que o de m2
*/

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

/*
/brief  Compara 2 stright flush e ve se o primeiro e maior que o segundo
@param  m1     mao de um jogador
@param  m2     mao de um segundo jogador 
@return        1 se o valor do naipe da primeira mao for maior que o da segunda, 0 caso contrario.
*/

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

/*
/brief  Verifica qual o tipo de combinacao de 5 uma mao representa.
@param  m       mao do jogador
@return         5 se a combinacao foi straight flush, 4 se for four_of_a_kind, 3 se for full_house, 2 se for flush,1 se for straight e 0 se nao for uma combinacao de 5 cartas valida.
*/

int tipo_comb_5(MAO m){
   if(straight_flush(m)) return 5;
   if(four_of_a_kind(m)) return 4;
   if(full_house(m)) return 3;
   if(flush(m)) return 2;
   if(straight(m)) return 1;   
   return 0;
}

/*
/brief  Verifica se, dada uma jogada feita com mais de 1 carta, os valores das cartas jogadas são iguais.
/brief  Para fazer isto, verifica primeiro a existência de 1 carta, e depois verifica se o valor dessa carta é o maior da jogada(que tem de ser, pois todas as cartas devem ter o mesmo valor).
@param  m        mao do jogador
@return          1 se o valor da carta for o maior da jogada, 0 se nao
*/

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

/*
/brief  Verifica se a combinação de cartas da ultima jogada feita é maior que o da selecao feita pelo jogador.
/brief  É usada quando o jogador joga, para comparar com a jogada do computador.
@param  s      mao da selecao
@param  uj     mao do ultimo jogador
@return        1 se o valor da combinação de cartas da selecao é maior, 0 caso contrario.
*/

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

/*
/brief  Verifica se uma combinação de cartas que lhe é dada é válida.
@param  m      mao de um jogador     
@return        1 se é combinação valida, 0 caso contrario.
*/

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

/*
/brief  Verifica se é possivel ao jogador jogar.
É usada no imprime_jogada para impedir o jogador de jogar se não for válida a jogada.
@param  e      O estado do jogo atual
@return        1 se é possivel jogar, 0 caso contrario.
*/

int possivel_jogar(ESTADO e){
    if(!e.termina){
       if((e.jogador_atual==0)&&(comb_val(e.selecao))){
         if((e.ultimo_jogador!=0)&&(maior_comb(e.selecao,e.ultima_jogada))){
           if((contar(e.ultima_jogada))==(contar(e.selecao))){
             return 1;
           }else{
             return 0;
           }
         }else{
           if(carta_existe(e.mao[0],0,0)){
             if((carta_existe(e.selecao,0,0))&&(comb_val(e.selecao))){
               return 1;
             }else{
               return 0;
             }
           }else{
              if((e.passagem==3)&&(e.jogador_atual==0)&&(comb_val(e.selecao))){
                return 1;
              }else{
                return 0;
              }
           }
         }
       }
    }
    return 0;
}

/*
/brief  Verifica se e possivel passar
@param  e      O estado do jogo atual
@return        1 se é possivel passar, 0 caso contrario.
*/

int possivel_passar(ESTADO e){
    if(!e.termina){
      if(carta_existe(e.mao[0],0,0)) return 0;
      else{
        if((e.ultima_jogada!=0)&&(e.passagem!=3)) return 1;
      }
    }
    return 0;
}

/*
/brief  Verifica se o jogador venceu
/brief  Para isso, verifica se a mao de um jogador está ou não vazia, e no futuro será usada para o calculo do score do jogador que venceu.
@param  m      mao do jogador.      
@return        1 se o jogador tem a mao vazia e venceu, 0 caso contrario.
*/

int vencedor(MAO m){
    int i;
    i=contar(m);
    if(i==0) return 1;
    else return 0;
}

/*
/brief  Funcao que calcula a pontuacao final do vencedor
@param  e      O estado do jogo atual
@return s      Score.
*/

int calc_score(ESTADO e){
    int m;
    int n,s=0;
    for(n=0;n<4;n++){
       m=contar(e.mao[n]);
       if(m<10){
         s+=m;
       }
       if((m>=10)&&(m<13)){
         s+=(m*2);
       }
       if(m==13){
         s+=(m*3);
       }
    }
    return s;
}

/*
/brief  Imprime as cartas da seleção do jogador e dá a ordem para mudar a query que se vê no ecrã(variavel de ambiente)
@param  x          Coordenada em relação ao eixo dos xx
@param  y          Coordenada em relação ao eixo dos yy
@param  e          O estado do jogo atual
@param  naipe      O naipe da carta
@param  valor      O valor da carta
*/

void imprime_carta(int x, int y, ESTADO e, int naipe, int valor) {
    char *suit = NAIPES;
    char *rank = VALORES; 
    char *path = BARALHO;   
    ESTADO print=e;
    if(carta_existe(print.selecao, naipe, valor)){
      print.selecao=rem_carta(print.selecao, naipe, valor);
    }else{
      print.selecao=add_carta(print.selecao, naipe, valor);
    }
    print.selecionar=1;
    printf("<a xlink:href = \"%s?", SCRIPT);
    estadoparastr(print);
    printf("\"><image x = \"%d\" y = \"%d\" height = \"110\" width = \"80\" xlink:href = \"%s/%c%c.svg\" /></a>\n", x, y, path, rank[valor], suit[naipe]);         
}

/*
/brief  Imprime uma jogada de um jogador
/brief  Estas cartas não são possiveis de interação por parte do jogador.
@param  x          Coordenada em relação ao eixo dos xx
@param  y          Coordenada em relação ao eixo dos yy
@param  naipe      O naipe da carta
@param  valor      O valor da carta
*/

void imprime_jogada(int x, int y, int naipe, int valor){
     char *suit = NAIPES;
     char *rank = VALORES;
     char *path = BARALHO;
     printf("\"><image x = \"%d\" y = \"%d\" height = \"110\" width = \"80\" xlink:href = \"%s/%c%c.svg\" /></a>\n", x, y, path, rank[valor], suit[naipe]);
}

/*
/brief  Imprime o botão limpar, que limpa todas as cartas da seleção do jogador
/brief  Não remove as cartas dos outros jogadores
@param  e  O estado do jogo atual
*/

void imprime_limpar(ESTADO e){
     if(!e.termina){
       e.selecao=0;
       printf("<a xlink:href = \"%s?", SCRIPT);
       estadoparastr(e);
       printf("\"> <rect x=820 y=5 width=60 height=55 ry=5 fill = \"rgb(500,500,0)\" /></a>\n"); 
       printf("<text x=\"850\" y= \"35\" text-anchor = \"middle\" text-align = \"center\" font-family = \"serif\" font-weight = \"bold\"> Limpar </text> \n");
     }else{
       printf("\"> <rect x=820 y=5 width=60 height=55 ry=5 fill = \"rgb(500,500,0)\" /></a>\n"); 
       printf("<text x=\"850\" y= \"35\" text-anchor = \"middle\" text-align = \"center\" font-family = \"serif\" font-weight = \"bold\"> Limpar </text> \n");
     }
}

/*
/brief  Imprime o botão jogar.
@param  e    O estado do jogo atual
*/

void imprime_jogar(ESTADO e){
     if(possivel_jogar(e)){
       e.jogar=1;
       e.passagem=0;
       printf("<a xlink:href = \"%s?", SCRIPT);
       estadoparastr(e);
       printf("\"> <rect x=680 y=5 width=60 height=55 ry=5 fill=\"rgb(500,500,0)\" /></a>\n");
       printf("<text x=\"710\" y=\"35\" text-anchor = \"middle\" text-align = \"center\" font-family = \"serif\" font-weight = \"bold\"> Jogar </text> \n");
     }else{
       printf("\"> <rect x=680 y=5 width=60 height=55 ry=5 fill=\"rgb(500,500,0)\" /></a>\n");
       printf("<text x=\"710\" y=\"35\" text-anchor = \"middle\" text-align = \"center\" font-family = \"serif\" font-weight = \"bold\"> Jogar  </text> \n");
     }
}

/*
/brief  Imprime o botão passa.
Se o jogador carregar neste botão, ele vai passar.
@param  e    O estado do jogo atual
*/

void imprime_passa(ESTADO e){
       if(possivel_passar(e)){
         e.passar=1;
         e.passagem++;
         printf("<a xlink:href = \"%s?", SCRIPT);
         estadoparastr(e);
         printf("\"><rect x=\"750\" y=\"5\" width=60 height=55 ry=5 fill = \"rgb(500,500,0)\" /></a>\n");
         printf("<text x=\"780\" y=\"35\" text-anchor = \"middle\" text-align = \"center\" font-family = \"serif\" font-weight = \"bold\"> Passar </text> \n");
       }else{
         printf("\"><rect x=\"750\" y=\"5\" width=60 height=55 ry=5 fill = \"rgb(500,500,0)\" /></a>\n");
         printf("<text x=\"780\" y=\"35\" text-anchor = \"middle\" text-align = \"center\" font-family = \"serif\" font-weight = \"bold\"> Passar </text> \n");
       }
}

/*
/brief  Imprime o botão ordena por naipe/valor.
@param  e    O estado do jogo atual
*/

void imprime_ordena(ESTADO e){
     if((!e.termina)&&(!e.acabou)){
       if(e.ordem == 1){
         e.ordem = 0;
         printf("<a xlink:href = \"%s?", SCRIPT);
         estadoparastr(e);
         printf("\"><rect x=\"680\" y=\"65\" width=60 height=55 ry=5 fill = \"rgb(500,500,0)\" /></a>\n");
         printf("<text x=\"710\" y=\"95\" text-anchor = \"middle\" text-align = \"center\" font-family = \"serif\" font-weight = \"bold\"> Ordena </text> \n");
       }else{
         e.ordem=1;
         printf("<a xlink:href = \"%s?", SCRIPT);
         estadoparastr(e);
         printf("\"><rect x=\"680\" y=\"65\" width=60 height=55 ry=5 fill = \"rgb(500,500,0)\" /></a>\n");
         printf("<text x=\"710\" y=\"95\" text-anchor = \"middle\" text-align = \"center\" font-family = \"serif\" font-weight = \"bold\"> Ordena </text> \n");
       }   
     }else{
       printf("\"><rect x=\"680\" y=\"65\" width=60 height=55 ry=5 fill = \"rgb(500,500,0)\" /></a>\n");
       printf("<text x=\"710\" y=\"95\" text-anchor = \"middle\" text-align = \"center\" font-family = \"serif\" font-weight = \"bold\"> Ordena </text> \n");
     }
}

/*
/brief  Imprime o botão novo_jogo
@param  e    O estado do jogo atual
*/

void imprime_novo_jogo(ESTADO e){
     if(!e.acabou){
       e.novo_jogo=1;
       printf("<a xlink:href = \"%s?", SCRIPT);
       estadoparastr(e);
       printf("\"><rect x=\"820\" y=\"65\" width=60 height=55 ry=5 fill = \"rgb(500,500,0)\" /></a>\n");
       printf("<text x=\"850\" y=\"85\" text-anchor = \"middle\" text-align = \"center\" font-family = \"serif\" font-weight = \"bold\"> Novo </text> \n");
       printf("<text x=\"850\" y=\"105\" text-anchor = \"middle\" text-align = \"center\" font-family = \"serif\" font-weight = \"bold\"> Jogo </text> \n");
     }else{
       printf("\"><rect x=\"820\" y=\"65\" width=60 height=55 ry=5 fill = \"rgb(500,500,0)\" /></a>\n");
       printf("<text x=\"850\" y=\"85\" text-anchor = \"middle\" text-align = \"center\" font-family = \"serif\" font-weight = \"bold\"> Novo </text> \n");
       printf("<text x=\"850\" y=\"105\" text-anchor = \"middle\" text-align = \"center\" font-family = \"serif\" font-weight = \"bold\"> Jogo </text> \n");
     }
}

/*
/brief  Imprime as mãos do jogador, seguido das jogadas dos computadores
/brief  Nenhuma das cartas imprimidas por esta função é possivel de ser interagida com. 
@param  e    O estado do jogo atual
*/

void imprime_mao(ESTADO e){
     int n,v;
     int x=10,y=10,z=460;
     if(e.ordem==0){
       for(n=0;n<4;n++){
          for(v=0;v<13;v++){
             if((carta_existe(e.mao[0],n,v))&&(!carta_existe(e.selecao,n,v))){
               y=10;
               x+=20;
               imprime_carta(x, y, e, n, v);
             }
             if(carta_existe(e.selecao,n,v)){
               z+=20;
               imprime_carta(z, y, e, n, v);
             }
          }
      }
    }else{
      for(v=0;v<13;v++){
          for(n=0;n<4;n++){
             if((carta_existe(e.mao[0],n,v))&&(!carta_existe(e.selecao,n,v))){
               y=10;
               x+=20;
               imprime_carta(x, y, e, n, v);
             }
             if(carta_existe(e.selecao,n,v)){
               z+=20;
               imprime_carta(z, y, e, n, v);
             }
          }
      }
    }
}

/*
/brief  Imprime a caixa passar para os computadores
@param  e    O estado do jogo atual
*/

void imprime_passagem(ESTADO e){
       int x=510;
       int y[4]={60,210,360,510};  
       int m,p=0,j;
       if((!e.termina)&&(!e.acabou)){
         for(m=1;m<4;m++){
            if((e.jogada[m]==0)&&(e.ultima_jogada!=0)&&(((contar(e.jogada[0]))+(contar(e.jogada[1]))+(contar(e.jogada[2]))+(contar(e.jogada[3]))+(contar(e.mao[0]))+(contar(e.mao[1]))+(contar(e.mao[2]))+(contar(e.mao[3])))!=52)){
              printf("<svg height=900 height=900 >\n");
              printf("<rect x=\"%d\" y=\"%d\" width=60 height=50 ry=10 fill = \"rgb(255,120,0)\" />\n", x-30, y[m]-30);
              printf("<text x=\"%d\" y=\"%d\" text-anchor = \"middle\" text-align = \"center\" font-family = \"serif\" font-weight = \"bold\"> Passou </text> \n", x, y[m]);
              printf("</svg>\n");
           }else{
              for(j=0;j<4;j++){
                 if(carta_existe(e.jogada[j],0,0)) p=j;
              }
              if((e.jogada[m]==0)&&(e.ultima_jogada!=0)&&(m>p)){
                printf("<svg height=900 height=900 >\n");
                printf("<rect x=\"%d\" y=\"%d\" width=60 height=50 ry=10 fill = \"rgb(255,120,0)\" />\n", x-30, y[m]-30);
                printf("<text x=\"%d\" y=\"%d\" text-anchor = \"middle\" text-align = \"center\" font-family = \"serif\" font-weight = \"bold\"> Passou </text> \n", x, y[m]);
                printf("</svg>\n");
              }
           }
         }   
       }  
}

/*
/brief  Imprime a jogada de um computador
@param  e    O estado do jogo atual
*/

void imprime_jogada_comp(ESTADO e){
     int n,v;
     int m;
     int x=460;
     int y[4]={10,150,300,450};
     for(m=1;m<4;m++){
        for(n=0;n<4;n++){
           for(v=0;v<13;v++){
              if(carta_existe(e.jogada[m],n,v)){
                x+=20;
                imprime_jogada(x, y[m], n, v);
              }
           }
        }
        x=460;
     }
}

/*
/brief  Imprime a sugestao da melhor jogada do jogador humano
@param  e    O estado do jogo atual
*/

void imprime_sugestao(ESTADO e){
     if((!e.termina)&&(!e.acabou)){
       e.sugerir=1;
       printf("<a xlink:href = \"%s?", SCRIPT);
       estadoparastr(e);
       printf("\"><rect x=\"750\" y=\"65\" width=60 height=55 ry=5 fill = \"rgb(500,500,0)\" /></a>\n");
       printf("<text x=\"780\" y=\"95\" text-anchor = \"middle\" text-align = \"center\" font-family = \"serif\" font-weight = \"bold\"> Sugestão </text> \n");
     }else{
       printf("\"><rect x=\"750\" y=\"65\" width=60 height=55 ry=5 fill = \"rgb(500,500,0)\" /></a>\n");
       printf("<text x=\"780\" y=\"95\" text-anchor = \"middle\" text-align = \"center\" font-family = \"serif\" font-weight = \"bold\"> Sugestão </text> \n");
     }
}

/*
/brief  Imprime a base do jogo.
*/

void imprime_base (){
     printf("<rect x = \"0\"   y = \"0\" height = \"600\" width = \"900\" style = \"fill:#007700\"/>\n");
}

/*
/brief  Imprime caixas que contêm o nome dos jogadores e o nº de cartas que eles têm nas suas mãos.
@param  e    O estado do jogo atual
*/

void imprime_jogadores (ESTADO e){
     int m1,m2,m3;
     m1=contar(e.mao[1]);
     m2=contar(e.mao[2]);
     m3=contar(e.mao[3]);
     printf("<svg height=900 height=900 >\n");
     printf("<rect x=\"30\" y=\"170\" width=100 height=65 ry=5 fill = \"rgb(500,500,0)\" />\n");
     printf("<text x=\"80\" y=\"200\" text-anchor = \"middle\" text-align = \"center\" font-family = \"serif\" font-weight = \"bold\"> Jogador 2 </text> \n");
     printf("<text x=\"80\" y=\"220\" text-anchor = \"middle\" text-align = \"center\" font-family = \"serif\" font-weight = \"bold\"> %d cartas</text> \n", m1);
     printf("</svg>\n");
     printf("<svg height=900 height=900 >\n");
     printf("<rect x=\"30\" y=\"320\" width=100 height=65 ry=5 fill = \"rgb(500,500,0)\" />\n");
     printf("<text x=\"80\" y=\"350\" text-anchor = \"middle\" text-align = \"center\" font-family = \"serif\" font-weight = \"bold\"> Jogador 3 </text> \n");
     printf("<text x=\"80\" y=\"370\" text-anchor = \"middle\" text-align = \"center\" font-family = \"serif\" font-weight = \"bold\"> %d cartas</text> \n", m2);
     printf("</svg>\n");
     printf("<svg height=900 height=900 >\n");
     printf("<rect x=\"30\" y=\"470\" width=100 height=65 ry=5 fill = \"rgb(500,500,0)\" />\n");
     printf("<text x=\"80\" y=\"500\" text-anchor = \"middle\" text-align = \"center\" font-family = \"serif\" font-weight = \"bold\"> Jogador 4 </text> \n");
     printf("<text x=\"80\" y=\"520\" text-anchor = \"middle\" text-align = \"center\" font-family = \"serif\" font-weight = \"bold\"> %d cartas</text> \n", m3);
     printf("</svg>\n");
}

/*
/brief  Imprime uma caixa com uma mensagem de vitória no caso de um dos jogadores esvaziar a sua mão.
@param  e    O estado do jogo atual
*/

void imprime_vencedor (ESTADO *e){  
     if(!e->acabou){
       if(vencedor(e->mao[0])){        
         e->score[0]+=calc_score(*e);
         printf("<svg height=900 height=900 >\n");
         printf("<text x=\"220\" y=\"65\" text-anchor = \"middle\" text-align = \"center\" font-family = \"serif\" font-weight = \"bold\"> Jogador 1 vence!</text> \n");
         printf("<text x=\"220\" y=\"80\" text-anchor = \"middle\" text-align = \"center\" font-family = \"serif\" font-weight = \"bold\"> Score=%d </text> \n",e->score[0]);
         printf("</svg>\n");
         if(e->score[0]>=75) e->acabou=1;
         e->termina=1;
       }else{
          if(vencedor(e->mao[1])){
            e->score[1]+=calc_score(*e);
            printf("<svg height=900 height=900 >\n");
            printf("<text x=\"220\" y=\"210\" text-anchor = \"middle\" text-align = \"center\" font-family = \"serif\" font-weight = \"bold\"> Jogador 2 vence!</text> \n");
            printf("<text x=\"220\" y=\"225\" text-anchor = \"middle\" text-align = \"center\" font-family = \"serif\" font-weight = \"bold\"> Score=%d </text> \n",e->score[1]);
            printf("</svg>\n");
            if(e->score[1]>=75) e->acabou=1;
            e->termina=1;
          }else{
            if(vencedor(e->mao[2])){
              e->score[2]+=calc_score(*e);
              printf("<svg height=900 height=900 >\n");
              printf("<text x=\"220\" y=\"360\" text-anchor = \"middle\" text-align = \"center\" font-family = \"serif\" font-weight = \"bold\"> Jogador 3 vence!</text> \n");
              printf("<text x=\"220\" y=\"375\" text-anchor = \"middle\" text-align = \"center\" font-family = \"serif\" font-weight = \"bold\"> Score=%d </text> \n",e->score[2]);
              printf("</svg>\n");
              if(e->score[2]>=75) e->acabou=1;
              e->termina=1;
            }else{ 
               if(vencedor(e->mao[3])){
                 e->score[3]+=calc_score(*e);
                 printf("<svg height=900 height=900 >\n");
                 printf("<text x=\"220\" y=\"510\" text-anchor = \"middle\" text-align = \"center\" font-family = \"serif\" font-weight = \"bold\"> Jogador 4 vence!</text> \n");
                 printf("<text x=\"220\" y=\"525\" text-anchor = \"middle\" text-align = \"center\" font-family = \"serif\" font-weight = \"bold\"> Score=%d </text> \n",e->score[3]);
                 printf("</svg>\n");
                 if(e->score[3]>=75) e->acabou=1;
                 e->termina=1;
               }          
            }
          }
       } 
     }else{
       printf("<svg height=900 height=900 >\n");
       if(e->score[0]>=75){
         printf("<text x=\"220\" y=\"65\" text-anchor = \"middle\" text-align = \"center\" font-family = \"serif\" font-weight = \"bold\"> Jogador 1 vence!</text> \n");
         printf("<text x=\"220\" y=\"80\" text-anchor = \"middle\" text-align = \"center\" font-family = \"serif\" font-weight = \"bold\"> Score=%d </text> \n",e->score[0]);
       }
       if(e->score[1]>=75){
         printf("<text x=\"220\" y=\"210\" text-anchor = \"middle\" text-align = \"center\" font-family = \"serif\" font-weight = \"bold\"> Jogador 2 vence!</text> \n");
         printf("<text x=\"220\" y=\"225\" text-anchor = \"middle\" text-align = \"center\" font-family = \"serif\" font-weight = \"bold\"> Score=%d </text> \n",e->score[1]);
       }
       if(e->score[2]>=75){
         printf("<text x=\"220\" y=\"360\" text-anchor = \"middle\" text-align = \"center\" font-family = \"serif\" font-weight = \"bold\"> Jogador 3 vence!</text> \n");
         printf("<text x=\"220\" y=\"375\" text-anchor = \"middle\" text-align = \"center\" font-family = \"serif\" font-weight = \"bold\"> Score=%d </text> \n",e->score[2]);
       }
       if(e->score[3]>=75){
         printf("<text x=\"220\" y=\"510\" text-anchor = \"middle\" text-align = \"center\" font-family = \"serif\" font-weight = \"bold\"> Jogador 4 vence!</text> \n");
         printf("<text x=\"220\" y=\"525\" text-anchor = \"middle\" text-align = \"center\" font-family = \"serif\" font-weight = \"bold\"> Score=%d </text> \n",e->score[3]);
       }
       printf("</svg>\n");
     }   
}

/*
/brief  Remove da 1ª mão todas as cartas que constam da 2ª.
/brief  Por exemplo, rem_mao(m1,m2) remove de m1 todas as cartas que estão em m2(se m1 não tiver uma carta de m2, a função procura na próxima carta de m2).
@param  m      mao do jogador
@param  e      mao da qual removemos as cartas
*/

void rem_mao(MAO m, MAO *e){
     int n,v;
     for(n=0;n<4;n++){
        for(v=0;v<13;v++){
           if(carta_existe(m,n,v)){
             *e=rem_carta(*e,n,v);
           }
        }
     }
}

/*
/brief  Imprime todas as componentes do jogo no ecrã.
@param  e    O estado do jogo atual
*/

void imprime(ESTADO e){       
     imprime_vencedor(&e);
     if(e.acabou) imprime_vencedor(&e);
     imprime_mao(e);
     imprime_jogar(e);
     imprime_passa(e);
     imprime_limpar(e);     
     imprime_jogadores(e);
     imprime_ordena(e);
     imprime_novo_jogo(e);
     imprime_sugestao(e);
     imprime_passagem(e);
     imprime_jogada_comp(e);
}

/*
/brief  Esta função serve para criar pares de uma mao de um jogador
@param  m      mao do jogador
@return        1 se é um par, 0 caso contrario
*/

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

/*
/brief  Esta função serve para criar trios de uma mao de um jogador
@param  m      mao do jogador
@return        trio se existir, 0 caso contrario
*/

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

/*
/brief  Esta função serve para criar maos four_of_a_kind de uma mao de um jogador
@param  m      mao do jogador
@return        for of a kind se existir, 0 caso contrario
*/

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

/*
/brief  Esta função serve para criar maos full_house de uma mao de um jogador
@param  m      mao do jogador
@return        full house se existir, 0 caso contrario
*/

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

/*
/brief  Esta função serve para criar maos flush de uma mao de um jogador
@param  m      mao do jogador
@return        flush se existir, 0 caso contrario
*/

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

/*
/brief  Esta função serve para criar maos straight de uma mao de um jogador
@param  m      mao do jogador
@return        straight se existir, 0 caso contrario
*/

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

/*
/breif  Esta função serve para criar maos straight_flush de uma mao de um jogador
@param  m      mao do jogador
@return        straight flush se existir, 0 caso contrario
*/

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


/*
/brief  Esta função serve para criar pares de uma mao de um jogador com o 3 de ouros
@param  m      mao do jogador
@return        par se existir, 0 caso contrario
*/

MAO cria_pares_inicio(MAO m){
   int n;
   MAO aux=0;
   for(n=0;n<4;n++){
      if((carta_existe(m,n,0))&&(contar(aux)<2)) aux=add_carta(aux,n,0);
   }
   if(contar(aux)==2) return aux;
   else return 0;
}

/*
/brief  Esta função serve para criar trios de uma mao de um jogador com o 3 de ouros
@param  m      mao do jogador
@return        trio se existir, 0 caso contrario
*/

MAO cria_trios_inicio(MAO m){
   int n;
   MAO aux=0;
   for(n=0;n<4;n++){
      if((carta_existe(m,n,0))&&(contar(aux)<3)) aux=add_carta(aux,n,0);
   }
   if(contar(aux)==3) return aux;
   else return 0;
}

/*
/brief  Esta função serve para criar maos four of a kind de uma mao de um jogador com o 3 de ouros
@param  m      mao do jogador
@return        for of a kind se existir, 0 caso contrario
*/


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

/*
Esta funcao serve para criar maos full house de uma mao de um jogador com o 3 de ouros
@param  m      mao do jogador
@return        full house se existir, 0 caso contrario
*/

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

/*
/brief  Esta função serve para criar maos flush de uma mao de um jogador 3 de ouros
@param  m      mao do jogador
@return        flush se existir, 0 caso contrario
*/

MAO cria_flush_inicio(MAO m){
   int v;
   MAO aux=0;
   for(v=0;v<13;v++){
      if(((carta_existe(m,0,v))&&(contar(aux)<5))) aux=add_carta(aux,0,v);
   }
   if(flush(aux)) return aux;
   else return 0;
}

/*
/brief  Esta função serve para criar maos straight de uma mao de um jogador com o 3 de ouros
@param  m      mao do jogador
@return        straight se existir, 0 caso contrario
*/

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

/*
/brief  Esta função serve para criar maos straight flush de uma mao de um jogador com o 3 de ouros
@param  m      mao do jogador
@return        straight flush se existir, 0 caso contrario
*/

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

/*
/brief  Esta função serve para criar maos four_of_a_kind do jogador principal caso ja outro jogador tenha jogado
@param  m      mao do jogador
@param  a      mao do jogador que jogou antes
@param  t      tipo de combinação de 5 cartas da mao do jogador anterior
@return        1 se é um four of a kind, 0 caso contrario
*/

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

/*
/brief  Esta função serve para criar maos full_house do jogador principal caso ja outro jogador tenha jogado
@param  m      mao do jogador
@param  a      mao do jogador que jogou antes
@param  t      tipo de combinação de 5 cartas da mao do jogador anterior
@return        1 se é um full house, 0 caso contrario
*/

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

/*
/brief  Esta função serve para criar maos flush de uma mao do jogador principal caso ja outro jogador tenha jogado
@param  m      mao do jogador
@param  a      mao do jogador que jogou antes
@param  t      tipo de combinação de 5 cartas da mao do jogador anterior
@return        1 se é um flush, 0 caso contrario
*/

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

/*
/breif  Esta função serve para criar maos straight de uma mao do jogador principal caso ja outro jogador tenha jogado
@param  m      mao do jogador
@param  a      mao do jogador que jogou antes
@param  t      tipo de combinação de 5 cartas da mao do jogador anterior
@return        1 se é um straight, 0 caso contrario
*/

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

/*
/breif  Esta função serve para criar maos straight_flush de uma mao do jogador principal caso ja outro jogador tenha jogado
@param  m      mao do jogador
@param  a      mao do jogador que jogou antes
@param  t      tipo de combinação de 5 cartas da mao do jogador anterior
@return        1 se é um straight flush, 0 caso contrario
*/

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

/*
/breif  Esta função cria a melhor mao que o jogador pode jogar caso ja alguem tenha jogado anteriormente
@param  e      o estado do jogo atual
@param  t      tipo de combinação de 5 cartas da mao do jogador anterior      
*/

MAO reage_a_5_cartas(ESTADO e,int t){
   MAO a=e.ultima_jogada;
   MAO m=e.mao[0];
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

/*
/brief  Para uma mao de um jogador calcula a melhor combinacao de 1,2,3 ou 5 cartas possivel, e retorna-a
@param  x      mao do jogador
@return        melhor mao possivel
*/

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

/*
/brief  Função que calcula a melhor mao no inicio do jogo
@param  x      mao do jogador
@return        melhor mao possivel com o 3 de ouros
*/

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

/*
/brief  Função que dá a melhor mao que é possivel jogar
@param  e      o estado do jogo atual
@return        melhor mao possivel considerando a anterior
*/

MAO melhor_mao_possivel(ESTADO e){
    MAO aux=0;
    MAO tmp;
    MAO m=e.ultima_jogada;
    int c,n,v;
    int k,w;
    int t=tipo_comb_5(m);
    c=contar(m);
    n=carta_maior_naipe(m);
    v=carta_maior_valor(m);
    if(c==1){
      for(k=v;k<13;k++){
         for(w=n;w<4;w++){
            if(carta_existe(e.mao[0],w,k)){
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
              if((carta_existe(e.mao[0],w,k))&&(contar(tmp)<2)){
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
                if((carta_existe(e.mao[0],w,k))&&(contar(tmp)<3)&&(mao_valor_igual(tmp))){
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



void jogar (ESTADO *e);

/*
/brief  Esta função serve para o computador jogar four_of_a_kind
@param  e      o estado do jogo atual
@param  t      tipo de combinação de 5 cartas da mao do jogador anterior
*/

void jogar_four_of_a_kind(ESTADO *e, int t){
   int n,v,p,s;
   int x;
   MAO aux;
   x=e->jogador_atual;
   e->jogada[x]=0;
   aux=0;
   for(p=0;p<13;p++){
      for(v=0;v<13;v++){
         s=v;
         for(n=0;n<4;n++){
            if(carta_existe(e->mao[x],n,v)) aux=add_carta(aux,n,v);
         }
         if(contar(aux)==4) break;
         else aux=0;
      }
      if(aux!=0){
        for(v=0;v<13;v++){
           for(n=0;n<4;n++){
              if((carta_existe(e->mao[x],n,v))&&(v!=s)){
                aux=add_carta(aux,n,v);
              }
           }
        }
      }
      if((four_of_a_kind(aux))&&((compara_four_of_a_kind(aux,e->ultima_jogada))||(t<4))) break;
      else aux=0;
   }
   if((four_of_a_kind(aux))&&((compara_four_of_a_kind(aux,e->ultima_jogada))||(t<4))){
     e->ultima_jogada=aux;
     e->ultimo_jogador=x;
     e->jogada[x]=aux;
     e->passagem=0;
     rem_mao(aux,&e->mao[x]);
   }
}

/*
/brief  Esta função serve para o computador jogar full_house
@param  e      o estado do jogo atual
@param  t      tipo de combinação de 5 cartas da mao do jogador anterior
*/

void jogar_full_house(ESTADO *e, int t){
   int n,v,p;
   int s;
   int x;
   MAO aux;
   MAO tmp;
   x=e->jogador_atual;
   e->jogada[x]=0;
   aux=0;
   tmp=0;
   for(p=0;p<13;p++){
      for(v=0;v<13;v++){
         s=v;
         for(n=0;n<4;n++){
            if(((carta_existe(e->mao[x],n,v))&&(contar(aux)<3))) aux=add_carta(aux,n,v);
         }
         if(contar(aux)==3) break;
         else aux=0;
      }
      if(contar(aux)==3){
        for(v=0;v<13;v++){
           for(n=0;n<4;n++){
              if(((carta_existe(e->mao[x],n,v))&&(contar(tmp)<2)&&(v!=s))) tmp=add_carta(tmp,n,v);
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
      }else{
        for(v=0;v<13;v++){
           s=v;
           for(n=0;n<4;n++){
              if(((carta_existe(e->mao[x],n,v))&&(contar(aux)<2))) aux=add_carta(aux,n,v);
           }
           if(contar(aux)==2) break;
           else aux=0;
        }
        if(contar(aux)==2){
          for(v=0;v<13;v++){
             for(n=0;n<4;n++){
                if(((carta_existe(e->mao[x],n,v))&&(contar(tmp)<3)&&(v!=s))) tmp=add_carta(tmp,n,v);
             }
             if(contar(tmp)==3) break;
             else tmp = 0;
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
      if((full_house(aux))&&((compara_full_house(aux,e->ultima_jogada))||(t<3))) break;
      else aux=0;
   }
   if((full_house(aux))&&((compara_full_house(aux,e->ultima_jogada))||(t<3))){
     e->ultima_jogada=aux;
     e->ultimo_jogador=x;
     e->jogada[x]=aux;
     e->passagem=0;
     rem_mao(aux,&e->mao[x]);
   }
}

/*
/brief  Esta função serve para o computador jogar flush
@param  e      o estado do jogo atual
@param  t      tipo de combinação de 5 cartas da mao do jogador anterior
*/

void jogar_flush(ESTADO *e, int t){
   int n,v;
   int x;
   MAO aux;
   x=e->jogador_atual;
   e->jogada[x]=0;    
   aux=0;
   for(n=0;n<4;n++){
      for(v=0;v<13;v++){
         if(((carta_existe(e->mao[x],n,v))&&(contar(aux)<5))) aux=add_carta(aux,n,v);
      }
      if((flush(aux))&&((compara_flush(aux,e->ultima_jogada))||(t<2))) break;
      else aux=0;
   }
   if((flush(aux))&&((compara_flush(aux,e->ultima_jogada))||(t<2))){
     e->ultima_jogada=aux;
     e->ultimo_jogador=x;
     e->jogada[x]=aux;
     e->passagem=0;
     rem_mao(aux,&e->mao[x]);
   }
}

/*
/brief  Esta função serve para o computador jogar straight
@param  e      o estado do jogo atual
@param  t      tipo de combinação de 5 cartas da mao do jogador anterior
*/

void jogar_straight(ESTADO *e, int t){
   int n,v,s;
   int x;
   MAO aux;
   MAO a;
   MAO m;
   int vals[13]={0};
   s=0;
   x=e->jogador_atual;
   aux=0;
   a=e->ultima_jogada;
   m=e->mao[x];
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
   if((straight(aux))&&((compara_straight(aux,e->ultima_jogada))||(t<1))){
     e->ultima_jogada=aux;
     e->ultimo_jogador=x;
     e->jogada[x]=aux;
     e->passagem=0;
     rem_mao(aux,&e->mao[x]);
   }
}

/*
/brief  Esta função serve para o computador jogar straight_flush
@param  e      o estado do jogo atual
@param  t      tipo de combinação de 5 cartas da mao do jogador anterior
*/

void jogar_straight_flush(ESTADO *e, int t){
   int n,v,s=0;
   int x=e->ultimo_jogador;
   MAO m=e->mao[x];
   MAO a=e->ultima_jogada;
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
   if((straight_flush(aux))&&((compara_straight_flush(aux,e->ultima_jogada))||(t<5))){
     e->ultima_jogada=aux;
     e->ultimo_jogador=x;
     e->jogada[x]=aux;
     e->passagem=0;
     rem_mao(aux,&e->mao[x]);
   }
}

/*
/brief  Permite ao computador que tenha o 3 de ouros jogar a melhor mao de inicio que contenha essa carta
@param  e      o estado do jogo atual
@param  m      numero do jogador que tem o 3 de outos
*/

void comp_inicio(ESTADO *e, int m){
    MAO aux=melhor_mao_inicio(e->mao[m]);
    e->jogada[m]=0;
    rem_mao(aux,&e->mao[m]);
    e->ultima_jogada=aux;
    e->ultimo_jogador=m;
    e->jogada[m]=aux;
    e->passagem=0;  
    e->jogador_atual++;
}

/*
/brief  Esta função é usada para quando se joga 1 carta.
@param  e      o estado do jogo atual
@param  v      valor da carta jogada anteriormente
@param  n      naipe da carta jogada anteriormente
@param  m      numero do jogador atual
*/


void comp_1_carta(ESTADO *e, int v, int n, int m){
       int z,w;
       MAO aux=0;
       e->jogada[m]=0;
       for(z=0;z<4;z++){
          for(w=0;w<13;w++){
             if(contar(aux)<1){
               if(carta_existe(e->mao[m],z,w)){
                 if((w==v)&&(z>n)){
                   e->mao[m]=rem_carta(e->mao[m],z,w);
                   aux=add_carta(aux,z,w);
                 }
                 if(w>v){
                   e->mao[m]=rem_carta(e->mao[m],z,w);
                   aux=add_carta(aux,z,w);
                 }
               }
             }
          }
       }
       if(contar(aux)==1){
         e->ultima_jogada=aux;
         e->ultimo_jogador=m;
         e->jogada[m]=aux;
         e->passagem=0;
       }else{
         e->passagem++;
       }
       e->jogador_atual++;
}

/*
/brief  Esta função é usada para quando se joga pares.
@param  e      o estado do jogo atual
@param  v      Valor da carta jogada anteriormente
@param  m      numero do jogador atual
*/

void comp_2_cartas(ESTADO *e, int v, int m){
    int z,w;
    MAO aux=0;
    e->jogada[m]=0;
    for(w=v;w<13;w++){
       for(z=0;z<4;z++){
          if(contar(aux)<2){
            if(carta_existe(e->mao[m],z,w)){
               aux=add_carta(aux,z,w);            
            }                    
         }
       }
       if((contar(aux)==2)&&(maior_comb(aux,e->ultima_jogada))&&(mao_valor_igual(aux))){
         rem_mao(aux,&e->mao[m]);
         e->ultima_jogada=aux;
         e->ultimo_jogador=m;
         e->jogada[m]=aux; 
         e->passagem=0;
         break;
       }
       aux=0;
    }
    if(e->jogada[m]==0){
      e->passagem++;
    }
    e->jogador_atual++;
}

/*
/brief  Esta função é usada para quando se joga trios.
@param  e      o estado do jogo atual
@param  v      Valor da carta jogada anteriormente
@param  m      numero do jogador atual
*/

void comp_3_cartas(ESTADO *e, int v, int m){
    int z,w;
    MAO aux=0;
    e->jogada[m]=0;
    for(w=v;w<13;w++){
       for(z=0;z<4;z++){
          if(contar(aux)<3){
            if(carta_existe(e->mao[m],z,w)){
               aux=add_carta(aux,z,w);            
            }                    
         }
       }
       if((contar(aux)==3)&&(maior_comb(aux,e->ultima_jogada))&&(mao_valor_igual(aux))){
         rem_mao(aux,&e->mao[m]);
         e->ultima_jogada=aux;
         e->ultimo_jogador=m;
         e->jogada[m]=aux;
         e->passagem=0;
         break;
       }
       aux=0;
    }
    if(e->jogada[m]==0){
      e->passagem++;
    }
    e->jogador_atual++;
}

/*
/brief  Função usada quando se joga combinações de 5 cartas.
/brief  Se houver uma combinacao de maior valor que a jogada anteriormente, esta será jogada.
/brief  Nota: A prioridade e straight flush, four of a kind, full house, flush e straight
@param  e      o estado do jogo atual
@param  t      tipo de combinação de 5 cartas da mao do jogador anterior
@param  m      numero do jogador atual
*/

void comp_5_cartas(ESTADO *e, int t, int m){
    e->jogada[m]=0;
    if(t==5){
      jogar_straight_flush(e,t);
      if(e->ultimo_jogador!=m) e->passagem++;
    }
    if(t==4){
      jogar_straight_flush(e,t);
      if(e->ultimo_jogador!=m) jogar_four_of_a_kind(e,t);
      if(e->ultimo_jogador!=m) e->passagem++;
    }
    if(t==3){
      jogar_straight_flush(e,t);
      if(e->ultimo_jogador!=m) jogar_four_of_a_kind(e,t);
      if(e->ultimo_jogador!=m) jogar_full_house(e,t);
      if(e->ultimo_jogador!=m) e->passagem++;
    }
    if(t==2){
      jogar_straight_flush(e,t);
      if(e->ultimo_jogador!=m) jogar_four_of_a_kind(e,t);
      if(e->ultimo_jogador!=m) jogar_full_house(e,t);
      if(e->ultimo_jogador!=m) jogar_flush(e,t);
      if(e->ultimo_jogador!=m) e->passagem++;
    }
    if(t==1){
      jogar_straight_flush(e,t);
      if(e->ultimo_jogador!=m) jogar_four_of_a_kind(e,t);
      if(e->ultimo_jogador!=m) jogar_full_house(e,t);
      if(e->ultimo_jogador!=m) jogar_flush(e,t);
      if(e->ultimo_jogador!=m) jogar_straight(e,t);
      if(e->ultimo_jogador!=m) e->passagem++;
    }
    e->jogador_atual++;
}

/*
/brief  Função que continua o jogo se um dos computadores ganhar 
@param  e      o estado do jogo atual
@param  x      numero do jogador atual
*/

void comp_auto(ESTADO *e, int x){
    MAO aux=melhor_mao(e->mao[x]);
    e->jogada[x]=0;
    e->ultima_jogada=0;
    rem_mao(aux,&e->mao[x]);
    e->ultima_jogada=aux;
    e->ultimo_jogador=x;
    e->jogada[x]=aux;
    e->jogador_atual=x+1;
    e->passagem=0;
}

/*
/biref  Esta função é a faz os computadores joguem.
@param  e      o estado do jogo atual
*/

void jogar_comp (ESTADO *e){
       int x;
       int m,n,v,c,t;
       x=e->jogador_atual;
       for(m=x;m<4;m++){     
          if((!vencedor(e->mao[0]))&&(!vencedor(e->mao[1]))&&(!vencedor(e->mao[2]))&&(!vencedor(e->mao[3]))){    
            v=carta_maior_valor(e->ultima_jogada);
            n=carta_maior_naipe(e->ultima_jogada);
            c=contar(e->ultima_jogada);
            t=tipo_comb_5(e->ultima_jogada);
            if((e->passagem)==3){            
              comp_auto(e,m);
            }else{
              if(c==1){
                comp_1_carta(e,v,n,m);
              }
              if(c==2){ 
                comp_2_cartas(e,v,m);
              } 
              if(c==3){
                comp_3_cartas(e,v,m);
              }
              if(c==5){
                comp_5_cartas(e,t,m);
              }
            }
          }else{
            if(vencedor(e->mao[0])){
              e->jogada[1]=0;
              e->jogada[2]=0;
              e->jogada[3]=0;
            }
            if(vencedor(e->mao[1])){
              e->jogada[2]=0;
              e->jogada[3]=0;
            }
            if(vencedor(e->mao[2])){
              e->jogada[3]=0;
            }
          }
       }
       e->jogador_atual=0;
}

/*
/brief  Função que determina qual o primeiro jogador.
@param  e      o estado do jogo atual
*/

void jogar_primeiro(ESTADO *e){
	int m;
	for(m=0;m<4;m++){
	   if((carta_existe(e->mao[m],0,0))&&(m!=0)){
             e->jogador_atual=m;
             comp_inicio(e,m);
             jogar_comp(e);  
          }else{
            if((carta_existe(e->mao[m],0,0))&&(m==0)) e->jogador_atual=m;
          }
        }
}

/*
/brief  Esta é a função que ocorre quando se carrega no botão jogar.
/brief  A primeira coisa que faz é remover as cartas que existem na seleção da mão do jogador e imprimilhas no ecrã.
/brief  Depois, faz com que a ultima jogada e a jogada[0] fiquem iguais à seleção, põe o ultimo jogador igual a 0, põe seleção igual a 0 e faz com que os computadores joguem.
@param  e      o estado do jogo atual
*/

void jogar(ESTADO *e){
    int n,v;
    e->jogar=0;
    e->jogador_atual=0;
    for(n=0;n<4;n++){
       for(v=0;v<13;v++){
          if(carta_existe(e->selecao,n,v)){
            e->mao[0]=rem_carta(e->mao[0],n,v);
          }
       } 
    }
    e->ultima_jogada=e->selecao;
    e->jogada[0]=e->selecao;
    e->ultimo_jogador=0;
    e->selecao=0;     
    e->jogador_atual=1;
    jogar_comp(e);
}

/*
/brief  Esta função corre quando se carrega no botão passar.
/brief  Põe o valor da seleção, do passar e do jogador atual a 0, e faz com que os computadores joguem. 
@param  e      o estado do jogo atual
*/

void passar (ESTADO *e){
    e->selecao=0;
    e->passar=0;    
    e->jogada[0]=0;
    e->jogador_atual=1;
    jogar_comp(e);
}

/*
/brief  Esta função corre quando se carrega no botao sugestao
/brief  Sugere uma mao ao jogador
@param  e      o estado do jogo atual
*/

void sugerir (ESTADO *e){
    MAO aux=melhor_mao_possivel(*e);
    int n,v;
    int x=680,y=125;
    if(carta_existe(e->mao[0],0,0)){
      aux=melhor_mao_inicio(e->mao[0]);
    }else{
      if(e->passagem==3){
        aux=melhor_mao(e->mao[0]);
      }else{
        aux=melhor_mao_possivel(*e);
      }
    }
    e->sugerir=0;
    for(n=0;n<4;n++){
       for(v=0;v<13;v++){
          if((carta_existe(aux,n,v))&&(aux!=0)){
            x+=20;
            imprime_jogada(x,y,n,v);
          }else{
            if(aux==0){
              printf("<svg height=900 height=900 >\n");
              printf("<rect x=\"%d\" y=\"%d\" width=60 height=50 ry=10 fill = \"rgb(255,120,0)\" />\n", x+70, y);
              printf("<text x=\"%d\" y=\"%d\" text-anchor = \"middle\" text-align = \"center\" font-family = \"serif\" font-weight = \"bold\"> Passar </text> \n", x+100, y+30);
              printf("</svg>\n");
            }
          }
       }
    }
}

/*
/brief  Verifica qual o estado de query, e atua dependendo disso.
/brief  Se a query não for nula e tiver mais do que 0 carateres,atua baseado se algo foi selecionado, jogado ou passado, começando as funções apropriadas.
/brief  Caso contrário, distribui e imprime(só acontece no inicio do jogo). 
@param  query  a variavel de ambiente
@param  e      o estado do jogo atual
*/

void parse(char *query, ESTADO *e) {
     if((query!=NULL)&&(strlen(query)!=0)){
       *e=strparaestado(query);
       if(e->selecionar){
         e->selecionar=0;
       }
       if(e->jogar){
         jogar(e);
       }
       if(e->passar){
         passar(e);
       }
       if(e->sugerir){
         sugerir(e);
       }
       if(e->novo_jogo){
         e->novo_jogo=0;
         distribuir_continuo(e);
         jogar_primeiro(e); 
       }       
       imprime(*e); 
     }else{
       *e=distribuir();
       jogar_primeiro(e);    
       imprime(*e);
     }
}

/*
/brief  Função principal
@return        0 se tudo correu devidamente
*/

int main() {
    ESTADO e;
    srandom(time(NULL));
    printf("Content-Type: text/html; charset=utf-8\n\n");
    printf("<header><title>Big Two</title></header>\n");
    printf("<body>\n");
    printf("<svg height = \"600\" width = \"900\">\n");
    imprime_base();
    parse(getenv("QUERY_STRING"),&e);
    printf("</svg>\n");
    printf("</body>\n");
    return 0;
}
