#include "closest.h"


int main ()
{
	/**********************************************/
	//tamanho da entrada
	int size = 10;
	//distancia minima
	float dist = 0;
	//vetor de entrada
	Ponto* inputs;
	//ponteiro para pontos mais proximos
	Ponto* menor_a, *menor_b;
	//ponteiro para arquivo txt de entrada e saida
	FILE *f_inputs, *f_outputs;	
	/**********************************************/
	//inicializa os arquivos
	if ((f_inputs = fopen(INPUTS, "r")) == NULL)
	{
			return -1;
	}
	if ((f_outputs = fopen(OUTPUTS, "w")) == NULL)
	{
			return -1;
	}
	/**********************************************/
	//recebe o tamanho da entrada (com problema, arrumar)
	printf ("size: %d\n", size);
	size = fscanf(f_inputs, "%d", &size);
	printf ("size: %d\n", size);
	size = 10;
	//aloca o espaco para o vetor de entrada
	inputs = (float*) malloc (size * sizeof(Ponto));
	/**********************************************/
	//le os valor de entrada e passa para ovetor de pontos
	for (int i = 0; i< size; i++)
	{
		fscanf(f_inputs, "%f", &(inputs[i].x));
		fscanf(f_inputs, "%f", &(inputs[i].y));
	}	
	for (int i = 0; i< size; i++)
	{
		printf("ponto %d : (%f, %f)\n",i, inputs[i].x, inputs[i].y);
	}
	/**********************************************/
	//mede o tempo de execucao do forca bruta em ms
	clock_t begin = clock();
	dist = forca_bruta (inputs, size, &menor_a, &menor_b);
	clock_t end = clock();
	double time_spent = (double)(end - begin)/1000;	
	/**********************************************/
	//imprime o resultado na tela e no arquivo de saida
	printf("menor distancia : %f, entre (%f, %f), e (%f, %f)\n tempo: %lfms",
	 dist, menor_a->x, menor_a->y, menor_b->x, menor_b->y, time_spent);	
	//imprime no arquivo para quando formos fazer os graficos
	fprintf(f_outputs, "%d %lf\n", size, time_spent);
	/**********************************************/
	//fecha os arquivos e desaloca o vetor
	fclose (f_outputs);
	fclose (f_inputs);
	free (inputs);

//Apenas teste de ornecacao do merge sort
	inputs = (float*) malloc (11 * sizeof(Ponto));
	inputs[0].x = 1;
	inputs[1].x = 3;
	inputs[2].x = 5;
	inputs[3].x = 7;
	inputs[4].x = 9;
	inputs[5].x = 2;
	inputs[6].x = 4;
	inputs[7].x = 6;
	inputs[8].x = 8;
	inputs[9].x = 10;
	
	merge_sort(inputs, 0, 4, 9);
	printf("\n----------------------------------------------------------\n");
	for (int i = 0; i < 10; i++)
	{
		printf("pos %i: %f\n",i, inputs[i].x);
	}
}


float distance (Ponto* first, Ponto* second)
{
	float dist;
	dist = (first->x - second->x)* (first->x - second->x)+ (first->y - second->y)* (first->y - second->y);
	//dist = sqrt (dist);
	return dist;
}

//calcula a distancia entre todos os pontos
float forca_bruta (Ponto* inputs, int size, Ponto** first, Ponto** second)
{
	int dist  = FLT_MAX;
	for (int i=0;i<size-1;i++)
	{
		for(int j=i+1; j<size; j++)
		{
			int aux = distance (&inputs[i], &inputs[j]);
			if (aux < dist)
			{
				dist = aux;
				*first = &inputs[i];
				*second = &inputs[j];
			}
		}
	}
	dist = sqrt(dist);
	return dist;
}

// Funcao de ordenacao O(nlogn)
void merge_sort(Ponto* vectorPoints, int p, int r)
{
	// A condicao de recursao e de que a lista tenha mais que um elemento
	if (p < r)
	{
		// Faz a divisao (divide a lista no meio)
		int q = (int)(p+r)/2;
		// Faz a ordenacao para o lado esquerdo da lista
		merge_sort(vectorPoints, p, q);
		//Ordena o lado direito
		merge_sort(vectorPoints, q+1, r);
		//Faz a unicao ordenada das duas listas
		merge(vectorPoints, p, q, r);
	}
}

// Funcao que ordene duas listas ordenadas: O(n)
void merge(Ponto* vectorPoints, int p, int q, int r)
{
	// Define o tamanho que cada lista deve ter
	int sizeLeft = q-p;
	int sizeRigth = r-q -1;

	// Auxiliares para colocar na lista ordenadamente
	int posL = 0;
	int posR = 0;

	// Cria a lista auxiliar
	Ponto* left = (float*) malloc ( (sizeLeft+1) * sizeof(Ponto));
	Ponto* rigth = (float*) malloc ( (sizeRigth+1) * sizeof(Ponto));
	
	// Copia os elementos da primeira lista em uma auxiliar
	for (int i = 0; i<= sizeLeft; i++)
	{
		left[i] = vectorPoints[p+i];
	}
	// Copia os elementos da segunda lista em uma outra auxiliar
	for (int i = 0; i<= sizeRigth; i++)
	{
		rigth[i] = vectorPoints[q+i+1];
	}

	//Percorre todo o range da lista na lista original
	for (int i = p; i<=r; i++)
	{	
		// Se a lista da esquerda ja tiver sido toda percorrida, coloca automaticamente os elementos da esquerda	
		if (posL > sizeLeft)
		{
			// Coloca na lista original o elemento atual da lista direita
			vectorPoints[i] = rigth[posR];
			posR++;
		}
		// Faz o mesmo processo que antes, mas agora para a lista da direita
		else if (posR > sizeRigth)
		{
			vectorPoints[i] = left[posL];
			posL++;
		}
		// Caso ainda tenha elemento nas duas listas
		else
		{	
			// Compara qual o menor elemento, e adicona ele na lista original
			if(left[posL].x > rigth[posR].x)
			{
				vectorPoints[i] = rigth[posR];
				posR++;
			}
			else{
				vectorPoints[i] = left[posL];
				posL++;
			}
		}
	}
	// Limpa o espaço alocado para as listas auxiliares
	free(left);
	free(rigth);
}


float divisao_e_conquista (Ponto* inputs, int size, Ponto** first, Ponto** second)
{
	
}

float recursivo (Ponto** ord_x, Ponto** ord_y, Ponto** first, Ponto** second);