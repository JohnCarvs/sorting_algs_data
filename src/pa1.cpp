//---------------------------------------------------------------------
// Arquivo      : evalenum.c
// Conteudo     : avaliacao de desempenho de algoritmos de ordenacao 
// Autor        : Wagner Meira Jr. (meira@dcc.ufmg.br)
// Historico    : 2023-11-04 - arquivo criado
//---------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <time.h>
#include <fstream>
#include <iostream>
#include <chrono>
#include <ctime>
#include <cstdint>

#define ALGINSERTION 1
#define ALGSELECTION 2
#define ALGQSORT     3
#define ALGQSORT3    4
#define ALGQSORTINS  5
#define ALGQSORT3INS 6
#define ALGSHELLSORT 7
#define ALGRECSEL    8

std::ofstream initTextFile(std::string path){
  std::ofstream arquivo(path);

    // Verificar se o arquivo foi aberto corretamente
    if (arquivo.is_open()) {
        auto agora = std::chrono::system_clock::now();
        std::time_t tempo_atual = std::chrono::system_clock::to_time_t(agora);
        arquivo << "Date and time: " << std::ctime(&tempo_atual) << std::endl;
        
        return(arquivo);
        
        std::cout << "Texto foi escrito no arquivo com sucesso.\n";
    } else {
        std::cerr << "Não foi possível abrir o arquivo para escrita.\n";
    }
}

typedef struct alg{
  int num;
  char * name;
} alg_t;

alg_t algvet[]={
  {ALGINSERTION,"i"},
  {ALGSELECTION,"s"},
  {ALGQSORT,"q"},
  {ALGQSORT3,"q3"},
  {ALGQSORTINS,"qi"},
  {ALGQSORT3INS,"q3i"},
  {ALGSHELLSORT,"h"},
  {ALGRECSEL,"rs"},
  {0,0}
};

int name2num(char * name){
  int i=0;
  while (algvet[i].num){
    if (!strcmp(algvet[i].name,name)) return algvet[i].num;
    i++;
  }
  return 0;
}

char * num2name(int num){
  int i=0;
  while (algvet[i].num){
    if (algvet[i].num==num) return algvet[i].name;
    i++;
  }
  return 0;
}

typedef struct opt{
  int size;
  int seed;
  int alg;
  int type;
} opt_t;

typedef struct sortperf{
  int cmp;
  int move;
  int calls;
} sortperf_t;

void resetcounter(sortperf_t * s){
// Descricao: inicializa estrutura
// Entrada: 
// Saida: s
  s->cmp = 0;
  s->move = 0;
  s->calls = 0;
}

void inccmp(sortperf_t * s, int num){
// Descricao: incrementa o numero de comparacoes em num 
// Entrada: s, num
// Saida: s
  s->cmp += num;
}

void incmove(sortperf_t * s, int num){
// Descricao: incrementa o numero de movimentacoes de dados em num 
// Entrada: s, num
// Saida: s
  s->move += num;
}

void inccalls(sortperf_t * s, int num){
// Descricao: incrementa o numero de chamadas de função em num 
// Entrada: s, num
// Saida: s
  s->calls += num;
}

void printsortperf(sortperf_t * s){
// Descricao: gera string com valores de sortperf 
// Entrada: s, pref
// Saida: str
 printf("cmp %d move %d calls %d", s->cmp, s->move, s->calls); 
}

void initVector(int * vet, int size, int tipoDeVetor){
// Descricao: inicializa vet com valores aleatorios ou ordenados
// 0: Aleatório   1: Crescente  2: Decrescente
// Entrada: vet
// Saida: vet
  int i;
  // inicializa a parte alocada da vetor com valores aleatorios
  if(tipoDeVetor == 0){
    for (i=0; i<size; i++){
      vet[i] = (int)(drand48()*size);
    }
  }
  else if(tipoDeVetor == 1){
   for (i=0; i<size; i++){
      vet[i] = i;
   }
  }
  else if(tipoDeVetor == 2){
   for (i=0; i<size; i++){
      vet[i] = size-i;
   }  
  }
}


void printVector(int * vet, int size){
  int i;
  for (i=0; i<size; i++){
    printf("%d ",vet[i]);
  }
  printf("\n");
}

void swap(int *xp, int *yp, sortperf_t *s){
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
    incmove(s,3);
}

