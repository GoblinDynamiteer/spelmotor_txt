#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include "strings_text_v1.h" //Eget funktionsbibliotek
#define N 300
#define TEXTFIL "spel_default.txt" //Standardtextfil med "Björnspelet"
#define L "\n-----------------------------------------------------------------\n"
#define TEXTHASTIGHET 3 //sätt till 0 för instant text
#define LT 11  //antal tecken innan text som ska visas i textfil V1001|2100|Text som ska visas

/*
Spelmotor för textäventyr med extern textfil

Version 2016-10-10
Initial release
Spelet fungerar utifrån textfilen.
Spelet kan avslutas utifrån textfilens val
Check för inmatning av val, med do-sats
Debug-switch

Version 2016-10-11
Radbrytningar i text, dessa triggas med tecknet | i textfilen
Funktion för switchar
Test för om textfilen existerar
Tagit bort onödig variabel "bokstav", behövs ej för att fånga upp första bokstaven i textraderna
Det går att starta spelet med en textfil som argument, då laddas denna fil ex: 'text_dyn.exe piratspel.txt'
Namngivning av program: SPELMOTOR TXT
Borttagning av onödiga deklarerade variabler som inte användes.

Version 2016-10-12
Stöd för titel av spel, skrivs på översta raden i textfilen
Stöd för omspel J/N
Fler sifferkoder tillagda för färgbyte vid avslut av spel

Att göra:
Kommentera koden
Fler koder för TXXX|YYYY| Där Y kan vara färgbyte, pip, omstart-fråga etc 9998 9997 lämpligen...
Spellogo, ex L0000| ASCII ART
PROBLEM INMATNING AV ICKESIFFRA
TITEL MANUAL!!!!!!

*/
int listaVal(int a, FILE *f);
void skrivUtText(char *string, int n, _Bool linjer);
_Bool textSwitchar(int s);

//sätt till 1 för att se diverse info från funktionen
_Bool debugMode = 0;

int main(int argc, char *argv[]){
	FILE * textfil = fopen(argv[1], "r");
	char s[N], filnamn[N], restart = 'j';
	int siffra, idCheck, switchCheck;
	if(textfil == NULL){
		//Programmet har inte fått en textfil som argument, använder standard.
		textfil = fopen(TEXTFIL, "r");
		}
	//Om standard-textfilen inte har kunnat läsas:
	if(textfil == NULL){
		printf("Standardfilen '%s' kunde inte hittas. \nAnge filnamn för textfil eller mata in EOF (Ctrl+Z) för att avbryta: ", TEXTFIL);
		if(radInput(filnamn, N)){
			textfil = fopen(filnamn, "r");
		}
		else{
			printf("Programmet avslutas!");
			exit(0);
		}
	}
	while(tolower(restart) == 'j'){
		siffra = 1000;
		TTS(s, N, textfil); //Läser in textfilens första rad (som ska vara spelets titel) till variabel 's'
		printf("%sVälkommen till SPELMOTOR TXT!\nSpel som kommer köras är: %s %s", L, s, L);
		system("pause");
		system("cls");
		while(TTS(s, N, textfil)){
			sscanf(s+1, "%d", &idCheck);
			if(s[0] == 'T' && idCheck == siffra){
				skrivUtText(s+LT, strlen(s)-LT, 1);
				sscanf(s+6,"%d", &switchCheck);
				if(!textSwitchar(switchCheck)){
					skrivUtText("Spelet är Slut|Tack!", 20, 1);
					break;
				}
				siffra = listaVal(siffra, textfil);
				rewind(textfil);
			} 
		}
	rewind(textfil);
	clearBuffer();
	printf("Tryck J för att spela igen: ");
	restart = getchar();
	system("color 7"); //Återställer textfärgen till vit
	}
	return 0;
}

int listaVal(int a, FILE *f){
	int valraknare = 1, check, val = 0, nasta;
	_Bool korrektVal = 1;
	char s[N];
	while(TTS(s, N, f)){
		sscanf(s+1, "%d", &check);
		if(s[0] == 'V' && check == a+valraknare){
			printf("[%d] ", valraknare);
			skrivUtText(s+LT, strlen(s)-LT, 0);
			printf("\n");
			valraknare++;
		}
	}
	valraknare--;
	if(debugMode){printf("\nFunktion räknare: %d", valraknare);}
	//Do-sats som upprepas om användaren inte matar in ett korrekt val.
	do{
		if(korrektVal){
			printf("%sAnge val genom at slå in en siffra: ",L);
		}
		else {
			printf("%sFel val!\nAnge val genom at slå in en siffra: ",L);
		}
		scanf("%d", &val);
		if(debugMode){printf("\nval Input do-sats: %d, räknare: %d", val, valraknare);}
		korrektVal = 0;
	} while(val <=0 || val > valraknare);

	rewind(f);
		while(TTS(s, N, f)){
		sscanf(s+1, "%d", &check);
		if(s[0] == 'V' && check == a+val){
			sscanf(s+6,"%d", &nasta);
		}
	}
	if(debugMode){printf("\nFunktion returnerar: %d", nasta);}
	return nasta;
}

void skrivUtText(char *string, int n, _Bool linjer){
	if(linjer){
		printf("%s", L);
	}
	for(int i=0;i<n;i++){
		if (string[i] == '|'){
			printf("\n");
		}
		else{
			putchar(string[i]);
		}
		Sleep(TEXTHASTIGHET);
	}
	if(linjer){ 
		printf("%s", L);
	}
}

//Funktionen returnerar 0 om spelet ska avslutas, annars 1
_Bool textSwitchar(int s){
	if(debugMode){printf("Switch-funktion: Nummer: %d", s);}
	//break; behövs inte för switch-satsen inte funktionen avslutas när returvärde skickas.
	switch(s){
		case 9999:
			//Spelet avslutas
			return 0; 
		case 9996:
			//Spelet avslutas och texten byter färg till blå
			system("color 09");
			return 0;
		case 9997:
			//Spelet avslutas och texten byter färg till grön
			system("color 02");
			return 0;
		case 9998:
			//Spelet avslutas och texten byter färg till rött
			system("color 04");
			return 0;
		case 0000:
			return 1;
		default:
			return 1;
			break;
	}
}