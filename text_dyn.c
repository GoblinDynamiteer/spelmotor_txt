#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include "strings_text_v1.h" //Eget funktionsbibliotek innehållande diverse text- och textfil-funktioner

#define N 300	//Längd för char-variabler, och parameter till vissa funktioner
#define TEXTFIL "spel_default.txt" //Standardtextfil med "Björnspelet"
#define L "\n-----------------------------------------------------------------\n" //För utskrift av linjer med printf
#define TEXTHASTIGHET 3		//Antal millisekunder att pausera mellan varje teckenutskrift i funktionen skrivUtText
#define LT 11	//Antal tecken på rad i textfilen, innan text som ska visas, ex: "V1001|2100|Text som ska visas"

/*
SPELMOTOR TXT

2016-10-16
Experiment
BRANCH: Inläsning av hel textfil till struct - Test

Program för att köra speläventyr från textfiler.
Av Johan Kämpe

Ändringslogg:
2016-10-10
Initial release
Testspel fungerar att köras från textfil.
Spelet kan avslutas
Kontroll tillagt för användarinmatning vid val
Debug-funktionalitet tillagd

2016-10-11
Radbrytningar i text, dessa triggas med tecknet | i textfilen
Funktion för switchar
Test för om textfilen existerar
Tagit bort onödig variabel "bokstav", behövs ej för att fånga upp första bokstaven i textraderna
Det går att starta spelet med en textfil som argument, då laddas denna fil ex: 'text_dyn.exe piratspel.txt'
Namngivning av program: SPELMOTOR TXT
Borttagning av onödiga deklarerade variabler som inte användes.

2016-10-12
Stöd för titel av spel, som ska/måste skrivas på rad 1 i textfilen.
Stöd för omspel, användaren kan trycka på J/j efter avslutat spel för att spel om.
Fler sifferkoder tillagda för färgbyte vid avslut av spel.

2016-10-13
Ändrat ordning på laddning av textfiler
Lagt in system("chcp 1252") för att byta teckenkodning, så att svenska tecken visas korrekt.
Uppdaterat debug-texter
Korrigerat grammatik och stavfel i text.
Lagt till tre nya kommandon för att byta textfärg, utan att avsluta spelet
Flertalet namnbyten av variabler

BRANCH -------
2016-10-16
Experiment
Inläsning av hel textfil till struct - Test

*/

//Deklaration av funktioner som finns i denna fil:
int listaVal(int a, FILE *f);
void skrivUtText(char *string, int n, _Bool linjer);
_Bool textSwitchar(int s);

//Debug-läge. Sätt till 1 för att få extra information utskriven från programmet
_Bool debugMode = 0;


struct val{
	char text[N];
	int nastaText;
	int idNum;
};

struct aventyr{
	int idNum, antalVal, kod;
	char text[N];
	_Bool AvslutaSpel;
};

//strcpy(valText.valTextArray[0], "Test");