// shellsort
void shellSort(int *A, int n, sortperf_t * s) {
inccalls(s,1);
for (int h = n / 2; h > 0; h /= 2) {
    for (int i = h; i < n; i += 1) {
      inccmp(s,1);
      incmove(s,1);
      int temp = A[i];
      int j;

      for (j = i; j >= h && A[j-h] > temp; j-= h) {
       inccmp(s,1); 
       incmove(s,1);

        A[j] = A[j - h];
      }
      A[j] = temp;
      incmove(s,1);
    }
  }

}

//---------------------------------------------------------------------------
// INÍCIO: DEFINIÇÃO DOS ALGORÍTMOS 
//---------------------------------------------------------------------------

// recursive selection sort
void recursiveSelectionSort(int arr[], int l, int r, sortperf_t * s)
{
    // find the minimum element in the unsorted subarray `[i…n-1]`
    // and swap it with `arr[i]`
    int min = l;
    inccalls(s,1);
    for (int j = l + 1; j <= r; j++)
    {
        // if `arr[j]` is less, then it is the new minimum
	inccmp(s,1);
        if (arr[j] < arr[min]) {
            min = j;    // update the index of minimum element 
        }
    }

    // swap the minimum element in subarray `arr[i…n-1]` with `arr[i]`
    if (min!=l)
      swap(&arr[min], &arr[l], s);

    if (l + 1 < r) {
        recursiveSelectionSort(arr, l + 1, r, s);
    }
      
}

// selection sort
void selectionSort(int arr[], int l, int r, sortperf_t * s) { 
//int i, j, Min;
inccalls(s,1);
//inccmp(s,3);
for (int i = l; i < r ; i++)
{
  int Min = i;
  for (int j = i+1 ; j < r+1; j++)
  {
  
    inccmp(s,1);
    if (arr[j] < arr[Min])
    Min = j;
  }
if(Min != i)
  
   swap(&arr[i], &arr[Min], s);
}
}

//insertion sort
void insertionSort(int v[], int l, int r, sortperf_t * s) {
    inccalls(s,1);
    int aux;
    int i,k;
    //Loop que itera todos os valores do vetor
    for(i = l + 1; i < r + 1 ; i++){
      incmove(s,1);
      aux= v[i]; 
      k= i - 1;
        inccmp(s,1);
      //Loop pra achar menor posicao
      while(k>= l && (aux< v[k])){
          inccmp(s,1);
          incmove(s,1);
          v[k + 1]= v[k];
          k--;
      }

      incmove(s,1);
      v[k+1] = aux;
    }

}

// median of 3 integers
int median (int a, int b, int c) {
    if ((a <= b) && (b <= c)) return b;  // a b c
    if ((a <= c) && (c <= b)) return c;  // a c b
    if ((b <= a) && (a <= c)) return a;  // b a c
    if ((b <= c) && (c <= a)) return c;  // b c a
    if ((c <= a) && (a <= b)) return a;  // c a b
    return b;                            // c b a
}

// quicksort partition using median of 3
void partition3(int * A, int l, int r, int *i, int *j, sortperf_t *s) {
  inccalls(s,1);
  int x, w;
*i = l;
*j = r; 
x = median(A[l],A[(l + r)/2], A[r]); /* obtem o pivo x */
do
{
  inccmp(s,1);
  while (x > A[*i]) {inccmp(s,1);(*i)++;}
  inccmp(s,1);
  while (x < A[*j]) {inccmp(s,1);(*j)--;}
  if (*i <= *j)
  {
    swap(&A[*i], &A[*j], s);
    (*i)++; (*j)--;
  }
} while (*i <= *j);
//printf("%i    %i    %i\n",A[x],*i,*j);
}

// standard quicksort partition
void partition(int * A, int l, int r, int *i, int *j, sortperf_t *s) {
inccalls(s,1);
  int x, w;
*i = l;
*j = r; 
x = A[(l + r)/2]; /* obtem o pivo x */
do
{
  inccmp(s,1);
  while (x > A[*i]) {inccmp(s,1);(*i)++;}
  inccmp(s,1);
  while (x < A[*j]) {inccmp(s,1);(*j)--;}
  if (*i <= *j)
  {
    swap(&A[*i], &A[*j], s);
    (*i)++; (*j)--;
  }
} while (*i <= *j);
//printf("%i    %i    %i\n",A[x],*i,*j);
}

// standard quicksort
void quickSort(int * A, int l, int r, sortperf_t *s) { 
  inccalls(s,1);
  int i;
  int j;
  partition(A,l,r,&i,&j,s);
  if (l < j) {quickSort(A, l, j, s);}
  if (i <r) {quickSort(A,i, r, s);}
}

