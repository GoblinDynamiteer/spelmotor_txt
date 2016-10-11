#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include "strings_text_v1.h" //Eget funktionsbibliotek
#define N 300
#define TEXTFIL "text_spel.txt" //Standardtextfil med "Bj�rnspelet"
#define L "\n-----------------------------------------------------------------\n"
#define TEXTHASTIGHET 3 //s�tt till 0 f�r instant text
#define LT 11  //antal tecken innan text som ska visas i textfil V1001|2100|Text som ska visas

/*
Spelmotor f�r text�ventyr med extern textfil

Version 2016-10-10
Fixat: 
Initial release
Spelet fungerar utifr�n textfilen.
Spelet kan avslutas utifr�n textfilens val

Version 2016-10-10v2
Check f�r inmatning av val, med do-sats

Version 2016-10-10v3
Debug-switch

Version 2016-10-11
Radbrytningar i text, dessa triggas med tecknet | i textfilen
Funktion f�r switchar
Test f�r om textfilen existerar
Tagit bort on�dig variabel "bokstav", beh�vs ej f�r att f�nga upp f�rsta bokstaven i textraderna
Det g�r att starta spelet med en textfil som argument, d� laddas denna fil ex: 'text_dyn.exe piratspel.txt'
Namngivning av program: SPELMOTOR TXT


Att g�ra:
Enklare utforming av textfilens utf�rande?
Kommentera koden
Fler koder f�r TXXX|YYYY| D�r Y kan vara f�rgbyte, pip, omstart-fr�ga etc 9998 9997 l�mpligen...
Spellogo, ex L0000| ASCII ART

*/
int listaval(int a, FILE *f);
void skrivUtText(char *string, int n, _Bool linjer);
void textSwitchar(int s);

//s�tt till 1 f�r att se diverse info fr�n funktionen
_Bool debugMode = 0;

int main(int argc, char *argv[]){
	FILE * textfil = fopen(argv[1], "r");
	char s[N], text[N], valtext[N], filnamn[N];
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
	printf("%sV�lkommen till SPELMOTOR TXT!\nSpel som kommer k�ras �r: %s %s", L, L, L); //FIXA TITEL I TEXTFIL!!!
	system("pause");

	int siffra = 1000, check, val, switchCheck;
	while(TTS(s, N, textfil)){
		system("cls");
		sscanf(s+1, "%d", &check);
		strncpy(text, s+LT, strlen(s));
		if(s[0] == 'T' && check == siffra){
			skrivUtText(s+LT, strlen(s)-LT, 1);
			sscanf(s+6,"%d", &switchCheck);
			textSwitchar(switchCheck);
			siffra = listaval(siffra, textfil);
			rewind(textfil);
		} 
	}
	return 0;
}

int listaval(int a, FILE *f){
	int valraknare = 1, check, val, nasta;
	_Bool korrektVal = 1;
	char s[N];
	//TTS 'TextfilTillString' finns i strings_text_v1.c
	while(TTS(s, N, f)){
		sscanf(s+1, "%d", &check);
		if(s[0] == 'V' && check == a+valraknare){
			//printf("[%d] %s\n", valraknare, s+5+6);
			printf("[%d] ", valraknare);
			skrivUtText(s+LT, strlen(s)-LT, 0);
			printf("\n");
			valraknare++;
		}
	}
	valraknare--; //OKLART
	if(debugMode){printf("\nFunktion r�knare: %d", valraknare);}
	do{
		if(korrektVal){
			printf("%sAnge val genom at sl� in en siffra: ",L);
		}
		else {
			printf("%sAnge val genom at sl� in en KORREKT siffra: ",L);
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
		//TEXTHASTIGHET definieras som macro i kodens b�rjan
		Sleep(TEXTHASTIGHET);
	}
	if(linjer){ 
		printf("%s", L);
	}
}
void textSwitchar(int s){
	if(debugMode){printf("Switch-funktion: Nummer: %d", s);}
	switch(s){
		case 9999:
			//printf("%sSpelet �r slut! - Tack!%s",L,L);
			skrivUtText("Spelet �r Slut|Tack!", 20, 1);
			exit(0);
			break;
		case 9998:
			//f�r test    ---- tempor�r
			system("color 02");
			break;
		default:
			break;
	}
}