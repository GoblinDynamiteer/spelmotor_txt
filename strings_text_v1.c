#include "strings_text_v1.h"

/*TESTVERSION 1
2016-10-05
beh�ver uppsnyggning

Funktioner f�r inl�sning av text och fr�n textfil till textstr�ng

Som f�rfattarens i boken, men jag ville bygga dem sj�lv f�r
att fatta vad som h�nder med dem, jag valde ocks� andra namn

*/

_Bool removeNewLine(char a[]){
	int i = strlen(a) -1; //n�st sista bokstaven i str�ngen
	if (a[i] == '\n'){
		a[i] = '\0';
		return 1;
	}
	//printf("\nTest: removeNewLine returnerar 0, det kan finnas text i buffer!\n");
	return 0;
}

/* En annan funktion f�rfattaren anv�nder (Sida 190) 
Om man skriver in en l�ngre text �n x i fgets(string, x, stdin) s� l�ggs
resterande i en buffert som l�ggs in i n�sta inl�sning, om man anv�nder denna funktion
s� rensas bufferten */
void clearBuffer(void){
	int i;
	while ((i = getchar()) != '\n' && i != EOF)
		;
}

//Liknande funktionen clearBuffer, fast f�r l�sning fr�n textfil
void clearBufferFil(FILE *f){
	int i;
	while ((i = fgetc(f)) != '\n' && i != EOF)
		;
}

/* f�r att slippa anv�nda
	if (!removeNewLine(str�ng))
		clearBuffer();
efter varje inl�sning med fgets har f�rfattaren byggt en egen inl�sningsfunktion (Sida 200)

Funktionen ger returv�rdet 1 vid en lyckad inl�sning, annars 0 (vid EOF)
den f�r parametrarna: str�ngen som ska l�sas in, och 'n' f�r fgets(str�ng, n, stdin)
*/
_Bool radInput(char a[], int n){
	//fgets returnerar NULL vid misslyckad inl�sning
	if (fgets(a, n, stdin) == NULL){
		return 0;	
	}
	/*om removeNewLine returnerar 0 (!inverterat) finns det text kvar i buffert, 
	och clearBuffer-funktionen anropas */
	if (!removeNewLine(a)){ 
		clearBuffer();	
	}
	return 1;
}

_Bool TTS(char *a, int n, FILE *f){
	return textfilTillString(a, n, f);
}

//Funktion som l�ser text fr�n textfil till textstr�ng, returnerar 0 vid avslut
_Bool textfilTillString(char *a, int n, FILE *f){
	if (fgets(a, n, f) == NULL){
		return 0;
	}
	if (!removeNewLine(a)){ 
		clearBufferFil(f);
	}
	return 1;
}