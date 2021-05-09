//feito por:
//Felipe Negrelli Wolter, RA: 1607766
//Luan Carlos Klein, RA: 2022613

#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
#include <algorithm>
#include <time.h>
#include <iomanip>
#include <iterator>
using namespace std;

# define PI           3.14159265358979323846

//declaração da classe vértice (para evitar erros na definição da classe Aresta)
//classe Vertice definida e explicada posteriormente
class Vertice;
int a;


//Classe que representa um ponto 2D, que representa cada entrada do algoritmo
//armazena as coordenadas de um ponto dado no txt de entrada (input.txt)
//é usado dentro dos vértices para armazenar sua posĩção e, posteriormente,
//ser utilizado no cálculo do peso de cada aresta do grafo
class Ponto
{
  public:
    //coordenadas x e y do ponto
    float x;
    float y;
    //construtora da classe, que atribui as coordenadas
    //complexidade O(1)
    Ponto(float x, float y)
    {
        this->x = x;
        this->y = y;
    };
};

//Classe representa uma aresta direcional do grafo
//armazena dois vértices adjacentes e a distancia entre eles.
//é usado dentro do grafo (através dos vértices) e em todas as operações realizadas sobre ele, como a determinação da AGM pelo algoritmo de Prim e na busca em profundidade
class Aresta {   
  public: 
        //vertice de onde a aresta sai
        Vertice* origem;
        //vertice para o qual a aresta vai
        Vertice* destino;
        //custo da aresta (a distancia euclidiana entre os vertices)
        float custo;
};

//classe que representa um vertice do grafo
//armazena o ponto por ele representado, suas adjacencias e informações utilizadas pela heap de pelo do algoritmo de Prim
class Vertice 
{
  public:
    //-------------------------------------------------------------------
    //atributos de identificação do vertice e do ponto
    //identificador da posição em que ele está no arquivo de entrada
    int id;
    //posição do ponto que ele representa
    Ponto* ponto;
    //-------------------------------------------------------------------    
    //atributo usado pelo grafo por lista de adjacencias
    //lista de arestas do vertice
    vector<Aresta> adjacencias;
    //-------------------------------------------------------------------    
    //atributo utilizado para representação por arvore e busca em profundidade
    //lista de filhos  (na etapa 2, quando será tratado como nó de árvore)
    vector<Vertice*> filhos;
    //angulo que será usado para a ordenação das visitas na busca em profundidade
    float angulo;
    //-------------------------------------------------------------------    
    // os atributos a seguir sao usados pela heap de Fibonacci
    //vertice localizado a sua esquerda na lista encadeada que representa seu nivel na sua arvore
    Vertice* fibo_irmao_esquerda;
    //vertice localizado a sua esquerda na lista encadeada que representa seu nivel na sua arvore
    Vertice* fibo_irmao_direita;
    //aponta para o seu pai na arvore. nulo de for uma raiz
    Vertice* fibo_pai;
    //vertice que aponta para o seu filho mais a esquerda
    Vertice* fibo_filhos;
    //indica o numero de filhos do vertice
    int fibo_grau;
    //indica se teme um filho removido. caso tenha mais de um, sobe na arvore
    int marcado;
    //Armazena o custo da menor aresta de corte que chega nele em uma dada iteração do algoritmo de prim
    //é o peso usado pela heap
    float custo;
    //usado para evitar repetições na impressão da heap
    int aberto;
    //-------------------------------------------------------------------    
    // os atributos a seguir sao usados para o algoritmo de PRIM
    //O vertice pai do vertice na AGM gerada
    Vertice* pai;
    // Representa a posicao daquele vertice na heap
    // Tem duas funcoes: Auxilia no heapify, e quando sair do heap, tem valor negativo, que facilita a verificao para ver se um vertice esta no heap ou nao 
    int posHeap;
    //------------------------------------------------------------------

