//feito por:
//Felipe Negrelli Wolter, RA: 1607766
//Luan Carlos Klein, RA: 2022613

#include "tsp.h"

int main (int argc, char *argv[])
{
//-------------------------------------------------------------------
    if (argc != 2)
    {
        fprintf(stderr, "favor passar arquivo de pontos\n");
        return 1;
    }
//-------------------------------------------------------------------
    //inicializa o a contagem do tempo
    clock_t begin = clock();
//-------------------------------------------------------------------
    //algoritmo em si
    //chama função para criar o grafo completo a partir do arquivo passado (etapa 1)
    Grafo* grafo = criarGrafo(argv[1]);
    //chama a função na qual esta implementada o algoritmo de Prim para gerar a AGM (etapa 2)
    prim(grafo);
    //função para gerar o .txt usado no plot da AGM
    imprimeArvore(grafo);
    //chama a função que usa a busca em profundidade para gerar o ciclo (etapa 3)
    float distancia_total = buscaEmProfundiade(grafo);
    //função para gerar o .txt usado no plot da AGM
    imprimeCiclo(grafo);
//-------------------------------------------------------------------
    //para a contagem do tempo
    clock_t end = clock();
    //calcula o tempo decorrido em segundos
    float time_spent = (float)(end - begin)/1000000;
//-------------------------------------------------------------------
    //imprime no terminal
    cout<< time_spent <<" "<<distancia_total<< "\n";
}



//-------------------------------------------------------------------
//Calcula a distandia euclidiana entre dois pontos
//ENTRADA: dois pontos
//SAIDA: distancia euclidiana entre eles 
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
//ENTRADA: arquivo .txt com as coordenadas dos pontos
//SAIDA: grafo completo 
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
        Vertice* vertice = new Vertice (x, y, i);
        //e adiciona ao grafo
        grafo->adicionarVertice(vertice);
    }
    //para cada par de pontos distintos
    for(int i = 0; i< tamanho; i++)
    {
        for (int j = i+1; j<tamanho; j++)
        {
            //cria um vertice entre eles
            float peso = distancia ((grafo->vertices[i]->ponto), (grafo->vertices[j]->ponto));
            grafo->vertices[i]->adicionarAresta(grafo->vertices[j], peso);
            grafo->vertices[j]->adicionarAresta(grafo->vertices[i], peso);
        }
    }
    return grafo;
}


//-------------------------------------------------------------------
//Algortimo de Prim
//Computa uma AGM a partir de um grafo
//ENTRADA: grafo
//SAIDA: grafo representando uma arvore geradora minima do grafo de entrada
void prim(Grafo* grafo)
{
    // Cria o heap
    Heap h = Heap();
    Vertice* s;
    Vertice* u;
    Vertice* v;
    Aresta* e;
    // Da os valores inicais para cada vertice e adiciona ele no heap
    for (int i = 0; i < grafo->vertices.size(); i++)
        {
            grafo->vertices[i]->setCusto(999999000);
            grafo->vertices[i]->setPai(NULL);
            h.insere(grafo->vertices[i]);
        }
    
    //Pega o primeiro elemento, e comeca por ele
    s = grafo->vertices[0];
    s->setCusto(0);
    h.heapify(s);

    // Percorre enquanto nao for vazio
    while (h.heap.size() > 0)
    {
        // Remove a raiz
        u = h.remove();
        //h.mostraHeap();
//-------------------------------------------------------------------
        // insere no grafo a menor aresta de corte
        if (u->pai != NULL)
        {
            float peso = distancia ((u->ponto), (u->pai->ponto));
            u->pai->adicionarAresta(u, peso);
        }
//-------------------------------------------------------------------

        // Percorre os vertices adjacentes a ele
        for (int i = 0; i<u->adjacencias.size(); i++)
        {
            // Pega a areesta
            e = u->adjacencias[i];
            // Pega o vertice destino
            v = u->adjacencias[i]->destino;

            // Verifica se o vertice ainda faz parte do corte.
            if (v->posHeap > -1)
            {
                // Se fizer, entao verifica os custo, e atualiza 
                if (v->custo > e->custo)
                {
                    h.atualizarChave(v, e->custo);
                    v->setPai(u);
                } 
            }
        }
//-------------------------------------------------------------------
        // remove as arestas que saem do vertice
        // isso para deixar no grafo apenas as arestas da arvore
        for (int i = 0; i<u->adjacencias.size(); i++)
        {
            delete u->adjacencias[i];
        }
        u->adjacencias.clear();
//-------------------------------------------------------------------
    }
}


