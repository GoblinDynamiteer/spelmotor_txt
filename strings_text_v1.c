#include "strings_text_v1.h"

/*TESTVERSION 1
2016-10-05
behöver uppsnyggning

Funktioner för inläsning av text och från textfil till textsträng

Som författarens i boken, men jag ville bygga dem själv för
att fatta vad som händer med dem, jag valde också andra namn

*/

_Bool removeNewLine(char a[]){
	int i = strlen(a) -1; //näst sista bokstaven i strängen
	if (a[i] == '\n'){
		a[i] = '\0';
		return 1;
	}
	//printf("\nTest: removeNewLine returnerar 0, det kan finnas text i buffer!\n");
	return 0;
}

/* En annan funktion författaren använder (Sida 190) 
Om man skriver in en längre text än x i fgets(string, x, stdin) så läggs
resterande i en buffert som läggs in i nästa inläsning, om man använder denna funktion
så rensas bufferten */
void clearBuffer(void){
	int i;
	while ((i = getchar()) != '\n' && i != EOF)
		;
}

//Liknande funktionen clearBuffer, fast för läsning från textfil
void clearBufferFil(FILE *f){
	int i;
	while ((i = fgetc(f)) != '\n' && i != EOF)
		;
}

/* för att slippa använda
	if (!removeNewLine(sträng))
		clearBuffer();
efter varje inläsning med fgets har författaren byggt en egen inläsningsfunktion (Sida 200)

Funktionen ger returvärdet 1 vid en lyckad inläsning, annars 0 (vid EOF)
den får parametrarna: strängen som ska läsas in, och 'n' för fgets(sträng, n, stdin)
*/
_Bool radInput(char a[], int n){
	//fgets returnerar NULL vid misslyckad inläsning
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

//Funktion som läser text från textfil till textsträng, returnerar 0 vid avslut
_Bool textfilTillString(char *a, int n, FILE *f){
	if (fgets(a, n, f) == NULL){
		return 0;
	}
	if (!removeNewLine(a)){ 
		clearBufferFil(f);
	}
	return 1;
}