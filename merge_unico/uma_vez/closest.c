#include "closest.h"
int aqui;

int main ()
{
	/**********************************************/
	//tamanho da entrada
	int size = 100;
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
	fscanf(f_inputs, "%d", &size);
	printf ("size: %d\n", size);
	//aloca o espaco para o vetor de entrada
	inputs = (Ponto*) malloc (size * sizeof(Ponto));
	/**********************************************/
	//le os valor de entrada e passa para ovetor de pontos
	for (int i = 0; i< size; i++)
	{
		fscanf(f_inputs, "%f", &(inputs[i].x));
		fscanf(f_inputs, "%f", &(inputs[i].y));
	}	
	/*for (int i = 0; i< size; i++)
	{
		printf("ponto %d : (%f, %f)\n",i, inputs[i].x, inputs[i].y);
	}*/
	/**********************************************/
	//mede o tempo de execucao do forca bruta em ms
	clock_t begin = clock();
	dist = sqrt(forca_bruta (inputs, size, &menor_a, &menor_b));
	clock_t end = clock();
	double time_spent = (double)(end - begin)/1000;	
	/**********************************************/
	//imprime o resultado na tela e no arquivo de saida
	printf ("\n ---------------------------------\n");	
	printf ("FORCA BRUTA\n");
	printf("menor distancia : %f, entre (%f, %f), e (%f, %f)\n tempo: %lfms",
	 dist, menor_a->x, menor_a->y, menor_b->x, menor_b->y, time_spent);	
	printf ("\n ---------------------------------\n");
	//imprime no arquivo para quando formos fazer os graficos
	fprintf(f_outputs, "%d %lf\n", size, time_spent);
	/**********************************************/
	//mede o tempo de execucao do divisao e conquista em ms
	begin = clock();
	dist = divisao_e_conquista (inputs, size, &menor_a, &menor_b);
	end = clock();
	time_spent = (double)(end - begin)/1000;	
	/**********************************************/
	//imprime o resultado na tela e no arquivo de saida
	printf ("\n ---------------------------------\n");	
	printf ("DIVISAO E CONQUISTA\n");
	printf("menor distancia : %f, entre (%f, %f), e (%f, %f)\n tempo: %lfms",
	 dist, menor_a->x, menor_a->y, menor_b->x, menor_b->y, time_spent);	
	printf ("\n ---------------------------------\n");	
	//imprime no arquivo para quando formos fazer os graficos
	fprintf(f_outputs, "%d %lf\n", size, time_spent);
	/**********************************************/
	//fecha os arquivos e desaloca o vetor
	fclose (f_outputs);
	fclose (f_inputs);
	free (inputs);

	
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
	float dist  = FLT_MAX;
	for (int i=0;i<size-1;i++)
	{
		for(int j=i+1; j<size; j++)
		{
			float aux = distance (&inputs[i], &inputs[j]);
			if (aux < dist)
			{
				dist = aux;
				*first = &inputs[i];
				*second = &inputs[j];
			}
		}
	}
	//printf("fb: %d\n", dist);
	//dist = sqrt(dist);
	return dist;
}

// Funcao de ordenacao O(nlogn)
void merge_sort(Ponto** vectorPoints, int p, int r, char tipo)
{
	// A condicao de recursao e de que a lista tenha mais que um elemento
	if (p < r)
	{
		// Faz a divisao (divide a lista no meio)
		int q = (int)(p+r)/2;
		// Faz a ordenacao para o lado esquerdo da lista
		merge_sort(vectorPoints, p, q, tipo);
		//Ordena o lado direito
		merge_sort(vectorPoints, q+1, r, tipo);
		//Faz a unicao ordenada das duas listas
		merge(vectorPoints, p, q, r, tipo);
	}
}

