#include <stdio.h>
#include <stdlib.h>
int check(int, int);
int wahr=1; /*Wahr =0; falsch !=0 */
int Zahl=0;
int pos=0;

int main(){
	printf("\n\n\n Bit-Operationen\n\n");	
	printf("Bitte zahl zwischen 0 und 255 eingeben.");
	scanf("%d",&Zahl);
	printf("Bitte Position in der Binärzahl angeben(1-8)");
	scanf("%d",&pos);
	pos=pos-1;
	
	if((Zahl<0)|(Zahl>255)){
		printf("Zahlenbereich verlassen!");
	}
	else if((pos<0)|(pos>8)){
		printf("Position nicht im Wertebereich.");
	}
	else{
		int erg=check(Zahl,pos);
		printf("check ergibt: %d\n",erg);
}
	return 0;
}

int check(int Zahl, int pos){
	int neu=Zahl;
	neu = neu >> pos;
	wahr= neu &1;
	printf("Das bit ist als %d gesetzt\n Zahl ist: %d / Zahl schifted: %d\n",wahr,Zahl,neu);
	
	neu= 1 << pos;
	Zahl=Zahl|neu;
	printf("Das bit an Position %d ist gesetzt.\n Zahl ist %d\n",(pos+1),Zahl);

	neu= ~neu;
	Zahl=Zahl&neu;
	printf("Das bit an Position %d ist gelöscht.\n Zahl ist %d\n",(pos+1),Zahl);

	
	return wahr;
	}
