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
	printf ("size: %d\n", size);
	size = fscanf(f_inputs, "%d", &size);
	printf ("size: %d\n", size);
	size = 100;
	//aloca o espaco para o vetor de entrada
	inputs = (float*) malloc (size * sizeof(Ponto));
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
	Ponto** left = (float*) malloc ( (sizeLeft+1) * sizeof(Ponto*));
	Ponto** rigth = (float*) malloc ( (sizeRigth+1) * sizeof(Ponto*));
	
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
				if(left[posL]->x > rigth[posR]->x)
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
	float dist;
	Ponto** ord_x, **ord_y;
	if (size <=1)
	{
		*first = NULL;
		*second = NULL;
		return FLT_MAX;
	}
	ord_x = (Ponto**) malloc (size * sizeof(Ponto*));
	ord_y = (Ponto**) malloc (size * sizeof(Ponto*));
	for (int i = 0; i < size;i++)
	{
		ord_x[i] =  &inputs[i];
		ord_y[i] =  &inputs[i];
	}
    merge_sort(ord_x, 0, size-1, 'x');
	for (int i = 0; i< size; i++)
	{
		printf("ponto %d : (%f, %f)\n",i, ord_x[i]->x, ord_x[i]->y);
	}
    merge_sort(ord_y, 0, size-1, 'y');
    dist = recursivo (ord_x, ord_y, size, first, second);
    dist = sqrt(dist);
    return dist;
}

float recursivo (Ponto** ord_x, Ponto** ord_y, int size, Ponto** first, Ponto** second)
{
	if (size <= 3)
	{
		int dist  = FLT_MAX;
		for (int i=0;i<size-1;i++)
		{
			for(int j=i+1; j<size; j++)
			{
				int aux = distance (ord_x[i], ord_x[j]);
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
	int mid = (size-1)/2;
	int size_l = (size+1)/2, size_r = size/2;
	float d= FLT_MAX, d_l = FLT_MAX, d_r= FLT_MAX;
	float mid_x = ord_x[mid]->x;
	Ponto** ord_y_l, **ord_y_r;
	Ponto* left_a , *left_b, *right_a, *right_b;
	ord_y_l = (Ponto**) malloc (size_l * sizeof(Ponto*));
	ord_y_r = (Ponto**) malloc (size_r * sizeof(Ponto*));
	int l =0, r = 0;
	for (int i =0; i< size; i++)
	{
		if ((ord_y[i]->x <= mid_x|| ord_y[i] == ord_x[mid]) && l<size_l )
		{
			ord_y_l[l] = ord_y[i];
			l++;
		}
		else 
		{
			ord_y_r[r] = ord_y[i];
			r++;
		}
	}
	//printf ("min: %f, mid: %f, max: %f\n", ord_x[0]->x, mid_x, ord_x[size-1]->x);
    /*printf(" l: %d, r:%d", l, r);
    scanf("%d", &aqui);
	for (int i = 0; i< size_r; i++)
	{
		printf("ponto %d : (%f, %f)\n",i, ord_y_l[i]->x, ord_y_l[i]->y);
	}*/
	d_l = recursivo (ord_x, ord_y_l, size_l, &left_a, &left_b);
	d_r = recursivo ((&ord_x[size_l]), ord_y_r, size_r, &right_a, &right_b);
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
	int minj = 0;
	/*if (mid_x > 6808.0 && mid_x <= 6809.0)
	{
		printf ("min: %f, mid: %f, max: %f, l:%f\n", ord_x[0]->x, mid_x, ord_x[size-1]->x, d_l);
		for (int i = 0; i< size; i++)
		{
			printf("ponto %d : (%f, %f)\n",i, ord_x[i]->x, ord_x[i]->y);
		}
		for (int i = 0; i< size_l; i++)
		{
			printf("ponto %d : (%f, %f)\n",i, ord_y_l[i]->x, ord_y_l[i]->y);
		}
		for (int i = 0; i< size_r; i++)
		{
			printf("ponto %d : (%f, %f)\n",i, ord_y_r[i]->x, ord_y_r[i]->y);
		}
	}*/
	for (int i = 0; i<l; i++)
	{
		for (int j = minj; j<r; j++)
		{
			if (ord_y_r[j]->y< ord_y_l[i]->y-d)
			{
				minj++;
				continue;
			}
			if (ord_y_r[j]->y> ord_y_l[i]->y+d)
			{
				j--;
				break;
			}
			float dist_mid = distance(ord_y_l[i], ord_y_r[j]);
			if (dist_mid < d)
			{
				//printf("mid:%f\n", dist_mid);
				d = dist_mid;
				*first = ord_y_l[i];
				*second = ord_y_r[j];
			}
		}
	}
	return d;
}