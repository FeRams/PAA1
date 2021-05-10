//feito por:
//Felipe Negrelli Wolter, RA: 1607766
//Luan Carlos Klein, RA: 2022613

#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
#include <algorithm>
#include<time.h>
#include <iomanip>
using namespace std;

# define PI           3.14159265358979323846

//declaração da classe vértice (para evitar erros na definição da classe Aresta)
//classe Vertice definida e explicada posteriormente
class Vertice;


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
    Vertice* fibo_irmao_esquerda;
    Vertice* fibo_irmao_direita;
    Vertice* fibo_pai;
    vector<Vertice*> fibo_filhos;
    int fibo_grau;

    //Armazena o custo da menor aresta de corte que chega nele em uma dada iteração do algoritmo de prim
    //é o peso usado pela heap
    float custo;
    //O vertice pai do vertice na AGM gerada
    //-------------------------------------------------------------------    
    // os atributos a seguir sao usados para o algoritmo de PRIM

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


// Classe Heap, ja adaptada para o algoritmo de prim 
//implementação de um heap mínimo
//armazena uma lista dos vértices ordenados de acordo com as propriedades de uma heap minima (pai menor que seus filhos)
//utilizado para seleção do proximo vertice a ser inserido na arvore no algoritmo de Prim
class Heap
{

  //Uma lista, que é o heap
  public:
    vector<Vertice*> heap;

// Construtora
    //complexidade O(1)
  Heap()
  {

  };

//Destrutora
    ~Heap()
    //complexidade O(1) pois a heap estará sempre vazia na finalização do algoritmo de Prim
    {
      for (int i = 0; i<heap.size(); i++)
        {
          delete heap[i];
        }
    };

// Pega o indice do elemento da direita
    //ENTRADA: uma posição da heap
    //SAIDA: filho da direita
    //complexidade O(1)
  int getDireita(int i)
  {
      return 2*i + 1;
  }

//Pega o indice do elemento da esquerda
    //ENTRADA: uma posição da heap
    //SAIDA: filho da direita
    //complexidade O(1)
  int getEsquerda(int i)
  {
      return 2*i + 2;
  }

//Pega o indice do elemento pai
    //ENTRADA: uma posição da heap
    //SAIDA: pai
    //complexidade O(1)
  int getPai(int i)
  {
      return int((i-1)/2);
  }


// Realiza o heapify do vertice passado como argumento
    //ENTRADA: um vertice numa posição numa posição menor ou igual a correta
    //SAIDA: heap ordenada
    //complexidade O(log(n))
  void heapify(Vertice* vertice)
  {

    //Pega a posicao dele no heap (que e a posicao na lista)
    int i = vertice->posHeap;
    
    // Qtd de elementos no heap
    int qtdNos = heap.size();

    //acha o filho da esquerda
    int esquerda = getEsquerda(i);
    //acha o filho da direita
    int direita = getDireita(i);
    //variavel que armazena o vertice com menor custo entre os 3
    int menor;

    // Encontra o menor elemento entre: Os dois filhos e ele
    if (esquerda <= qtdNos && heap[esquerda]->custo < heap[i]->custo)
    {
      // Se e o da esquerda
      menor = esquerda;
    }
    else
    {
      //Se e o proprio vertice analisado
      menor = i;
    }
    if(direita <= qtdNos && heap[direita]->custo < heap[menor]->custo)
    {
      // Se o da direita
      menor = direita;
    }

    // Se nao for ele, significa que nao esta na posicao correta. Entao precisa trocar de posição e  continuar executando
    if (menor != i)
    {
      //--------------------------------------------------
      //troca a posição do pai com a do menor filho

      Vertice* aux = heap[i];

      // Troca os elementos de lugar
      heap[i] = heap[menor];
      heap[menor] = aux;
      // Como as posicoes foram trocadas tambem, precisamos colocar elas de volta ao lugar
      // Faz com que o as posicoes voltem a seus lugares originais

      //salva a nova posição na variavel do vertice
      heap[i]->setPosHeap(i);
      heap[menor]->setPosHeap(menor);      
      //--------------------------------------------------

      //chamada recursiva realizada até que menor ==1
      //recursão chamada no máximo log(n) vezes
      heapify(heap[menor]);
    }
  }


// Inserir elemento no heap
    //ENTRADA: um vertice que deseja-se inserir na heap
    //SAIDA: heap com o novo vértice ordenada 
    //complexidade O(log(n))
  void insere(Vertice* elemento)
  {

    // Adiciona no final
    int tamanho = heap.size();
    elemento->setPosHeap(tamanho);
    heap.push_back(elemento);
    int posAtual = tamanho;

    // Faz a iterecao, fazendo com que ele suba no heap, ate o lugar certo
    while (posAtual > 0 && heap[getPai(posAtual)]->custo > heap[posAtual]->custo)
    {
      Vertice* aux = heap[posAtual];
      int auxPosPai = heap[getPai(posAtual)]->posHeap;
      int auxPosAtual = heap[posAtual]->posHeap;
      cout << auxPosPai << "    " << auxPosPai << "\n";
      // Troca os vertices de lugar
      heap[posAtual] = heap[getPai(posAtual)];
      heap[getPai(posAtual)] = aux;


      cout << auxPosPai << "    " << auxPosPai << "\n";
      // Como as posicoes foram trocadas tambem, precisamos colocar elas de volta ao lugar
      // Faz com que o as posicoes voltem a seus lugares originais
      heap[posAtual]->setPosHeap(auxPosAtual);
      heap[getPai(posAtual)]->setPosHeap(auxPosPai);


      posAtual = getPai(posAtual);
    }
  }

// Funcao para remover a reaiz do elemento
    //ENTRADA: um vertice que deseja-se remover da heap
    //SAIDA: heap com o vertice removido e ordenada 
    //complexidade O(log(n))
  Vertice* remove()
  {
    // Copia a raiz para uma variavel, e copia o ultimo elemento para a raiz
    Vertice* raiz = heap[0];
    heap[0] = heap[heap.size()-1];
    heap[0]->setPosHeap(0);
    // Tira o ultimo elemento
    heap.pop_back();
    // Faz o heapfy para arrumar o heap
    heapify(heap[0]);
    // Informa que o vertice saiu do heap
    raiz->setPosHeap(-1);
    return raiz;
  }

  //atualiza a chave de um vertice
    //ENTRADA: um vertice cuja chave deve ser atualizada
    //SAIDA: heap ordenada com a chave do vertice atualizado caso seja menor que a anterior
    //complexidade O(log(n))
  void atualizarChave(Vertice* vertice, float custo)
  {
    //só atualiza se o custo for menor
    if (custo < vertice->custo)
    {
      vertice->custo = custo;
      int i = vertice->posHeap;
      //enquanto o custo do vertice for menor que o do seu pai
      while (i>0 && heap[i]->custo < heap[getPai(i)]->custo)
      {
        //troca os dois de posição
        heap[i] = heap[getPai(i)];
        heap[getPai(i)] = vertice;
        vertice->setPosHeap(getPai(i));
        heap[i]->setPosHeap(i);
        i = vertice->posHeap;
      }
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







