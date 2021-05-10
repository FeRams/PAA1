//feito por:
//Felipe Negrelli Wolter, RA: 1607766
//Luan Carlos Klein, RA: 2022613

#include "tsp.h"

//função principal que faz a chamada das funções que implementam as etapas do algoritmo de aproximação
//complexidade O(n^2*lg(n)) devido ao algoritmo de Prim, etapa mais demorada do algoritmo
int main (int argc, char *argv[])
{
//-------------------------------------------------------------------
    //da erro caso não seja passado o argumento
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
    //complexidade O(n+m), mas como o grafo é completo, m=O(n^2), ou seja possui complexidade O(n^2)
    Grafo* grafo = criarGrafo(argv[1]);

    //chama a função na qual esta implementada o algoritmo de Prim para gerar a AGM (etapa 2)
    //complexidade O((n+m)*log(n)) = O(n^2*lg(n))
    prim(grafo);

    //para a contagem do tempo para a geração do .txt da arvore
    clock_t end = clock();

    //função para gerar o .txt usado no plot da AGM
    //complexidade O(n+m), mas como m = n-1 para uma arvore, fica O(n)
    imprimeArvore(grafo);

    //calcula o tempo parcial
    float time_spent = (float)(end - begin)/1000000;
    //retoma a contagem do tempo
    begin = clock();

    //chama a função que usa a busca em profundidade para gerar o ciclo (etapa 3)
    //complexidade O(n+m), mas como m = n-1 para uma arvore, fica O(n)
    float distancia_total = buscaEmProfundidade(grafo);

    //para a contagem do tempo
    end = clock();

    //função para gerar o .txt usado no plot da AGM
    //complexidade O(n+m), mas como m = n para uma ciclo, fica O(n)
    imprimeCiclo(grafo);
//-------------------------------------------------------------------
    //calcula o tempo decorrido em segundos
    time_spent += (float)(end - begin)/1000000;
//-------------------------------------------------------------------
    //imprime no terminal
    cout<<fixed<<setprecision(6)<<time_spent <<" ";
    cout<<fixed<<setprecision(6)<<distancia_total<< "\n";
}



//-------------------------------------------------------------------
//Calcula a distancia euclidiana entre dois pontos
//ENTRADA: dois pontos 2D (a e b)
//SAIDA: distancia euclidiana entre eles 
//complexidade O(1), pois são apenas operações aritmeticas
float distancia (Ponto* a, Ponto* b)
{
    float dist;
    dist = (a->x - b->x)* (a->x - b->x)+ (a->y - b->y)* (a->y - b->y);
    //raiz é O(1) pois está sendo aproximada por uma parte de uma série com numero de somas constante
    dist = sqrt (dist);
    return dist;
}

//-------------------------------------------------------------------
//Le o arquivo de entrada
//Inicializa um grafo completo
//ENTRADA: arquivo .txt com as coordenadas dos pontos
//SAIDA: grafo completo, onde cada vértice corresponde a um ponto da entrada
//complexidade O(n^2)
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
    //laço repetido n vezes
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
    //laço repetido n vezes
    for(int i = 0; i< tamanho; i++)
    {
        //laço repetido O(n^2) vezes no total
        for (int j = i+1; j<tamanho; j++)
        {
            //calcula a distancia entre os 2 vértices (que será o peso da aresta criada)
            float peso = distancia ((grafo->vertices[i]->ponto), (grafo->vertices[j]->ponto));
            //cria um vertice entre eles
            grafo->vertices[i]->adicionarAresta(grafo->vertices[j], peso);
            grafo->vertices[j]->adicionarAresta(grafo->vertices[i], peso);
        }
    }
    return grafo;
}


