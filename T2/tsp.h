//feito por:
//Felipe Negrelli Wolter, RA: 1607766
//Luan Carlos Klein, RA: 2022613

#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
#include <algorithm>
#include<time.h>
using namespace std;

class Vertice;


//classe que representa um ponto 2D
class Ponto
{
  public:
    float x;
    float y;
    Ponto(float x, float y)
    {
        this->x = x;
        this->y = y;
    };
};

//representa uma aresta direcional
class Aresta {   
  public: 
        //vertice de onde a aresta sai
        Vertice* origem;
        //vertice para o qual a aresta vai
        Vertice* destino;
        //custo da aresta (a distancia entre os vertices)
        float custo;
};

//classe que representa um vertice
class Vertice 
{
  public:
    //posição do ponto que ele representa
    Ponto* ponto;
    //lista de arestas do vertice
    vector<Aresta*> adjacencias;
    //identificador da posição em que ele está no arquivo de entrada
    int id;
    
    // Usado para o algoritmo de PRIM

    //Armazena o custo da menor aresta para eçe (o .c do algortimo)
    float custo;
    //O vertice pai .p do algortimo
    Vertice* pai;
    // Representa a posicao daquele vertice no algortimo
    // Tem duas funcoes: Auxilia no heapify, e quando sair do heap, tem valor negativo, que facilita a verificao para ver se um vertice esta no heap ou nao 
    int posHeap;

    Vertice (float x, float y, int id)
    {
      this->ponto = new Ponto(x, y);
      this->id = id;

    };
    ~Vertice ()
    {
      for (int i = 0; i<adjacencias.size(); i++)
        {
          delete adjacencias [i];
        }
    };
    //adiciona no vetor de adjacencias uma aresta nova
    void adicionarAresta(Vertice* destino, float distancia)
    {
      Aresta* aresta = new Aresta ();
      aresta->origem = this;
      aresta->destino = destino;
      aresta->custo = distancia;
      adjacencias.push_back(aresta);
    };
    void printAdjacencias()
    {      
        for (int i = 0; i<adjacencias.size(); i++)
        {
          cout<<"  "<< adjacencias[i]->destino->id;
        }
    };

    // Seta o custo
    void setCusto(float _custo)
    {
        this->custo = _custo;
    }

    //Seta o pai
    void setPai(Vertice* _pai)
    {
      this->pai = _pai;
    }

    // Seta a posicao no heap
    void setPosHeap(int _pos)
    {
      this->posHeap = _pos;
    }

};

//classe que representa um grafo por lista de adjacencias
class Grafo
{
  public:
    //guarda um vetor de vertices
    //as arestas são armazenadas dentro do proprio vertice
    vector<Vertice*> vertices;

    Grafo ()
    {

    };
    ~Grafo ()
    {
      for (int i = 0; i<vertices.size(); i++)
        {
          delete vertices[i];
        }
    };
    //insere um novo vertice no final do vetor
    void adicionarVertice(Vertice* vertice)
    {
        vertices.push_back (vertice);
    };
    //imprime no terminal cada vertice com suas respectivas arestas
    void printGrafo()
    {
        for (int i = 0; i<vertices.size(); i++)
        {
          cout << vertices[i]->id << "->";
          vertices[i]->printAdjacencias();
          cout << "\n";
        }
    };
};


// Classe Heap, ja adaptada para facilitar o uso no algortimo de prim
class Heap
{

  //Uma lista, que e o heap
  public:
    vector<Vertice*> heap;

// Construtora
  Heap()
  {

  };

//Destrutora
    ~Heap()
    {
      for (int i = 0; i<heap.size(); i++)
        {
          delete heap[i];
        }
    };

// Pega o indice do elemento da direita
  int getDireita(int i)
  {
      return 2*i + 1;
  }

//Pega o indice do elemento da esquerda
  int getEsquerda(int i)
  {
      return 2*i + 2;
  }

//Pega o indice do elemento pai
  int getPai(int i)
  {
      return int((i-1)/2);
  }


// Realiza o heapgy do vertice passado como argumento
  void heapify(Vertice* vertice)
  {

    //Pega a posicao dele no heap (que e a posicao na lista)
    int i = vertice->posHeap;
    
    // Qtd de elementos no heap
    int qtdNos = heap.size();

    int esquerda = getEsquerda(i);
    int direita = getDireita(i);
    int menor;

    // Encontra o menor elemento entre: Os dois filhos e ele
    if (esquerda <= qtdNos && heap[esquerda]->custo < heap[i]->custo)
    {
      // Se e o da esquerda
      menor = esquerda;
    }
    else
    {
      //Se e o prorpio vertice analisado
      menor = i;
    }
    if(direita <= qtdNos && heap[direita]->custo < heap[menor]->custo)
    {
      // Se o da direita
      menor = direita;
    }

    // Se nao for ele, significa que nao esta na posicao correta. Entao precisa continuar executando
    if (menor != i)
    {

      Vertice* aux = heap[i];
      int posAuxI = heap[i]->posHeap;
      int posAuxMenor = heap[menor]->posHeap;

      // Troca os elemtnos de lugar
      heap[i] = heap[menor];
      heap[menor] = aux;

      // Como as posicoes foram trocadas tambem, precisamos colocar elas de volta ao lugar
      // Faz com que o as posicoes voltem a seus lugares originais

      heap[i]->setPosHeap(posAuxI);
      heap[menor]->setPosHeap(posAuxMenor);

      heapify(heap[menor]);
    }
  }


// Inserir elemento no heap
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

// Apenas para vizualizar o heap. ###############APAGAR NA ENTREGA
  void mostraHeap()
  {
    int i = heap.size();
    int cont = 0;
    cout << "Tamanho: " << i << "\n";
    while (cont < i)
    {
      cout << heap[cont]->custo << "   ID: " << heap[cont]->id << "\n";
      cont += 1;
    }
  }


// Funcao para remover a reaiz do elemento
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
  void atualizarChave(Vertice* vertice, float custo)
  {
    //só atualiza se for menor
    if (custo < vertice->custo)
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
};



float distancia (Ponto* a, Ponto* b);
Grafo* criarGrafo (char* nome);
void imprimeArvore (Grafo* grafo);
void imprimeCiclo (Grafo* grafo);
void imprimeVertice (Vertice* vertice, fstream* saida);



void prim(Grafo* grafo);
void imprimeCaminhoPrim(Grafo* grafo);
float buscaEmProfundiade(Grafo* grafo);
Vertice* buscaRecursivo(Vertice* vertice, Vertice* ultimo, float* distancia_total);







