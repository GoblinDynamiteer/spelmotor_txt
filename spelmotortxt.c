#include <windows.h>
#include "strings_text_v1.h" //Eget funktionsbibliotek innehållande diverse text- och textfil-funktioner

#define N 1000	//Längd för char-variabler, och parameter till vissa funktioner
#define TEXTFIL "spel_default.txt" //Standardtextfil med "Björnspelet"
#define L "\n-----------------------------------------------------------------\n" //För utskrift av linjer med printf
#define TEXTHASTIGHET 3		//Antal millisekunder att pausera mellan varje teckenutskrift i funktionen skrivUtText
#define LT 11	//Antal tecken på rad i textfilen, innan text som ska visas, ex: "V1001|2100|Text som ska visas"

/*
SPELMOTOR TXT
Program för att köra speläventyr från textfiler.
Av Johan Kämpe

Kommentarer visas över eller till höger om det det beskriver i koden.


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

2016-10-16
Ökade radinläsning (macro N) till 1000 st tecken.
Bytt parameter -> argument i kommentarer
Ny _Bool-parameter för funktion skrivUtText, används för tabbslag i början av text, och efter radbrytning
Ändrade hur utskrift av val ser ut, för läsbarheten skull

2016-10-19
Fixade kontroll för inmatning av användarval, programmet slutar nu inte att fungera om användaren matar in något annat än en siffra
Lade till funktionalitet för att läsa in val från textfil som inte ligger i ordning, eller efter texten de tillhör
Lade till sifferkod 9992, för färgbyte av text till vit

*/

//Deklaration av funktioner som finns i denna fil:
int listaVal(int a, FILE *f);
void skrivUtText(char *string, int n, _Bool linjer, _Bool tabb);
_Bool textSwitchar(int s);

//Debug-läge. Sätt till 1 för att få extra information utskriven från programmet
_Bool debugMode = 0;


//Argumentet argv för mainfunktionen ges från kommandopromten med "spelmotortxt.exe PARAMETER"
int main(int argc, char *argv[]){
	/* Öppnar värdet i argv[1] som textfil till variabel textfil 
	argv[0] är programmets namn */
	FILE * textfil = fopen(argv[1], "r");
	char s[N], filnamn[N], restart = 'j';
	int idNum, idCheck, switchCheck;
	/* Sätter teckenkodning till 1252, för korrekt utskrift av svenska tecken.
	Blankar sedan skärmen */
	system("chcp 1252");
	system("cls");
	/*skrivUtText(s,n,l,t) --> Skriver ut n antal tecken av strängen s. 
	Om l == 1 skrivs "linjer" ut innan och efter texten (macro L). 
	om t == 1 skrivs tabbslag ut i början av texten och efter returslag */
	skrivUtText("SPELMOTOR TXT", 13, 1, 0);

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
		//Om inmatad fil från användaren inte kan läsas försöker programmet ladda in standard-text definierad i macrot TEXTFIL.
		printf("Inmatad fil: '%s' kunde inte läsas.\nLäser in standard-spel %s", filnamn, TEXTFIL);
		textfil = fopen(TEXTFIL, "r");
	}

	if(textfil == NULL){
		//Om TEXTFIL inte kunde läsas in avslutas programmet.
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
		//Programmet pauserar tills användaren trycker på en valfri tangent, sedan blankas skärmen.
		system("pause");
		system("cls");
		while(TTS(s, N, textfil)){
			/* sscanf fungerar likt scanf, fast från en textsträng i stället för användarinmatning 
			Här läses siffervärdet efter det första tecknet i char-variabeln 's' till idCheck */
			sscanf(s+1, "%d", &idCheck);
			//Letar efter rad som börjar på T och har värdet i variabeln 'idNum'
			if(s[0] == 'T' && idCheck == idNum){
				/* När rätt rad hittas skrivs radens text ut med funktionen skrivUtText
				macrot LT är antalet tecken som finns på raden innan texten som ska skrivas ut. */
				skrivUtText(s+LT, strlen(s)-LT, 1, 0);
				/* Läser sifferkoden för den aktuella raden till switchCheck. 
				Funktionen textSwitchar anropas med sifferkoden.
				Funktionen utför olika kommandon beroende på kodens värde 
				Om spelet ska avslutas ger funktionen returvärdet 0, annars 1 */
				sscanf(s+6,"%d", &switchCheck);
				if(!textSwitchar(switchCheck)){
					skrivUtText("Spelet är Slut|Tack!", 20, 1, 0);
					break; //Bryter den inre while-loopen
				}
				/* Funktionen listaVal anropas med idNum och textfilen som parametrar.
				Funktionen returnerar ett nytt värde som idNum sätts till. */
				idNum = listaVal(idNum, textfil);
				//rewind() "spolar" tillbaka textfilen till dess början.
				rewind(textfil);
			}
		}
	rewind(textfil);
	printf("Tryck J för att spela igen: ");
	restart = getchar();
	clearBuffer();
	//Sätter textfärgen till vit
	system("color 7"); 
	}
	return 0;
}