// Funcao que ordene duas listas ordenadas: O(n)
void merge(Ponto** vectorPoints, int p, int q, int r, char tipo)
{
	// Define o tamanho que cada lista deve ter
	int sizeLeft = q-p;
	int sizeRigth = r-q -1;

	// Auxiliares para colocar na lista ordenadamente
	int posL = 0;
	int posR = 0;

	// Cria a lista auxiliar
	Ponto** left = (Ponto**) malloc ( (sizeLeft+1) * sizeof(Ponto*));
	Ponto** rigth = (Ponto**) malloc ( (sizeRigth+1) * sizeof(Ponto*));
	
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
			//se for ordenar em x
			if (tipo == 'x')
			{
				// Compara qual o menor elemento, e adicona ele na lista original
				if (left[posL]->x == rigth[posR]->x)
				{
					if (left[posL]->y < rigth[posR]->y)
					{
						vectorPoints[i] = left[posL];
						posL++;
					}
					else
					{
						vectorPoints[i] = rigth[posR];
						posR++;
					}
				}
				else if(left[posL]->x > rigth[posR]->x)
				{
					vectorPoints[i] = rigth[posR];
					posR++;
				}
				else{
					vectorPoints[i] = left[posL];
					posL++;
				}
			}
			else
			{
				// Compara qual o menor elemento, e adicona ele na lista original
				if(left[posL]->y > rigth[posR]->y)
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
	}
	// Limpa o espaço alocado para as listas auxiliares
	free(left);
	free(rigth);
}


float divisao_e_conquista (Ponto* inputs, int size, Ponto** first, Ponto** second)
{
	//-----------------------------------------------------
	float dist;
	//vetores usados para ordenacao em x e em y
	Ponto** ord_x, **ord_y;
	//-----------------------------------------------------
	//caso em que é impossível calcular a distância
	if (size <=1)
	{
		*first = NULL;
		*second = NULL;
		return FLT_MAX;
	}
	//-----------------------------------------------------
	//aloca os vetores
	ord_x = (Ponto**) malloc (size * sizeof(Ponto*));
	ord_y = (Ponto**) malloc (size * sizeof(Ponto*));
	//copia neles o vetor de entrada
	for (int i = 0; i < size;i++)
	{
		ord_x[i] =  &inputs[i];
		ord_y[i] =  &inputs[i];
	}
	//-----------------------------------------------------
	//ordena os dois vetores
    merge_sort(ord_x, 0, size-1, 'x');
	merge_sort(ord_y, 0, size-1, 'y');
	//-----------------------------------------------------
	//chamada recursiva
    dist = recursivo (ord_x, ord_y, size, first, second);
    //a raiz é calculada no final, por possuir uma complexidade maior
    dist = sqrt(dist);
    free (ord_x);
    free (ord_y);
    return dist;
}