//-------------------------------------------------------------------
//Algoritmo de Prim
//Computa uma AGM a partir de um grafo
//ENTRADA: grafo com arestas ponderadas
//SAIDA: grafo representando uma arvore geradora minima do grafo de entrada
//complexidade O((n+m)*log(n))
void prim(Grafo* grafo)
{
    float custo_total = 0;
    // Cria o heap
    Heap_Fibonacci h = Heap_Fibonacci();
    Vertice* s;
    Vertice* u;
    Vertice* v;
    Aresta* e;
    // Da os valores inicias para cada vertice e os adiciona no heap
    // laço repetido n vezes
    for (int i = 0; i < grafo->vertices.size(); i++)
        {
            //vertice começa com custo infinito (no caso, 999999000)
            grafo->vertices[i]->setCusto(999999000);
            grafo->vertices[i]->setPai(NULL);
            h.insere(grafo->vertices[i]);
        }
    
    //Pega o primeiro elemento, e comeca por ele
    s = grafo->vertices[0];
    //traz ele para o inicio da heap
    h.atualizarChave(s, 0);

    // Percorre enquanto a heap nao for vazia
    // laço repetido n vezes, custo total de O(n*log(n)) + O(m*log(n)) = O((n+m)*lg(n))
    while (h.tamanho > 0)
    {
        // Remove a raiz
        //complexidade O (log(n)) repetido n vezes
        u = h.remove();
        custo_total += u->custo;
        //h.mostraHeap();
//-------------------------------------------------------------------
        // insere no grafo a menor aresta de corte
        if (u->pai != NULL)
        {
            u->pai->adicionarFilho(u);
        }
//-------------------------------------------------------------------

        // Percorre os vertices adjacentes a ele
        //laço repetido um total de m vezes
        for (int i = 0; i<u->adjacencias.size(); i++)
        {
            // Pega a aresta
            e = &u->adjacencias[i];
            // Pega o vertice destino
            v = u->adjacencias[i].destino;

            // Verifica se o vertice ainda faz parte do corte.
            if (v->posHeap > -1)
            {
                // Se fizer, entao verifica os custo, e atualiza 
                if (v->custo > e->custo)
                {
                    //custo de O(log(n)) repetido m vezes
                    h.atualizarChave(v, e->custo);
                    v->setPai(u);
                } 
            }
        }
//-------------------------------------------------------------------
    }
    cout<<"custo total da arvore: "<<fixed<<setprecision(6)<<custo_total<<endl;
}