//-------------------------------------------------------------------
//Busca em profundidade
//gera o ciclo a partir da AGM a partir de uma busca em profundidade
//ENTRADA: grafo representando uma arvore
//SAIDA: grafo representando um ciclo e a soma das distancias entre os pontos adjacentes
float buscaEmProfundiade(Grafo* grafo)
{
    float distancia_total = 0;
    Vertice* fim;
    //chama a busca recursiva
    fim = buscaRecursivo(grafo->vertices[0], NULL, &distancia_total);
    //---------------------------------------------------------------
    //insere a ultima aresta, para fechar o ciclo
    float peso = distancia ((grafo->vertices[0]->ponto), (fim->ponto));
    fim->adicionarAresta(grafo->vertices[0], peso);
    distancia_total += peso;
    //---------------------------------------------------------------
    return distancia_total;
}
//-------------------------------------------------------------------


//-------------------------------------------------------------------
//parte recursiva da busca em profundidade
//insere uma aresta entre o vertice que esta sendo visitado e o anterior a ele
//ENTRADA: vertice com suas respectivas adjacencias
//SAIDA: arestas do ciclo inseridas para os vertices em niveis inferiores ao de entrada 
Vertice* buscaRecursivo(Vertice* vertice, Vertice* ultimo, float* distancia_total)
{
    Vertice* u;
    int aux;
    
    //insere a aresta entre o vertice que esta sendo visitado e o anterior a ele
    if (ultimo != NULL)
    {
        float peso = distancia ((vertice->ponto), (ultimo->ponto));
        ultimo->adicionarAresta(vertice, peso);
        *distancia_total += peso;
    }
    ultimo = vertice;
    //chama a função recursiva de busca para os vertices adjacentes a ele
    for (int i = vertice->adjacencias.size()-1; i >=0; i--)
    {
        u = vertice->adjacencias[i]->destino;
        //apaga as arestas, para que fiquem apenas as que pertencem ao ciclo
        delete vertice->adjacencias[i];
        vertice->adjacencias.erase(vertice->adjacencias.begin()+i);
        //chama a busca recursiva
        ultimo = buscaRecursivo(u, ultimo, distancia_total);
    }
    return ultimo;
}

//-------------------------------------------------------------------
//Imprime o ciclo
void imprimeCiclo(Grafo* grafo)
{ 
    fstream saida;
    saida.open("cycle.txt", ios::out);

    //chama a função recursiva
    imprimeVertice(grafo->vertices[0], &saida);
    //imprime a aresta para fechar o grafo
    saida<<grafo->vertices[0]->ponto->x << " " <<grafo->vertices[0]->ponto->y <<"\n";


    saida.close();
    system ("gnuplot cycle.plot");
    system ("xdg-open ciclo.pdf");
}


//-------------------------------------------------------------------
//função para impressão recursiva dos vertices e arestas
void imprimeVertice (Vertice* vertice, fstream* saida)
{    
    //std::rotate(vertice->adjacencias.rbegin(), vertice->adjacencias.rbegin() + 1, vertice->adjacencias.rend());
    (*saida) << vertice->ponto->x << " " << vertice->ponto->y <<"\n";
    for (int i = 0; i < vertice->adjacencias.size(); i++)
    {
        if (vertice->adjacencias[i]->destino->id != 0)
            imprimeVertice(vertice->adjacencias[i]->destino, saida);
    }
}

//-------------------------------------------------------------------
//imprime a arvore
void imprimeArvore (Grafo* grafo)
{    
    fstream saida;
    saida.open("tree.txt", ios::out);
    //percorre os vertices do grafo
    for (int i = 0; i < grafo->vertices.size(); i++)
    {
        //percorre as adjacencias do vertice
        for (int j = 0; j< grafo->vertices[i]->adjacencias.size();j++)
        {
            saida<<grafo->vertices[i]->ponto->x << " " <<grafo->vertices[i]->ponto->y <<"\n";
            saida<<grafo->vertices[i]->adjacencias[j]->destino->ponto->x << " " <<grafo->vertices[i]->adjacencias[j]->destino->ponto->y<<"\n\n";
        }
    }
    saida.close();
    system ("gnuplot tree.plot");
    system ("xdg-open arvore.pdf");
}