    //construtora da classe
    //complexidade O(1)
    Vertice (float x, float y, int id)
    {
      //
      this->ponto = new Ponto(x, y);
      this->id = id;

    };
    //desaloca as adjacencias, evita memory leaks
    //complexidade O(1)
    ~Vertice ()
    {

    };
    //adiciona no vetor de adjacencias uma aresta nova
    //complexidade O(1)
    //Entrada: vertice com o qual deseja-se adicionar a aresta e a distância euclidiana entre os dois
    //Saida: nova aresta inserida na lista de adjacencias do vértice
    void adicionarAresta(Vertice* destino, float distancia)
    {
      //cria nova aresta
      Aresta aresta = Aresta ();
      //seta o vertice atual como origem da aresta direcional
      aresta.origem = this;
      //seta o grafo passado por referencia como destino da aresta
      aresta.destino = destino;
      //seta o peso da aresta
      aresta.custo = distancia;
      //insere-a na lista de adjacencias do vertice
      adjacencias.push_back(aresta);
    };

    void adicionarFilho(Vertice* filho)
    {
      filhos.push_back(filho);
    };

    // Seta o custo
    //complexidade O(1)
    void setCusto(float _custo)
    {
        this->custo = _custo;
    }

    //Seta o pai
    //complexidade O(1)
    void setPai(Vertice* _pai)
    {
      this->pai = _pai;
    }

    // Seta a posicao no heap
    //complexidade O(1)
    void setPosHeap(int _pos)
    {
      this->posHeap = _pos;
    }

};

//classe que representa um grafo por lista de adjacencias
//armazena a lista de vértices que representam as entradas do problema
//É usado para representar o grafo completo (etapa 1), a AGM (etapa 2) e o ciclo (etapa 3)
class Grafo
{
  public:
    //guarda um vetor de vertices
    //as arestas são armazenadas dentro do proprio vertice
    vector<Vertice*> vertices;
    //construtor da classe
    //complexidade O(1)
    Grafo ()
    {

    };
    //destrutor da classe, desaloca os vertices
    //complexidade O(n+m)
    ~Grafo ()
    {
      for (int i = 0; i<vertices.size(); i++)
        {
          delete vertices[i];
        }
    };
    //insere um novo vertice no final do vetor
    //complexidade O(1)
    void adicionarVertice(Vertice* vertice)
    {
        vertices.push_back (vertice);
    };
};


// Classe Heap de fibonacci utilizada para o algoritmo de prim
//feita seguindo o pseudocódigo do capítulo 17 do Cormen
//implementação de um heap minimo de fibonacci
//armazena uma lista circular de arvores (com os irmãos sendo implementados como uma lista duplamente encadada)
//e onde os filhos também conhecem seus pais. Cada árvore obedece as propriedades de uma heap
//a heap apresenta um ponteiro para a rais de menor custo
//utilizado para seleção do proximo vertice a ser inserido na arvore no algoritmo de Prim
class Heap_Fibonacci
{

