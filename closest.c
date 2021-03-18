#include "closest.h"


int main ()
{
	/**********************************************/
	int size = 10;
	float dist = 0;
	Ponto* inputs;
	Ponto* menor_a, *menor_b;
	FILE *f_inputs, *f_outputs;	
	/**********************************************/
	if ((f_inputs = fopen(INPUTS, "r")) == NULL)
	{
			return -1;
	}
	if ((f_outputs = fopen(OUTPUTS, "w")) == NULL)
	{
			return -1;
	}
	/**********************************************/
	printf ("size: %d\n", size);
	size = fscanf(f_inputs, "%d", &size);
	printf ("size: %d\n", size);
	size = 10;
	inputs = (float*) malloc (size * sizeof(Ponto));
	/**********************************************/
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
	clock_t begin = clock();
	dist = forca_bruta (inputs, size, &menor_a, &menor_b);clock_t end = clock();
	double time_spent = (double)(end - begin)/1000;	
	/**********************************************/
	printf("menor distancia : %f, entre (%f, %f), e (%f, %f)\n tempo: %lfms",
	 dist, menor_a->x, menor_a->y, menor_b->x, menor_b->y, time_spent);	
	fprintf(f_outputs, "%d %lf\n", size, time_spent);
	/**********************************************/
	fclose (f_outputs);
	fclose (f_inputs);
	free (inputs);

}


float distance (Ponto* first, Ponto* second)
{
	float dist;
	dist = (first->x - second->x)* (first->x - second->x)+ (first->y - second->y)* (first->y - second->y);
	dist = sqrt (dist);
	return dist;
}

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
	return dist;
}