//Parametern argv för mainfunktionen ges från kommandopromten med "spelmotortxt.exe PARAMETER"
int main(int argc, char *argv[]){
	/* Öppnar värdet i argv[1] som textfil till variabel textfil 
	argv[0] är programmets namn */
	FILE * textfil = fopen(argv[1], "r");
	char s[N], filnamn[N], restart = 'j';
	int idNum, idCheck, switchCheck, T_Counter = 0, V_Counter = 0;
	/* Sätter teckenkodning till 1252, för korrekt utskrift av svenska tecken.
	Blankar sedan skärmen */
	system("chcp 1252");
	system("cls");
	/*skrivUtText(s,n,l) --> Skriver ut n antal tecken av strängen s. 
	Om l == 1 skrivs "linjer" ut innan och efter texten (macro L). */
	skrivUtText("SPELMOTOR TXT", 13, 1);

	if(textfil == NULL){ 
		/* Programmet har inte fått en textfil som argument, eller har inte kunnat hitta/läsa textfilen i argumentet. 
		Användaren uppmanas att skriva in ett namn för textfil att spela med. */
		printf("Ange filnamn (med filändelse) för textfil.\nMata in EOF (Ctrl+Z) för att avbryta\nOm ingen fil anges laddas %s\n\nTextfil: ",TEXTFIL);
		//radInput finns i strings_text_v1.c, och beskrivs där.
		if(radInput(filnamn, N)){
			textfil = fopen(filnamn, "r");
		}
		else {
			printf("Programmet avslutas!");
			exit(0);
		}
	}

	if(textfil == NULL){ 
		//Om inmatad fil från använder inte kan läsas försöker programmet ladda in standard-text definierad i macrot TEXTFIL
		printf("Inmatad fil: '%s' kunde inte läsas.\nLäser in standard-spel %s", filnamn, TEXTFIL);
		textfil = fopen(TEXTFIL, "r");
	}

	if(textfil == NULL){
		//Om TEXTFIL inte kunde läsas in avslutas programmet
		printf("Standardfilen '%s' kunde inte hittas. Programmet avlutas", TEXTFIL);
		return 0;
	}
	
	/* Spelet kommer startas om så länge användaren trycker på j/J efter avslutat spel
	tolower() används för att konvertera ett tecken till en gemen */
	while(tolower(restart) == 'j'){
		/* idNum sätts till 1000 för att skriva ut spelets första val, som ska vara just 1000 */
		idNum = 1000;
		//TTS finns i strings_text_v1.c, och beskrivs där. Textfilens första rad, som ska vara spelets titel, läses till variabeln s.
		TTS(s, N, textfil);
		printf("%sSpel som kommer köras är: %s %s", L, s, L);
		//Programmet pauserar tills användaren trycker på en valfri tangent, sedan blankas skärmen
		system("pause");
		system("cls");
		//Letar upp rader som börjar med ett T
		while(TTS(s, N, textfil)){
			if(s[0] == 'T'){
				T_Counter++;
			}
			if(s[0] == 'V'){
				V_Counter++;
			}
		}
	printf("Hittade %d rader med 'T' & %d rader med 'V'\n", T_Counter, V_Counter);
	//Sätter array-structar beroede av hur många rader texter & val som hittades textfilen
	struct aventyr texter[T_Counter];
	struct val vtexter[V_Counter];
	rewind(textfil);
	TTS(s, N, textfil); //För att hoppa över spelets titel på första raden
	int t = 0, v = 0;
	while(TTS(s, N, textfil)){
		if(s[0] == 'T'){
			//Sätter idNummer & sifferkod till struct
			sscanf(s+1, "%d", &texter[t].idNum);
			sscanf(s+6, "%d", &texter[t].kod);
			//Skriver radens text till struct
			strcpy(texter[t].text, s+LT);
			//Sätter antal val som hör till texten initialt till 0
			texter[t].antalVal = 0;
			//Läser nästa textrad i filen
			TTS(s, N, textfil); //Nästa rad i textfilen, som kan vara ett val
			while(s[0] == 'V'){ //Körs medan raden är ett val
				sscanf(s+1, "%d", &vtexter[v].idNum);
				sscanf(s+6, "%d", &vtexter[v].nastaText);
				strcpy(vtexter[v].text, s+LT);
				TTS(s, N, textfil);
				v++; //För nästa "val"-struct i arrayen
				//Ökar antal val som hör till text med 1
				texter[t].antalVal++;
			}
		t++; //För nästa "text"-struct i arrayen
		}
	}
	for(int k=0;k<T_Counter;k++){
		printf("texter[%d].idNum = %d\n", k, texter[k].idNum);
		printf("texter[%d].antalVal = %d\n", k, texter[k].antalVal);
	}
	for(int k=0;k<V_Counter;k++){
		// printf("vtexter[%d].idNum = %d\n", k, vtexter[k].idNum);
		// printf("vtexter[%d].nastaText = %d\n", k, vtexter[k].nastaText);
		// printf("vtexter[%d].text = %s\n", k, vtexter[k].text);
	}
	}
	return 0;
}