/* Funktionen listaVal skriver ut de val som finns för det aktuella scenariot i spelet. 
Som argument får funktionen textens identifikationsnummer som en int-variabel, 
samt den inlästa textfilen
Funktionen kontrollerar att användaren matar in en korrekt siffra för val, 
och returnerar sedan det identifikationsnummer som tillhör nästa text som ska skrivas ut. */
int listaVal(int a, FILE *f){
	int valRaknare = 1, checkVal, anvandareVal = 0, nastaIdNum;
	char valInput[2];
	_Bool korrektVal = 1;
	char s[N]; //Likt variabeln s i main-funktionen
	rewind(f);
	while(TTS(s, N, f)){
		//Läser in den aktuella radens identifikationsnummer till checkVal
		sscanf(s+1, "%d", &checkVal);
		/* Jämför identifikationsnummer i checkVal med identifikationsnummer i funktionens parameter 'a'
		adderat med värdet i valRaknare, som börjar på 1.
		Ex: Om texten som visats har identifikationsnummer T1000, så ska valen ha V1001, V1002 osv. */
		if(s[0] == 'V' && checkVal == a+valRaknare){
			/* Skriver ut val-siffra för inmatning från användare, efter skrivs valets text och nyradstecken
			valRaknare ökar med 1 för att kontrollera om det finns fler val. */
			printf("[%d] - ", valRaknare);
			skrivUtText(s+LT, strlen(s)-LT, 0, 1);
			printf("\n");
			valRaknare++;
			rewind(f);
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
		//Läser in text från användaren med radInput
		radInput(valInput, 2);
		/*Testar om första tecknet användaren har matat in är en siffra
		I så fall scannas siffran in till variabeln anvandareVal, annars sätts anvandareVal till -1
		för att låta do/while-loopen gå ännu ett varv */
		if(isdigit(valInput[0])){
			sscanf(valInput, "%d", &anvandareVal);
		}
		else{
			anvandareVal = -1;
			
		}
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

Som argument får funktionen textsträngen som ska skrivas ut, en int-variabel 
med antal tecken som ska skrivas ut. 

_Bool-variabeln linjer bestämmer om funktionen ska skriva ut ”rader” i  början och slutet av textblocken. */
void skrivUtText(char *string, int n, _Bool linjer, _Bool tabb){
	if(linjer){
		printf("%s", L);
	}
	if (tabb){
		printf("\t");
	}
	//for-loop som kör 'n' antal varv
	for(int i=0;i<n;i++){

		//Om tecknet '|' hittas i textsträngen skrivs ett nyradstecken ut i stället.
		if (string[i] == '|'){
			printf("\n");
			if (tabb){
				printf("\t");
			}
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
Som argument får funktionen den sifferkod som finns efter den aktuella textradens identifikationsnummer. 
Funktionen returnerar 1 eller 0 beroende på om spelet ska avslutas eller inte. */
_Bool textSwitchar(int s){
	if(debugMode){printf("[Switch-funktion: Nummer: %d]\n", s);}
	//break; behövs inte för switch-satsen, funktionen avslutas när returvärde skickas.
	switch(s){
		case 9999:
			//Spelet avslutas.
			return 0; 
		case 9992:
			//Spelets text byter färg till vit, standardfärg.
			system("color 07");
			return 1;
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