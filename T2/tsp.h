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

    Vertice (float x, float y)
    {
      this->ponto = new Ponto(x, y);
    };
    void adicionarAresta(Vertice* destino, float distancia)
    {
      Aresta* aresta = new Aresta ();
      aresta->origem = this;
      aresta->destino = destino;
      aresta->custo = distancia;
      adjacencias.push_back(aresta);
    };
};


class Grafo
{
  public:
    vector<Vertice*> vertices;

    Grafo ()
    {

    };
    void adicionarVertice(Vertice* vertice)
    {
        vertices.push_back (vertice);
    };
};
