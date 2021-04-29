#include "tsp.h"

#include <fstream>


//-------------------------------------------------------------------
//Calcula a distandia euclidiana entre dois pontos
float distancia (Ponto* a, Ponto* b)
{
    float dist;
	dist = (a->x - b->x)* (a->x - b->x)+ (a->y - b->y)* (a->y - b->y);
	dist = sqrt (dist);
	return dist;
}

//-------------------------------------------------------------------
//Le o arquivo de entrada
//Inicializa um grafo completo
Grafo* criarGrafo (char* nome)
{
    float x, y;
    int tamanho;
    fstream entrada;
    Grafo* grafo = new Grafo();
    entrada.open(nome, ios::in);
    //le o tamanho de entrada
    entrada >> tamanho;
    //le todas as entradas
    for (int i = 0; i< tamanho; i++)
    {
        //le a coordenada em x, em y
        entrada >> x;
        entrada >> y;
        //cria um novo vertice
        Vertice* vertice = new Vertice (x, y);
        //e adiciona ao grafo
        grafo->adicionarVertice(vertice);
    }
    //para cada par de pontos distintos
    for(int i = 0; i< tamanho; i++)
    {
        for (int j = i; j<tamanho; j++)
        {
            //cria um vertice entre eles
            float peso = distancia ((grafo->vertices[i]->ponto), (grafo->vertices[j]->ponto));
            grafo->vertices[i]->adicionarAresta(grafo->vertices[j], peso);
            grafo->vertices[j]->adicionarAresta(grafo->vertices[i], peso);
        }
    }
    return grafo;
}
int main (int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "favor passar arquivo de pontos\n");
        return 1;
    }
    criarGrafo(argv[1]);
}