/* Funktionen listaVal skriver ut de val som finns för det aktuella scenariot i spelet. 
Som parametrar får funktionen textens identifikationsnummer som en int-variabel, 
samt den inlästa textfilen
Funktionen kontrollerar att användaren matar in en korrekt siffra för val, 
och returnerar sedan det identifikationsnummer som tillhör nästa text som ska skrivas ut. */
int listaVal(int a, FILE *f){
	int valRaknare = 1, checkVal, anvandareVal = 0, nastaIdNum;
	_Bool korrektVal = 1;
	char s[N]; //Likt variabeln s i main-funktionen
	while(TTS(s, N, f)){
		//Läser in den aktuella radens identifikationsnummer till checkVal
		sscanf(s+1, "%d", &checkVal);
		/* Jämför identifikationsnummer i checkVal med identifikationsnummer i funktionens parameter 'a'
		adderat med värdet i valRaknare, som börjar på 1.
		Ex: Om texten som visats har identifikationsnummer T1000, så ska valen ha V1001, V1002 osv. */
		if(s[0] == 'V' && checkVal == a+valRaknare){
			/* Skriver ut val-siffra för inmatning från användare, efter skrivs valets text och nyradstecken
			valRaknare ökar med 1 för att kontrollera om det finns fler val. */
			printf("[%d] ", valRaknare);
			skrivUtText(s+LT, strlen(s)-LT, 0);
			printf("\n");
			valRaknare++;
		}
	}
	valRaknare--; //Backar valRaknare med 1 (sista valet hittades inte)
	if(debugMode){printf("\n[Funktion listaVal - valräknare värde: %d]\n", valRaknare);}
	//Do-sats som upprepas om användaren inte matar in ett korrekt val.
	do{
		if(korrektVal){ //Triggar alltid första gången
			printf("%sAnge val genom at slå in en siffra: ",L);
		}
		else {
			printf("%sFel val!\nAnge val genom at slå in en siffra: ",L);
		}
		scanf("%d", &anvandareVal);
		//korrektVal sätts till 0, för att skriva ut "Fel val!" ovan, om användaren skulle skriva in ett felaktigt val
		korrektVal = 0; 
	//Det inmatade värdet från användaren ska vara mellan 1 och värdet i valRaknare
	} while(anvandareVal <=0 || anvandareVal > valRaknare);

	rewind(f);
		while(TTS(s, N, f)){
		/* checkVal sätts till den aktuella radens identifikationsnummer och jämförs 
		med identifikationsnummer i funktionens parameter 'a' adderat med användarinmatningen 
		i anvandareVal */
		sscanf(s+1, "%d", &checkVal);
		//Val-rader i textfilen ska börja med tecknet 'V'
		if(s[0] == 'V' && checkVal == a+anvandareVal){
			//När korrekt rad hittas sätts nastaIdNum till identifikationsnumret för nästa rad som ska användas i spelet
			sscanf(s+6,"%d", &nastaIdNum);
		}
	}
	if(debugMode){printf("\n[Funktion listaVal returnerar: %d]\n", nastaIdNum);}
	return nastaIdNum;
}

/* Funktionen skrivUtText skriver ut text till skärmen, ett tecken i taget. 
Mellan varje teckenutskrift sker en fördröjning på lika många millisekunder 
som macrot TEXTHASTIGHET är definierat till. 

Som parametrar får funktionen textsträngen som ska skrivas ut, en int-variabel 
med antal tecken som ska skrivas ut. 

_Bool-variabeln linjer bestämmer om funktionen ska skriva ut ”rader” i  början och slutet av textblocken. */
void skrivUtText(char *string, int n, _Bool linjer){
	if(linjer){
		printf("%s", L);
	}
	//for-loop som kör 'n' antal varv
	for(int i=0;i<n;i++){
		//Om tecknet '|' hittas i textsträngen skrivs ett nyradstecken ut i stället.
		if (string[i] == '|'){
			printf("\n");
		}
		else{
			//putchar skriver ut ett tecken
			putchar(string[i]);
		}
		//Pauserar programmet TEXTHASTIGHET millisekunder
		Sleep(TEXTHASTIGHET);
	}
	if(linjer){ 
		printf("%s", L);
	}
}

/*
Funktionen textSwitchar används för att köra vissa kommandon som kan användas i spelet. 
Som t.ex. att avsluta spelet eller att byta färg på texten. 
Som parametrar får funktionen den sifferkod som finns efter den aktuella textradens identifikationsnummer. 
Funktionen returnerar 1 eller 0 beroende på om spelet ska avslutas eller inte. */
_Bool textSwitchar(int s){
	if(debugMode){printf("[Switch-funktion: Nummer: %d]\n", s);}
	//break; behövs inte för switch-satsen, funktionen avslutas när returvärde skickas.
	switch(s){
		case 9999:
			//Spelet avslutas.
			return 0; 
		case 9993:
			//Spelets text byter färg till blå.
			system("color 09");
			return 1;
		case 9994:
			//Spelets text byter färg till grön.
			system("color 02");
			return 1;
		case 9995:
			//Spelets text byter färg till röd.
			system("color 04");
			return 1;
		case 9996:
			//Spelets text byter färg till blå och spelet avslutas.
			system("color 09");
			return 0;
		case 9997:
			//Spelets text byter färg till grön och spelet avslutas.
			system("color 02");
			return 0;
		case 9998:
			//Spelets text byter färg till röd och spelet avslutas.
			system("color 04");
			return 0;
			//0000 används för att enbart skriva ut texten, utan extra funktionalitet.
		case 0000:
			return 1;
		default:
			return 1;
	}
}