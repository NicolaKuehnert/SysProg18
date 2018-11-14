#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main()
{
	printf("Aufgabe 2: while\n\n");
	float sinus = 0;
	int grad = 0;
	double rad = 0;

	while(rad<=6.3)
	{
		sinus = sin(rad);
		printf("Grad: %d, Sinus: %.3lf\n", grad, sinus);
		grad += 10;
		rad+=0.175;
		
	}
}
