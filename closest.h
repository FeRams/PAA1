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

void merge(Ponto* vectorPoints, int p, int q, int r);

void merge_sort(Ponto* vectorPoints, int p, int r);

float divisao_e_conquista (Ponto* inputs, int size, Ponto** first, Ponto** second);

float recursivo (Ponto** ord_x, int size, Ponto** ord_y, Ponto** first, Ponto** second);