  public:
  	//vetor auxiliar usado na consolidação
    vector<Vertice*> A;
    //ponteiro par o vertice de menor valor
    //serve tambem para apontar para a lista circular de raizes
    Vertice* minimo;
    //numero de elementos da heap
    int tamanho;

// Construtora
    //complexidade O(1)
  Heap_Fibonacci()
  {
    minimo = NULL;
    tamanho = 0;
  };

//Destrutora
    ~Heap_Fibonacci()
    //complexidade O(1) pois a heap estará sempre vazia na finalização do algoritmo de Prim
    {

    };


// Inserir elemento no heap
    //ENTRADA: um vertice que deseja-se inserir na heap (elemento)
    //SAIDA: heap com o novo vértice e com suas propriedades mantidas
    //complexidade O(1)
  void insere(Vertice* elemento)
  {  	
//-------------------------------------------------------------------
  	//seta como nulo todos os parametros do vertice relacionados a heap 
    elemento->fibo_grau =0;
    elemento->fibo_pai = NULL;
    elemento->fibo_filhos = NULL;
    elemento->marcado = 0;
    elemento->aberto = 0;
//-------------------------------------------------------------------
    //se for o primeiro elemento inserido
    if(minimo == NULL)
    {
    	//seta ele como minimo
      minimo = elemento;
      //insere ele na lista circular de raizes (anteriormente vazia)
      elemento->fibo_irmao_esquerda = elemento;
      elemento->fibo_irmao_direita = elemento;
    }
//-------------------------------------------------------------------
    //se ja houverem elementos na lista
    else
    {
    //insere ele à esquerda do minimo na lista circular
      elemento->fibo_irmao_direita = minimo;
      minimo->fibo_irmao_esquerda->fibo_irmao_direita = elemento;
      elemento->fibo_irmao_esquerda = minimo->fibo_irmao_esquerda;
      minimo->fibo_irmao_esquerda = elemento;
      //se for menor que o mínimo, o seta como minimo
      if (elemento->custo < minimo->custo)
      {
        minimo = elemento;
      }
    }
//-------------------------------------------------------------------
    //incrementa o tamanho da heap
    tamanho ++;
    //indica que o elemento está na heap
    elemento->posHeap = 1;

  }

// Funcao para remover o menor elemento da heap
    //ENTRADA: apenas a heap
    //SAIDA: heap com suas propriedades mantidas e com o vertice de menor custo removido
    //complexidade O(log(n))
  Vertice* remove()
  {
    Vertice* removido = minimo;
    //indica que o elemento indicado pelo minimo não esta mais na heap
    removido->posHeap = -1;
    //verifica se a heap nãoestá vazia
    if (removido != NULL)
    {
    	//se ele tiver filhos, passa eles para a raiz
      if (removido ->fibo_filhos !=NULL)
      {
        Vertice* atual = removido->fibo_filhos;
        //laço repetido O(log(n)) vezes ???
        do 
        {
        	//define os filhos como raizes
          atual->fibo_pai = NULL;
          removido->fibo_grau = 0;
          atual = atual->fibo_irmao_direita;
        }while(atual!=removido->fibo_filhos);
      }

      //verifica se era a unica raiz
      if (removido->fibo_irmao_direita == removido)
      {
      	//se era, o minimo passa a ser um de seus filhos, caso tivesse
        if(removido->fibo_filhos != NULL)
        {
          minimo = removido->fibo_filhos;
          //e consolida para manter as propriedades de heap de fibonacci
          //complexidade amortizada de O(log(n))
          consolidar();
        }
        //se não, a arvore estara vazia
        else
        {
          minimo = NULL;
        }
      }
      //se y não for a unica raiz antes do inicio da operação
      else
      {
      	//e tiver filhos ao inicio da operação
        if(removido->fibo_filhos!= NULL)
        {
          //insere seus filhos na lista circular em seu lugar
          removido->fibo_filhos->fibo_irmao_esquerda->fibo_irmao_direita = removido->fibo_irmao_direita;
          removido->fibo_irmao_esquerda->fibo_irmao_direita = removido->fibo_filhos;
          removido->fibo_irmao_direita->fibo_irmao_esquerda = removido->fibo_filhos->fibo_irmao_esquerda;
          removido->fibo_filhos->fibo_irmao_esquerda = removido->fibo_irmao_esquerda;      
        }
        else
        {
        	//se não tiver filhos, apenas é removido da lista
          removido->fibo_irmao_direita->fibo_irmao_esquerda = removido->fibo_irmao_esquerda;
          removido->fibo_irmao_esquerda->fibo_irmao_direita = removido->fibo_irmao_direita;
        }
        //faz o ponteiro do minimo apontar para uma raiz qualquer
        //o minimo será atualizado na consolidação, logo não faz diferença 
        //ser realmente o menor valor ou não
        minimo = removido->fibo_irmao_direita;
        //chama o método para restaurar as propriedades na heap de fibonacci
        consolidar();
      }
      //decrementa o tamanho da heap de fibonacci
      tamanho --;
    }
    //retorna o menor elemento
    return removido;

  }


// Responsável por manter as propriedades da heap de fibonacci após uma remoção
    //ENTRADA: apenas a heap
    //SAIDA: heap de fibonacci com suas propriedades
    //complexidade O(log(n))
  void consolidar()
  {
  	//cria um vetor auxiliar que terá no máximo log1.618(n) posições usadas.
  	//foi utilizado 1.5 para dar uma folga.
    int tam = (int)(log(tamanho)/log(1.5)+1);
    //inicia o vetor auxiliar com todas as posições nulas
    for(int i = 0; i < tam; i++)
    {
      A.push_back(NULL);
    }
    Vertice* atual = minimo;
    //loop que busca não deixar raizes de mesmo grau
    //vai percorrer todas as raizes
    while (atual != NULL)
    {
      //se ouver outra raiz com o mesmo grau do vertice atual na lista auxiliar
      while (A[atual->fibo_grau] !=NULL)
      {
        //vai juntar as duas arvores, mantendo como raiz a que possuir o menor custo
        Vertice* novo = A[atual->fibo_grau];
        //condicional para deixar a raiz de menor custo como raiz da nova arvore
        if (atual->custo>novo->custo)
        {
          novo = atual;
          atual = A[atual->fibo_grau];
          //condição para decidir corretamente o nó a ser setado como atual após encerrado o loop
          if (novo->fibo_irmao_direita != novo)
          {
			atual->fibo_irmao_direita = novo->fibo_irmao_direita;
			atual->fibo_irmao_esquerda = novo->fibo_irmao_esquerda;
          }
          else
          {
          	//garante que o laço se encerre caso seja o ultimo elemento e haja a troca
          	atual->fibo_irmao_direita = atual;
          	atual->fibo_irmao_esquerda = atual;
          }
        }
        //indica que não ha mais nenhum vertice no vetor auxiliar com grau do vertice atual
        A[atual->fibo_grau] = NULL;
        //une as duas arvores
        //O(1)
        linkar (novo, atual);
      }
      //se o loop se quebrou, significa que não ha nenhum outro vertice de mesmo grau na lista
      //logo, a arvore atual sera inserida nela, aguardando para formar um novo par
      A[atual->fibo_grau] = atual;
      //vai encerrar o loop caso seja o ultimo elemento
      if (atual->fibo_irmao_direita == atual)
      {
        atual = NULL;
      }
      //se restares arvores a analisar, remove o nó atual da lista de raizes para que não seja repetido
      else
      {
        atual->fibo_irmao_direita->fibo_irmao_esquerda = atual->fibo_irmao_esquerda;
        atual->fibo_irmao_esquerda->fibo_irmao_direita = atual->fibo_irmao_direita;
        Vertice* aux = atual;
        //e indica a proxima arvore a ser analisada
        atual = atual->fibo_irmao_direita;
        aux->fibo_irmao_esquerda = aux;
        aux->fibo_irmao_direita = aux;
      }
    }
    //se todos ja tiverem sido visitados, esvazia a heap
    minimo = NULL;
    for (int i = 0; i<A.size();i++)
    {
      if (A[i]!=NULL)
      {
      	//e vai adicionando na heap as arvores que estão no vetor auxiliar
        if (minimo == NULL)
        {
        	//caso seja o primeiro
          minimo = A[i];
          A[i]->fibo_irmao_esquerda = A[i];
          A[i]->fibo_irmao_direita = A[i];
        }
        else
        {
         //se ja houverem elementos na lista
          A[i]->fibo_irmao_direita = minimo;
          A[i]->fibo_irmao_esquerda = minimo->fibo_irmao_esquerda;
          minimo->fibo_irmao_esquerda->fibo_irmao_direita = A[i];
          minimo->fibo_irmao_esquerda = A[i];
          //verifica se é o custo é menor que o do menor elemento que ja estava na lista
          if (A[i]->custo < minimo->custo)
          {
          	//se for, seta ele como o minimo
            minimo = A[i];
          }
        }
      }
    }
    //esvazia o vetor
    A.clear();

  }

// Responsável por manter tornar um vértice y como filho do vértice x
    //ENTRADA: vertice que tera seu grau aumentado (x) e vertice que se tornara filho de x (y)
    //SAIDA: y como filho de x
    //complexidade O(1)
  void linkar(Vertice* y, Vertice* x)
  {
  	//se x não tinha filhos anteriormente
    if (x->fibo_filhos != NULL)
    {
      y->fibo_irmao_esquerda= x->fibo_filhos->fibo_irmao_esquerda;
      y->fibo_irmao_direita= x->fibo_filhos;
      x->fibo_filhos->fibo_irmao_esquerda->fibo_irmao_direita = y;
      x->fibo_filhos->fibo_irmao_esquerda = y;
    }
    //se x ja possuia filhos
    else
    {
      y->fibo_irmao_esquerda= y;
      y->fibo_irmao_direita= y;
    }
    x->fibo_filhos = y;
    y->fibo_pai = x;
    //desmarca y
    y->marcado = 0;
    //aumenta o grau de x
    x->fibo_grau ++;
  }

