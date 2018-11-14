#include <stdlib.h>
#include <stdio.h>

int main(){
	int n=0;
	int i=0;
	int j=0;
	int k=0;
printf("\nAufgabe 3: Pyramieden.\n\nBitte geben sie eine Zahl ein\n");
	scanf("%i",&n);
	printf("\nAufgabenteil 1: n Sterne in einer Reihe\n");
	for(i=0;i<n;i++){
		printf("*");
	}
	printf("\n\nAufgabenteil 2: linksbündige Pyramide\n");
	for(i=0;i<=n;i++){
		for(j=0;j<i;j++){
			printf("*");
		}
		printf("\n");
	}
	printf("\n\nAufgabenteil 3: Pyramide\n");
	if(n %2 ==0){
		printf("Grade Zahl. Es wird eine ungerade Benötigt. Zahl wird um 1 erhöt.");
		n=n+1;
	}
	for(i=0;i<=(n/2+1);i++){
		for(j=n-i;j>0;j--){
			printf(" ");
		}
		for(k=0;k<((i*2)-1);k++){
			printf("*");
		}		
		printf("\n");
		}
	
	
	return 0;
}
