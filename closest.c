#include "closest.h"

typedef struct 
{
	float x;
	float y;
}Ponto;

int main ()
{
	int size = 10;
	Ponto* inputs;
	FILE *f_inputs;
	if ((f_inputs = fopen(INPUTS, "r")) == NULL)
	{
			return -1;
	}
	printf ("size: %d\n", size);
	size = fscanf(f_inputs, "%d", &size);
	printf ("size: %d\n", size);
	size = 9;
	inputs = (float*) malloc (size * sizeof(Ponto));
	for (int i = 0; i< size; i++)
	{
		fscanf(f_inputs, "%f", &(inputs[i].x));
		fscanf(f_inputs, "%f", &(inputs[i].y));
	}	
	for (int i = 0; i< size; i++)
	{
		printf("ponto %d : (%f, %f)\n",i, inputs[i].x, inputs[i].y);
	}
	free (inputs);
	

}