//-------------------------------------------------------------------
//Busca em profundidade
//gera o ciclo a partir da AGM a partir de uma busca em profundidade
//ENTRADA: grafo representando uma arvore
//SAIDA: grafo representando um ciclo e a soma das distancias entre os pontos adjacentes
//complexidade O(n + m)
float buscaEmProfundidade(Grafo* grafo)
{
    float distancia_total = 0;
    Vertice* fim;
    //chama a busca recursiva
    //função recursiva é chamada n vezes, sendo que o custo total dentro delas é O(m)
    //resultando em uma complexidade de O(n + m)
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
//parte recursiva da busca em profundidade
//insere uma aresta entre o vertice que esta sendo visitado e o anterior a ele, formando o ciclo
//ENTRADA: vertice que está sendo visitado com suas respectivas adjacencias, ponteiro para o último vértice visitado e
//soma das distancias dos vertices inseridos até a chamada
//SAIDA: arestas do ciclo inseridas para os vertices em niveis inferiores ao de entrada 
//complexidade O(n + m) 
Vertice* buscaRecursivo(Vertice* vertice, Vertice* ultimo, float* distancia_total)
{
    Vertice* u;
    int aux;
    vertice->adjacencias.clear();
    ordenarAnticlk(vertice, ultimo);
    //insere a aresta entre o vertice que esta sendo visitado e o anterior a ele
    if (ultimo != NULL)
    {
        float peso = distancia ((vertice->ponto), (ultimo->ponto));
        ultimo->adicionarAresta(vertice, peso);
        *distancia_total += peso;
    }
    ultimo = vertice;
    //chama a função recursiva de busca para os vertices adjacentes a ele
    //for repetido m vezes durante toda a execução da busca
    for (int i = vertice->filhos.size()-1; i >=0; i--)
    {
        u = vertice->filhos[i];
        //chama a busca recursiva
        //chamada recursiva feita n vezes durante toda a execução da busca
        ultimo = buscaRecursivo(u, ultimo, distancia_total);
    }
    return ultimo;
}

//-------------------------------------------------------------------
//função para ordenar as arestas de um vértice em sentido anti-horario a partir da aresta de entrada
//ENTRADA: vértice com arestas a serem ordenadas e vértice visitado imediatamente antes dele
//SAIDA: arestas do vetor ordenadas em sentido anti-horario a partir da aresta de entrada
//complexidade O(m*log(m)) 
void ordenarAnticlk (Vertice* vertice, Vertice* ultimo)
{
    //variavel que armazena o angulo de entrada. servira de referencia
    //para o calculo dos angulos relativos de cada vertice
    float angulo_entrada = 0;
    //se for o vertice 0, não vai haver vertice anterior e o angulo de entrada
    //será setado como 0º
    if (ultimo != NULL)
    {
        //vai calcular o angulo de entrada
        //verifica pelo caso onde a tangente é indefinida (ang = +-pi/2)
        if (ultimo->ponto->x != vertice->ponto->x)
        {
            //se a tangente puder ser calculada, utiliza a função atan2 da biblioteca
            //math.h (complexidade O(1), pois calcula por series).
            //a função retorna um angulo no intervalo [pi, -pi)
            angulo_entrada = atan2((ultimo->ponto->y - vertice->ponto->y),(ultimo->ponto->x - vertice->ponto->x));
        }
        else
        {
            //se for o caso indefinido, atribui o valor manualmente
            if (ultimo->ponto->y > vertice->ponto->y)
            {
                angulo_entrada = PI / 2;
            }
            else
            {
                angulo_entrada = - PI / 2;
            }
        }
    }
    //laço que calcula o angulo relativo de cada filho (chamado n vezes)
    for (int i= 0; i< vertice->filhos.size(); i++)
    {
        Vertice* filho = vertice->filhos[i];
        //etapa analoga à feita para o angulo de entrada
        //a diferença é que é feito angulo de entrada - ang calculado
        //isso para calcular o angulo relativo no sentido anti-horario
        if (filho->ponto->x != vertice->ponto->x)
        {
            filho->angulo = angulo_entrada- atan2((filho->ponto->y - vertice->ponto->y),(filho->ponto->x - vertice->ponto->x));
        }
        else
        {
            if (filho->ponto->y > vertice->ponto->y)
            {
                filho->angulo = angulo_entrada- PI / 2;
            }
            else
            {
                filho->angulo = angulo_entrada+ PI / 2;
            }
        }
        //ajusta para que todos os valores fiquem entre 0 e 360
        //para poder fazer a comparação corretamente
        if (filho->angulo <0)
        {
            filho->angulo += 360;
        }
        if (filho->angulo >= 360)
        {
            filho->angulo -= 360;
        }
    }
    //usa a função sort do STL para ordenar os filhos em ordem crescente de angulo
    //é feito por uma adaptação do quick sort para evitar o pior caso.
    //custo amortizado de O(m*log(m))
    sort(vertice->filhos.begin(), vertice->filhos.end(), criterioOrd);
}
//-------------------------------------------------------------------
//Criterio de ordenação utilizado pelo sort
//ENTRADA: dois vertices (a e b)
//SAIDA: TRUE se o angulo de a for menor que o de b 
//complexidade O(1) 
bool criterioOrd(Vertice* a, Vertice* b)
{
    return (a->angulo < b->angulo);
}

//-------------------------------------------------------------------
//Imprime o ciclo
//ENTRADA: grafo com um ciclo hamiltoniano
//SAIDA: arquivo de texto no formato mostrado no pdf de descrição do trabalho para plot do ciclo 
//complexidade O(n) 
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
//ENTRADA: vértice e ponteiro para arquivo de saida
//SAIDA: vertice atual e os subsequentes a ele escritos no arquivo de saida
//complexidade O(n) 
void imprimeVertice (Vertice* vertice, fstream* saida)
{    
    (*saida) << vertice->ponto->x << " " << vertice->ponto->y <<"\n";

    if (vertice->adjacencias[0].destino->id != 0)
    {
        imprimeVertice(vertice->adjacencias[0].destino, saida);
    }
}

//-------------------------------------------------------------------
//imprime a arvore
//ENTRADA: grafo com uma arvore
//SAIDA: arquivo de texto no formato mostrado no pdf de descrição do trabalho para plot da arvore 
//complexidade O(n) 
void imprimeArvore (Grafo* grafo)
{    
    fstream saida;
    saida.open("tree.txt", ios::out);
    //percorre os vertices do grafo
    for (int i = 0; i < grafo->vertices.size(); i++)
    {
        //percorre as adjacencias do vertice
        for (int j = 0; j< grafo->vertices[i]->filhos.size();j++)
        {
            saida<<grafo->vertices[i]->ponto->x << " " <<grafo->vertices[i]->ponto->y <<"\n";
            saida<<grafo->vertices[i]->filhos[j]->ponto->x << " " <<grafo->vertices[i]->filhos[j]->ponto->y<<"\n\n";
        }
    }
    saida.close();
    system ("gnuplot tree.plot");
    system ("xdg-open arvore.pdf");
}