float recursivo (Ponto** ord_x, Ponto** ord_y, int size, Ponto** first, Ponto** second)
{	
	//-----------------------------------------------------
	//caso base-> testa todos contra todos
	if (size <= 3)
	{
		float dist  = FLT_MAX;
		for (int i=0;i<size-1;i++)
		{
			for(int j=i+1; j<size; j++)
			{
				float aux = distance (ord_x[i], ord_x[j]);
				if (aux < dist)
				{
					dist = aux;
					*first = ord_x[i];
					*second = ord_x[j];
				}
			}
		}
		return dist;
	}
	//-----------------------------------------------------
	//acha a mediana de x
	int mid = (size-1)/2;
	//tamanho dos vetores da esquerda e direita
	int size_l = (size+1)/2, size_r = size/2;
	//inicializa os valores de distancia com o valor maximo
	float d= FLT_MAX, d_l = FLT_MAX, d_r= FLT_MAX;
	//valor da mediana de x
	float mid_x = ord_x[mid]->x;
	//vetores que vão ser usados para as chamadas recursivas
	Ponto** ord_y_l, **ord_y_r, **ord_nxt;
	//vetores que vao salvar os pontos de distancia minima na esquerda e direita
	Ponto* left_a , *left_b, *right_a, *right_b;
	//inicializa os vetores
	ord_nxt = (Ponto**) malloc (size * sizeof(Ponto*));
	ord_y_l= ord_y;
	ord_y_r = &(ord_y[size_l]);
	//**************************************************************************
	//ETAPA DE DIVISÃO
	int l =0, r = 0;
	//vai copiar os valores de y para novos vetores que serão usados nas chamadas recursivas
	for (int i =0; i<size; i++)
	{
		ord_nxt[i] = ord_y[i];
	}
	for (int i =0; i< size; i++)
	{
	//se o ponto esta na esquerda, passa o ponto para a lista de y da esquerda
		if (ord_nxt[i]->x == mid_x && ord_nxt[i] != ord_x[mid])
		{			
			if (ord_nxt[i]->y<ord_x[mid]->y)
			{
				ord_y_l[l] = ord_nxt[i];
				l++;
			}
			else
			{
				ord_y_r[r] = ord_nxt[i];
				r++;
			}
		}
		else if ((ord_nxt[i]->x < mid_x|| ord_nxt[i] == ord_x[mid]) && l<size_l )
		{
			ord_y_l[l] = ord_nxt[i];
			l++;
		}
	//se o ponto esta na direita, passa o ponto para a lista de y da direita
		else 
		{
			ord_y_r[r] = ord_nxt[i];
			r++;
		}
	}
	for (int i =0; i< size; i++)
	{
		ord_nxt[i] = ord_y[i];
	}
	//-----------------------------------------------------
	//chamadas recursivas
	d_l = recursivo (ord_x, ord_y_l, size_l, &left_a, &left_b);
	d_r = recursivo ((&ord_x[size_l]), ord_y_r, size_r, &right_a, &right_b);
	//-----------------------------------------------------
	//**************************************************************************
	//CONQUISTA
	//verifica qual dos lados retorna a menor distancia e atualiza os valores de acordo
	if (d_l <= d_r)
	{
		d = d_l;
		*first = left_a;
		*second = left_b;
	}
	else
	{
		d = d_r;
		*first = right_a;
		*second = right_b;
	}
	//-----------------------------------------------------
	//Atualiza os vetores em y de cadalado para possuir apenas os que estao na regiao
	//delimitada por d a partir do x central	
	ord_y_l = ord_nxt;
	ord_y_r = &(ord_nxt[size_l]);
	l=0;
	for (int i =0; i< size_l; i++)
	{
		if (ord_y_l[i]->x >= mid_x-d && ord_y_l[i]->x <= mid_x+d)
		{
			ord_y_l[l] = ord_y_l[i];
			l++;
		}
	}
	r=0;
	for (int i =0; i< size_r; i++)
	{
		if (ord_y_r[i]->x >= mid_x-d && ord_y_r[i]->x <= mid_x+d)
		{
			ord_y_r[r] = ord_y_r[i];
			r++;
		}
	}
	//-----------------------------------------------------
	//procura os pontos que fazem parte da "hitbox" de cada ponto
	int minj = 0;	
	for (int i = 0; i<l; i++)
	{
		for (int j = minj; j<r; j++)
		{
			//como ambos os vetores são crescentes, se um ponto ja ficou par atras,
			//nao precisa ser utilizado novamente
			if (ord_y_r[j]->y< ord_y_l[i]->y-d)
			{
				minj++;
				continue;
			}
			//se for crescente, nao adianta continuar percorrendo com esse i
			if (ord_y_r[j]->y> ord_y_l[i]->y+d)
			{
				j--;
				break;
			}
			//se passou das etapas anteriores, j esta na "hitbox" de x
			//verifica se a distancia entre eles é menor que a minima
			float dist_mid = distance(ord_y_l[i], ord_y_r[j]);
			if (dist_mid < d)
			{
				d = dist_mid;
				*first = ord_y_l[i];
				*second = ord_y_r[j];
			}
		}
	}
	//-----------------------------------------------------*/
	free(ord_nxt);
	return d;
}
