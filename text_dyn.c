#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include "strings_text_v1.h" //Eget funktionsbibliotek
#define N 300
#define TEXTFIL "text_spel.txt" //Standardtextfil med "Björnspelet"
#define L "\n-----------------------------------------------------------------\n"
#define TEXTHASTIGHET 3 //sätt till 0 för instant text
#define LT 11  //antal tecken innan text som ska visas i textfil V1001|2100|Text som ska visas

/*
Spelmotor för textäventyr med extern textfil

Version 2016-10-10
Fixat: 
Initial release
Spelet fungerar utifrån textfilen.
Spelet kan avslutas utifrån textfilens val

Version 2016-10-10v2
Check för inmatning av val, med do-sats

Version 2016-10-10v3
Debug-switch

Version 2016-10-11
Radbrytningar i text, dessa triggas med tecknet | i textfilen
Funktion för switchar
Test för om textfilen existerar
Tagit bort onödig variabel "bokstav", behövs ej för att fånga upp första bokstaven i textraderna
Det går att starta spelet med en textfil som argument, då laddas denna fil ex: 'text_dyn.exe piratspel.txt'

Manual:
Textfilen ska vara i detta format:

T1000|0000|INITIAL TEXT VISAS I SPELETS BÖRJAN
V1001|2100|VAL1 FÖR TEXT T1000
V1002|2200|VAL2 FÖR TEXT T1000
V1003|2300|VAL3 FÖR TEXT T1000

T2100|0000|TEXT FÖR VAL 1 FRÅN TEXT1000
T2101|4000|VAL1 FÖR TEXT T2100

osv

V står för Val
T för Text
Sifferlängd måste vara fyra, utan inledande nollor

Val kan gå till samma text.
Spelet avslutas genom att ange 9999 efter TXXX
Ex:
T4201|9999|BRA VAL, DU VANN!


Att göra:
Enklare utforming av textfilens utförande?
Kommentera koden
Fler koder för TXXX|YYYY| Där Y kan vara färgbyte, pip, omstart-fråga etc 9998 9997 lämpligen...
Spellogo, ex L0000| ASCII ART

*/
int listaval(int a, FILE *f);
void skrivUtText(char *string, int n, _Bool linjer);
void textSwitchar(int s);

//sätt till 1 för att se diverse info från funktionen
_Bool debugMode = 0;

int main(int argc, char *argv[]){
	FILE * textfil = fopen(argv[1], "r");
	if(textfil == NULL){
		//Programmet har inte fått en textfil som argument, använder standard.
		textfil = fopen(TEXTFIL, "r");
		}
	//Om standard-textfilen inte har kunnat läsas:
	if(textfil == NULL){
		printf("Standardfilen '%s' kunde inte hittas. Avslutar", TEXTFIL);
		exit(1);
		}
	char s[N], text[N], valtext[N];
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
	if(debugMode){printf("\nFunktion räknare: %d", valraknare);}
	do{
		if(korrektVal){
			printf("%sAnge val genom at slå in en siffra: ",L);
		}
		else {
			printf("%sAnge val genom at slå in en KORREKT siffra: ",L);
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
		//TEXTHASTIGHET definieras som macro i kodens början
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
			//printf("%sSpelet är slut! - Tack!%s",L,L);
			skrivUtText("Spelet är Slut|Tack!", 20, 1);
			exit(0);
			break;
		case 9998:
			//för test    ---- temporär
			system("color 02");
			break;
		default:
			break;
	}
}