  //atualiza a chave (custo) de um vertice
    //ENTRADA: um vertice cuja chave deve ser atualizada e o custo que deseja-se atribuir a ele
    //SAIDA: heap com suas propriedades mantidas e
    //com a chave do vertice atualizado caso seja menor que a anterior
    //complexidade amortizada O(1)
  void atualizarChave(Vertice* vertice, float custo)
  {
  	//verifica se a chave é menor que a anterior
  	//só atualiza se for menor
    if (custo < vertice->custo)
    {
    //atualiza o custo
      vertice->custo = custo;
      Vertice* y = vertice->fibo_pai;
      //se violar a propriedade da heap
      if (y!= NULL && vertice->custo<y->custo)
      {
      	//torna o vertice uma raiz
        corte (vertice, y);
        //chama o corte em cascata
        //responsavel por manter o grau maximo de uma arvore de n elementos em log(n)
        //e assim, manter o custo amortizado em O(1)
        corteEmCascata (y);
      }
      //verifica se o novo custo é menor que o do menor minimo.
      //se for, seta ele como minimo
      if (vertice->custo < minimo->custo)
      {
        minimo = vertice;
      }
    }
  }

  //seta um vertice como raiz e elimina sua relação com o seu pai (que trágico).
  //Utilizada pela função de atualizar chave
    //ENTRADA: um vertice filho (x) e seu pai (y)
    //SAIDA: filho como uma raiz e sem ligações com seu pai. Se a chave for menor que a minima,
    //torna o vértice x como minimo
    //com a chave do vertice atualizado caso seja menor que a anterior
    //complexidade amortizada O(1)
  void corte(Vertice* x, Vertice* y)
  {
  	//se x for o unico filho de y, esvazia a lista de filhos de y
    if (x->fibo_irmao_direita == x)
    {
      y->fibo_filhos = NULL;
    }
    //se houverem mais filhos, remove x da lista de filhos
    else
    {
      x->fibo_irmao_esquerda->fibo_irmao_direita = x->fibo_irmao_direita;
      x->fibo_irmao_direita->fibo_irmao_esquerda = x->fibo_irmao_esquerda;
      if (y->fibo_filhos == x)
        y->fibo_filhos = x->fibo_irmao_direita;
    }
    //como y perdeu um filho, seu grau é decrementado
    y->fibo_grau-- ;
    //torna x uma raiz
    x->fibo_pai = NULL;
    //como x se torna uma raiz, será desmarcado
    x->marcado = 0;
    //adiciona x na lista de raizes
    x->fibo_irmao_direita = minimo;
    minimo->fibo_irmao_esquerda->fibo_irmao_direita = x;
    x->fibo_irmao_esquerda = minimo->fibo_irmao_esquerda;
    minimo->fibo_irmao_esquerda = x;
  }
  //responsavel por manter o grau maximo de uma arvore de n elementos em log(n)
  //e assim, manter o custo amortizado da atualização de chave em O(1)
  //quando é feita a primeira remoção (filho com chave atualizada vira raiz)
  //de um pai, ele é marcado.
  //Na segunda remoção, sobe na arvore setando os nós como raiz
  //recursivamente até encontrar um nó não marcado e para
  //Utilizada pela função de atualizar chave
    //ENTRADA: um vertice y
    //SAIDA: se y não estiver marcado, o marca. Se estiver, a saida é 
    //y e seus antecessores marcados (até o primeiro não marcado) setados como raiz
    //complexidade amortizada O(1)
  void corteEmCascata(Vertice* y)
  {
  	//verifica se y não é uma raiz
    Vertice* z = y->fibo_pai;
    if (z!= NULL)
    {
    	//se não for e estiver desmarcado
    	//significa que é a primeira remoção
      if (y->marcado == 0)
      {
      	//por eisso, y é apenas marcado
        y->marcado = 1;
      }
      //se for a segunda remoção
      else
      {
      	//torna y uma raiz
        corte (y, z);
        //repete o mesmo procedimento recursivamente
        //até encontrar um nó desmarcado ou uma raiz
        corteEmCascata(z);
      }
    }
  }
  void imprime(Vertice* y)
  {
    Vertice* x = y;
    int i = 0;
    do
    {
      cout<<x->id<<"(";
      if (x->aberto)
      {
        i = 30;
        break;
      }
      x->aberto = 1;
      if(x->fibo_filhos != NULL)
        imprime(x->fibo_filhos);
      cout<<")";
      x->aberto = 0;
      x = x->fibo_irmao_direita;
      i++;
    }while(x != y && i<30);
    if (i == 30)
    {
      cout<<"erro";
      cin>>a;
    }
  }
};


