#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main()
{
	printf("Aufgabe2: for\n\n");
	float sinus = 0;
	int grad = 0;

	for(double rad=0; rad<=6.3; rad+=0.175)
	{
		sinus = sin(rad);
		printf("Grad: %d, Sinus: %.3lf\n", grad, sinus);
		grad += 10;
		
	}
	return 0;
}