// quicksort with median of 3
void quickSort3(int * A, int l, int r, sortperf_t *s) { 
   inccalls(s,1);
  int i;
  int j;
  partition3(A,l,r,&i,&j,s);
  if (l < j) {quickSort3(A, l, j, s);}
  if (i <r) {quickSort3(A,i, r, s);}
}

// quicksort with insertion for small partitions
void quickSortIns(int * A, int l, int r, sortperf_t *s) { 
   inccalls(s,1);
  int i;
  int j;
  partition(A,l,r,&i,&j,s);
  if (l<j) {
    if(j-l >50) quickSortIns(A, l, j,s);
    else insertionSort(A,l,j,s);
    }
  
  if (i<r) {
    if(r-i >50) quickSortIns(A, i, r,s);
    else insertionSort(A,i,r,s);
    }
}

// quicksort with insertion for small partitions and median of 3
void quickSort3Ins(int * A, int l, int r, sortperf_t *s) { 
  inccalls(s,1);
  int i;
  int j;
  partition3(A,l,r,&i,&j,s);
  if (l<j) {
    if(j-l >50) quickSort3Ins(A, l, j,s);
    else insertionSort(A,l,j,s);
    }
  
  if (i<r) {
    if(r-i >50) quickSort3Ins(A, i, r,s);
    else insertionSort(A,i,r,s);
    }
}

//---------------------------------------------------------------------------
// FIM: DEFINIÇÃO DOS ALGORÍTMOS 
//---------------------------------------------------------------------------


void uso()
// Descricao: imprime as opcoes de uso
// Entrada: nao tem
// Saida: impressao das opcoes de linha de comando
{
  fprintf(stderr,"sortperf\n");
  fprintf(stderr,"\t-z <int>\t(vector size)\n");
  fprintf(stderr,"\t-s <int>\t(initialization seed)\n");
  fprintf(stderr,"\t-a <s|i|q|q3|qi|q3i|h|rs>\t(algorithm)\n");
  fprintf(stderr,"\t    s\tselection\n");
  fprintf(stderr,"\t    i\tinsertion\n");
  fprintf(stderr,"\t    q\tquicksort\n");
  fprintf(stderr,"\t    q3\tquicksort+median3\n");
  fprintf(stderr,"\t    qi\tquicksort+insertion\n");
  fprintf(stderr,"\t    q3i\tquicksort+median3+insertion\n");
  fprintf(stderr,"\t    h\theapsort\n");
  fprintf(stderr,"\t    rs\trecursive selection\n");
}

void parse_args(int argc, char ** argv, opt_t * opt)
// Descricao: le as opcoes da linha de comando e inicializa variaveis
// Entrada: argc, argv, opt
// Saida: opt
{
     // variaveis externas do getopt
     extern char * optarg;
     extern int optind;

     // variavel auxiliar
     int c;

     // inicializacao variaveis globais para opcoes
     opt->seed = 1;
     opt->size = 10;
     opt->alg = 1;
     opt->type =0;

     // getopt - letra indica a opcao, : junto a letra indica parametro
     // no caso de escolher mais de uma operacao, vale a ultima
     while ((c = getopt(argc, argv, "z:s:a:t:h")) != EOF){
       switch(c) {
         case 'z':
	          opt->size = atoi(optarg);
                  break;
         case 's':
	          opt->seed = atoi(optarg);
                  break;
         case 'a':
		  opt->alg = name2num(optarg);
                  break;
         case 't':
		  opt->type = atoi(optarg);
                  break;
         case 'h':
         default:
                  uso();
                  exit(1);

       }
     }
     if (!opt->alg) {
       uso();
       exit(1);
     }
}

void init_args_name_Alg(int seed, int size, char * alg, int type, opt_t * opt)
// Descricao: inicia as variáveis de acorodo com as entradas
// Entrada: seed, size, alg, argc, argv, opt
// Saida: opt
{
     // inicializacao variaveis globais para opcoes
     opt->seed = seed;
     opt->size = size;
     opt->alg = name2num(alg);
     opt->type = type;

}

void init_args_num_Alg(int seed, int size, int alg, int type, opt_t * opt)
// Descricao: inicia as variáveis de acorodo com as entradas
// Entrada: seed, size, alg, argc, argv, opt
// Saida: opt
{
     // inicializacao variaveis globais para opcoes
     opt->seed = seed;
     opt->size = size;
     opt->alg = alg;
     opt->type = type;

}