//-------------------------------------------------------------------
//Calcula a distancia euclidiana entre dois pontos
//ENTRADA: dois pontos 2D (a e b)
//SAIDA: distancia euclidiana entre eles 
//complexidade O(1), pois são apenas operações aritmeticas
float distancia (Ponto* a, Ponto* b);
//-------------------------------------------------------------------
//Le o arquivo de entrada
//Inicializa um grafo completo
//ENTRADA: arquivo .txt com as coordenadas dos pontos
//SAIDA: grafo completo, onde cada vértice corresponde a um ponto da entrada
//complexidade O(n^2)
Grafo* criarGrafo (char* nome);
//-------------------------------------------------------------------
//Imprime o ciclo
//ENTRADA: grafo com um ciclo hamiltoniano
//SAIDA: arquivo de texto no formato mostrado no pdf de descrição do trabalho para plot do ciclo 
//complexidade O(n) 
void imprimeArvore (Grafo* grafo);
//-------------------------------------------------------------------
//função para impressão recursiva dos vertices e arestas
//ENTRADA: vértice e ponteiro para arquivo de saida
//SAIDA: vertice atual e os subsequentes a ele escritos no arquivo de saida
//complexidade O(n) 
void imprimeCiclo (Grafo* grafo);
//-------------------------------------------------------------------
//imprime a arvore
//ENTRADA: grafo com uma arvore
//SAIDA: arquivo de texto no formato mostrado no pdf de descrição do trabalho para plot da arvore 
//complexidade O(n) 
void imprimeVertice (Vertice* vertice, fstream* saida);
//-------------------------------------------------------------------
//Algoritmo de Prim
//Computa uma AGM a partir de um grafo
//ENTRADA: grafo com arestas ponderadas
//SAIDA: grafo representando uma arvore geradora minima do grafo de entrada
//complexidade O((n+m)*log(n))
void prim(Grafo* grafo);
//-------------------------------------------------------------------
//Busca em profundidade
//gera o ciclo a partir da AGM a partir de uma busca em profundidade
//ENTRADA: grafo representando uma arvore
//SAIDA: grafo representando um ciclo e a soma das distancias entre os pontos adjacentes
//complexidade O(n + m)
float buscaEmProfundidade(Grafo* grafo);
//-------------------------------------------------------------------
//parte recursiva da busca em profundidade
//insere uma aresta entre o vertice que esta sendo visitado e o anterior a ele, formando o ciclo
//ENTRADA: vertice que está sendo visitado com suas respectivas adjacencias, ponteiro para o último vértice visitado e
//soma das distancias dos vertices inseridos até a chamada
//SAIDA: arestas do ciclo inseridas para os vertices em niveis inferiores ao de entrada 
//complexidade O(n + m) 
Vertice* buscaRecursivo(Vertice* vertice, Vertice* ultimo, float* distancia_total);

void ordenarAnticlk (Vertice* vertice, Vertice* ultimo);
bool criterioOrd(Vertice* a, Vertice* b);







