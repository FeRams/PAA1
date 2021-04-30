#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
using namespace std;

class Vertice;



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


class Aresta {   
  public: 
        Vertice* origem;
        Vertice* destino;
        int custo;
};


class Vertice 
{
  public:
    Ponto* ponto;
    vector<Aresta*> adjacencias;
    int id;

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
};


class Grafo
{
  public:
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
    void adicionarVertice(Vertice* vertice)
    {
        vertices.push_back (vertice);
    };
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


float distancia (Ponto* a, Ponto* b);
Grafo* criarGrafo (char* nome);
void imprimeArvore (Grafo* grafo);
void imprimeCiclo (Grafo* grafo);
