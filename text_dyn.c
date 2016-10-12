#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include "strings_text_v1.h" //Eget funktionsbibliotek
#define N 300
#define TEXTFIL "spel_default.txt" //Standardtextfil med "Bj�rnspelet"
#define L "\n-----------------------------------------------------------------\n"
#define TEXTHASTIGHET 3 //s�tt till 0 f�r instant text
#define LT 11  //antal tecken innan text som ska visas i textfil V1001|2100|Text som ska visas

/*
Spelmotor f�r text�ventyr med extern textfil

Version 2016-10-10
Initial release
Spelet fungerar utifr�n textfilen.
Spelet kan avslutas utifr�n textfilens val
Check f�r inmatning av val, med do-sats
Debug-switch

Version 2016-10-11
Radbrytningar i text, dessa triggas med tecknet | i textfilen
Funktion f�r switchar
Test f�r om textfilen existerar
Tagit bort on�dig variabel "bokstav", beh�vs ej f�r att f�nga upp f�rsta bokstaven i textraderna
Det g�r att starta spelet med en textfil som argument, d� laddas denna fil ex: 'text_dyn.exe piratspel.txt'
Namngivning av program: SPELMOTOR TXT
Borttagning av on�diga deklarerade variabler som inte anv�ndes.

Version 2016-10-12
St�d f�r titel av spel, skrivs p� �versta raden i textfilen
St�d f�r omspel J/N
Fler sifferkoder tillagda f�r f�rgbyte vid avslut av spel

Att g�ra:
Kommentera koden
Fler koder f�r TXXX|YYYY| D�r Y kan vara f�rgbyte, pip, omstart-fr�ga etc 9998 9997 l�mpligen...
Spellogo, ex L0000| ASCII ART
PROBLEM INMATNING AV ICKESIFFRA
TITEL MANUAL!!!!!!

*/
int listaVal(int a, FILE *f);
void skrivUtText(char *string, int n, _Bool linjer);
_Bool textSwitchar(int s);

//s�tt till 1 f�r att se diverse info fr�n funktionen
_Bool debugMode = 0;

int main(int argc, char *argv[]){
	FILE * textfil = fopen(argv[1], "r");
	char s[N], filnamn[N], restart = 'j';
	int siffra, idCheck, switchCheck;
	if(textfil == NULL){
		//Programmet har inte f�tt en textfil som argument, anv�nder standard.
		textfil = fopen(TEXTFIL, "r");
		}
	//Om standard-textfilen inte har kunnat l�sas:
	if(textfil == NULL){
		printf("Standardfilen '%s' kunde inte hittas. \nAnge filnamn f�r textfil eller mata in EOF (Ctrl+Z) f�r att avbryta: ", TEXTFIL);
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
		TTS(s, N, textfil); //L�ser in textfilens f�rsta rad (som ska vara spelets titel) till variabel 's'
		printf("%sV�lkommen till SPELMOTOR TXT!\nSpel som kommer k�ras �r: %s %s", L, s, L);
		system("pause");
		system("cls");
		while(TTS(s, N, textfil)){
			sscanf(s+1, "%d", &idCheck);
			if(s[0] == 'T' && idCheck == siffra){
				skrivUtText(s+LT, strlen(s)-LT, 1);
				sscanf(s+6,"%d", &switchCheck);
				if(!textSwitchar(switchCheck)){
					skrivUtText("Spelet �r Slut|Tack!", 20, 1);
					break;
				}
				siffra = listaVal(siffra, textfil);
				rewind(textfil);
			} 
		}
	rewind(textfil);
	clearBuffer();
	printf("Tryck J f�r att spela igen: ");
	restart = getchar();
	system("color 7"); //�terst�ller textf�rgen till vit
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
	if(debugMode){printf("\nFunktion r�knare: %d", valraknare);}
	//Do-sats som upprepas om anv�ndaren inte matar in ett korrekt val.
	do{
		if(korrektVal){
			printf("%sAnge val genom at sl� in en siffra: ",L);
		}
		else {
			printf("%sFel val!\nAnge val genom at sl� in en siffra: ",L);
		}
		scanf("%d", &val);
		if(debugMode){printf("\nval Input do-sats: %d, r�knare: %d", val, valraknare);}
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
	//break; beh�vs inte f�r switch-satsen inte funktionen avslutas n�r returv�rde skickas.
	switch(s){
		case 9999:
			//Spelet avslutas
			return 0; 
		case 9996:
			//Spelet avslutas och texten byter f�rg till bl�
			system("color 09");
			return 0;
		case 9997:
			//Spelet avslutas och texten byter f�rg till gr�n
			system("color 02");
			return 0;
		case 9998:
			//Spelet avslutas och texten byter f�rg till r�tt
			system("color 04");
			return 0;
		case 0000:
			return 1;
		default:
			return 1;
			break;
	}
}