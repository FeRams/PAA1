#include <stdio.h>
#include <stdlib.h>
#include<ctype.h>
#include<string.h>
#include<time.h>
#include <math.h>
#include <float.h>
#define INPUTS "input.txt"
#define OUTPUTS "output.txt"


typedef struct 
{
	float x;
	float y;
}Ponto;

float distance (Ponto* first, Ponto* second);

float forca_bruta (Ponto* inputs, int size, Ponto** first, Ponto** second);

float divisao_e_conquista (Ponto* inputs, int inicio, int fim, Ponto** first, Ponto** second);