void clkDiff(struct timespec t1, struct timespec t2,
                   struct timespec * res)
// Descricao: calcula a diferenca entre t2 e t1, que e armazenada em res
// Entrada: t1, t2
// Saida: res
{
  if (t2.tv_nsec < t1.tv_nsec){
    // ajuste necessario, utilizando um segundo de tv_sec
    res-> tv_nsec = 1000000000+t2.tv_nsec-t1.tv_nsec;
    res-> tv_sec = t2.tv_sec-t1.tv_sec-1;
  } else {
    // nao e necessario ajuste
    res-> tv_nsec = t2.tv_nsec-t1.tv_nsec;
    res-> tv_sec = t2.tv_sec-t1.tv_sec;
  }
}
void execAlg(opt_t opt, int * vet, sortperf_t * s){
  switch (opt.alg){
    case ALGINSERTION:
         insertionSort(vet, 0, opt.size-1, s);
         break;
    case ALGSELECTION:
         selectionSort(vet, 0, opt.size-1, s);
         break;
    case ALGQSORT:
         quickSort(vet, 0, opt.size-1, s);
         break;
    case ALGQSORT3:
         quickSort3(vet, 0, opt.size-1, s);
         break;
    case ALGQSORTINS:
         quickSortIns(vet, 0, opt.size-1, s);
         break;
    case ALGQSORT3INS:
         quickSort3Ins(vet, 0, opt.size-1, s);
         break;
    case ALGSHELLSORT:
         shellSort(vet, opt.size, s);
         break;
    case ALGRECSEL:
         recursiveSelectionSort(vet, 0, opt.size-1, s);
         break;
  }
   }

int main (int argc, char ** argv){
  
  //Inicializa o arquivo de texto
  std::ofstream arquivo = initTextFile("outputFiles/algs_execution_data.txt");

  int * vet;
  char pref[100];
  opt_t opt;
  struct timespec inittp, endtp, restp;
  int retp;
 sortperf_t s;
  



  // loop principal XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX
  for(int alg=1 ; alg<=8 ; alg++){
    arquivo << "alg " << num2name(alg) << std::endl<<std::endl;
    arquivo << "SEED ,  SIZE ,  ALG ,  TYPE ,  CALLS ,  CMP ,     MOVE ,    TIME              ,"<< std::endl;
    
    for(int type=0 ; type<=2 ; type ++){
      arquivo << "TYPE"<<type<< std::endl;

      for(int size=5 ; size<=4000 ; size=size+40){
          //opt inicialização
          int seed = 27;
          init_args_num_Alg(seed,size,alg,type,&opt);

          vet = (int *) malloc((opt.size+1)*sizeof(int));

          // initialize
          resetcounter(&s);
          srand48(opt.seed);
          initVector(vet, opt.size, opt.type);
          retp = clock_gettime(CLOCK_MONOTONIC, &inittp);
          //vet[opt.size] = vet[0]; // for heapsort
          //if (opt.size < 100) printVector(vet, opt.size);opt.type

          
          execAlg(opt, vet, &s);

            retp = clock_gettime(CLOCK_MONOTONIC, &endtp);
          clkDiff(inittp, endtp, &restp);

          /*
          arquivo << "SEED ,  SIZE ,  ALG ,  TYPE " << std::endl;
          arquivo << seed<< " ,    " << size<<" ,     "<< alg << " ,    "<< type<< std::endl;
          arquivo << "CALLS ,  CMP ,  MOVE ,  TIME " << std::endl;
          arquivo << s.calls<< " ,     " << s.cmp <<" ,    "<< s.move << " ,    "<<restp.tv_nsec/100.0<< std::endl << std::endl;
          */

          
          arquivo << seed<< " ,    " << size<<" ,     "<< num2name(alg) << " ,    "<< type <<" ,     "<< s.calls<< " ,     " << s.cmp <<" ,    "<< s.move << " ,    "<<restp.tv_nsec/100<< "         ,"<< std::endl;

          
          //Printar o vetor depois da ordenação

          /*printf("alg %s seed %d size %d time %ld.%.2ld",
               num2name(opt.alg), opt.seed, opt.size, restp.tv_sec,restp.tv_nsec);
          std::cout<<std::endl;
          printsortperf(&s);
          std::cout<<std::endl;
          */
      }

   }
    arquivo << std::endl << std::endl;
  }
 

  // print stats
  std::cout << "Fim da execução";
  
  arquivo.close();
  exit(0);
}
