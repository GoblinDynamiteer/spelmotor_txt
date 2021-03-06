#include <windows.h>

 //Inneh�ller deklarationer av funktioner i libxt.c, och macron f�r textformattering
#include "libtxt.h"

//Denna header kan anv�ndas f�r att ta bort funktionalitet f�r f�rg p� text, om detta inte fungerar korrekt.
//Tillg�nglig p� programmets GitHub-sida: https://github.com/GoblinDynamiteer/spelmotor_txt
//#include "alt_header\libtxt.h"

#define N 1000	//L�ngd f�r char-variabler, och parameter till vissa funktioner
#define TEXTFIL "spel_default.txt" //Standardtextfil med "Bj�rnspelet"

#define TEXTHASTIGHET 15		//Antal millisekunder att pausera mellan varje teckenutskrift i funktionen skrivUtText
#define LT 11	//Antal tecken p� rad i textfilen, innan text som ska visas, ex: "V1001|2100|Text som ska visas"

/*
SPELMOTOR TXT
Program f�r att k�ra spel�ventyr fr�n textfiler.
Av Johan K�mpe

Kommentarer visas �ver eller till h�ger om det det beskriver i koden.


�ndringslogg:
2016-10-10
Initial release
Testspel fungerar att k�ras fr�n textfil.
Spelet kan avslutas
Kontroll tillagt f�r anv�ndarinmatning vid val
Debug-funktionalitet tillagd

2016-10-11
Radbrytningar i text, dessa triggas med tecknet | i textfilen
Funktion f�r switchar
Test f�r om textfilen existerar
Tagit bort on�dig variabel "bokstav", beh�vs ej f�r att f�nga upp f�rsta bokstaven i textraderna
Det g�r att starta spelet med en textfil som argument, d� laddas denna fil ex: 'text_dyn.exe spel_pirat.txt'
Namngivning av program: SPELMOTOR TXT
Borttagning av on�diga deklarerade variabler som inte anv�ndes.

2016-10-12
St�d f�r titel av spel, som ska/m�ste skrivas p� rad 1 i textfilen.
St�d f�r omspel, anv�ndaren kan trycka p� J/j efter avslutat spel f�r att spel om.
Fler sifferkoder tillagda f�r f�rgbyte vid avslut av spel.

2016-10-13
�ndrat ordning p� laddning av textfiler
Lagt in system("chcp 1252") f�r att byta teckenkodning, s� att svenska tecken visas korrekt.
Uppdaterat debug-texter
Korrigerat grammatik och stavfel i text.
Lagt till tre nya kommandon f�r att byta textf�rg, utan att avsluta spelet
Flertalet namnbyten av variabler

2016-10-16
�kade radinl�sning (macro N) till 1000 st tecken.
Bytt parameter -> argument i kommentarer
Ny _Bool-parameter f�r funktion skrivUtText, anv�nds f�r tabbslag i b�rjan av text, och efter radbrytning
�ndrade hur utskrift av val ser ut, f�r l�sbarheten skull

2016-10-19
Fixade kontroll f�r inmatning av anv�ndarval:
	programmet slutar nu inte att fungera om anv�ndaren matar in n�got annat �n en siffra
Lade till funktionalitet f�r att l�sa in val fr�n textfil som inte ligger i ordning, eller efter texten de tillh�r
Lade till sifferkod 9992, f�r f�rgbyte av text till vit
Lagt till st�d f�r enskild teckenf�rg och formatering

2016-10-21
�ndrat vissa teckenkoder f�r textformatering, s� f�rgen blir ljusare vid utskrift
Namnbyte av fil strings_text_v1.c till funk.c
Namnbyte av fil strings_text_v1.h till funk.h

2016-10-22
Namnbyte av funk.c till libtxt.c
Namnbyte av funk.h till libtxt.h
Flyttade macron f�r textformattering till libtxt.h
Flyttade macro f�r "linje" till libtxt.h
Ny namngivning av macron i libtxt.h
�ndring av macro-anrop i koden

2016-10-23
�ndrade felaktig text i kommentarer

2016-10-27
Ytterligare kommentering av k�llkoden

2016-10-30
Lade till kommenterad rad: #include "alt_header\libtxt.h"
	Denna fil �r en alternativ header, som tar bort f�rgformattering av texten i spel, 
	kan anv�ndas om ANSI-escape-koder ej fungerar korrekt.

*/

//Deklaration av funktioner som finns i denna fil:
int listaVal(int a, FILE *f);
void skrivUtText(char *string, int n, _Bool linjer, _Bool tabb);
_Bool textSwitchar(int s);

//Debug-l�ge. S�tt till 1 f�r att f� extra information utskriven fr�n programmet
_Bool debugMode = 0;

//Argumentet argv f�r mainfunktionen ges fr�n kommandopromten med "spelmotortxt.exe PARAMETER"
int main(int argc, char *argv[]){
	/* �ppnar v�rdet i argv[1] som textfil till variabel textfil 
	argv[0] �r programmets namn */
	FILE * textfil = fopen(argv[1], "r"); //r = read
	char s[N], filnamn[N], restart = 'j';
	int idNum, idCheck, switchCheck;
	/* S�tter teckenkodning till 1252, f�r korrekt utskrift av svenska tecken.
	Blankar sedan sk�rmen */
	system("chcp 1252");
	system("cls"); //Beh�vs ocks� f�r att ANSI-escape-koder ska fungera
	/*skrivUtText(s,n,l,t) --> Skriver ut n antal tecken av str�ngen s. 
	Om l == 1 skrivs "linjer" ut innan och efter texten (macro ADD_LINE i libtxt.h). 
	om t == 1 skrivs tabbslag ut i b�rjan av texten och efter returslag */
	printf(FORM_INTENSIVE); //Intensiv vit f�rg
	skrivUtText("SPELMOTOR TXT", 13, 1, 0); //Programmets namn
	printf(FORM_END); //Slut teckenformatering
	if(textfil == NULL){ 
		/* Programmet har inte f�tt en textfil som argument, 
		eller har inte kunnat hitta/l�sa textfilen i argumentet. 
		Anv�ndaren uppmanas att skriva in ett namn f�r textfil att spela med. */
		printf("Ange filnamn (med fil�ndelse) f�r textfil.\nMata in EOF (Ctrl+Z) f�r att avbryta.\n"
			"Om ingen fil anges laddas %s\n\nTextfil: ",TEXTFIL);
		//radInput finns i libtxt.c, och beskrivs d�r.
		if(radInput(filnamn, N)){
			textfil = fopen(filnamn, "r");
		}
		else {
			printf("Programmet avslutas!");
			exit(0);
		}
	}

	if(textfil == NULL){ 
		//Om inmatad fil fr�n anv�ndaren inte kan l�sas f�rs�ker programmet ladda in standard-text definierad i macrot TEXTFIL.
		printf("Inmatad fil: '%s' kunde inte l�sas.\nL�ser in standard-spel %s", filnamn, TEXTFIL);
		textfil = fopen(TEXTFIL, "r");
	}

	if(textfil == NULL){
		//Om TEXTFIL inte kunde l�sas in avslutas programmet.
		printf("Standardfilen '%s' kunde inte hittas. Programmet avlutas", TEXTFIL);
		return 0;
	}
	
	/* Spelet kommer startas om s� l�nge anv�ndaren trycker p� j/J efter avslutat spel
	tolower() anv�nds f�r att konvertera ett tecken till en gemen */
	while(tolower(restart) == 'j'){
		/* idNum s�tts till 1000 f�r att skriva ut spelets f�rsta val, som ska vara just 1000 */
		idNum = 1000;
		//TTS finns i libtxt.c, och beskrivs d�r. Textfilens f�rsta rad, som ska vara spelets titel, l�ses till variabeln s.
		TTS(s, N, textfil);
		printf(ADD_LINE "Spel som kommer k�ras �r:\n" FORM_WHITE "%s" FORM_END ADD_LINE, s);
		//Programmet pauserar tills anv�ndaren trycker p� en valfri tangent, sedan blankas sk�rmen.
		system("pause");
		system("cls"); //Clearscreen
		while(TTS(s, N, textfil)){
			/* sscanf fungerar likt scanf, fast fr�n en textstr�ng i st�llet f�r anv�ndarinmatning 
			H�r l�ses sifferv�rdet efter det f�rsta tecknet i char-variabeln 's' till idCheck */
			sscanf(s+1, "%d", &idCheck);
			//Letar efter rad som b�rjar p� T och har v�rdet i variabeln 'idNum'
			if(s[0] == 'T' && idCheck == idNum){
				/* N�r r�tt rad hittas skrivs radens text ut med funktionen skrivUtText
				macrot LT �r antalet tecken som finns p� raden innan texten som ska skrivas ut. */
				skrivUtText(s+LT, strlen(s)-LT, 1, 0);
				/* L�ser sifferkoden f�r den aktuella raden till switchCheck. 
				Funktionen textSwitchar anropas med sifferkoden.
				Funktionen utf�r olika kommandon beroende p� kodens v�rde 
				Om spelet ska avslutas ger funktionen returv�rdet 0, annars 1 */
				sscanf(s+6,"%d", &switchCheck);
				if(!textSwitchar(switchCheck)){
					skrivUtText("Spelet �r Slut|Tack!", 20, 1, 0); //Tecknet '|' skrivs ut som nyradstecken med funktionen
					break; //Bryter den inre while-loopen
				}
				/* Funktionen listaVal anropas med idNum och textfilen som parametrar.
				Funktionen returnerar ett nytt v�rde som idNum s�tts till. */
				idNum = listaVal(idNum, textfil);
				//rewind() "spolar" tillbaka textfilen till dess b�rjan.
				rewind(textfil);
			}
		} //Slut inre while-loop.
	rewind(textfil);
	printf("Tryck J f�r att spela igen: ");
	restart = getchar();
	clearBuffer(); //T�mmer teckenbuffer.
	//S�tter textf�rgen till vit.
	system("color 7"); 
	} //Slut yttre while-loop.
	return 0; //Programmet avslutas.
} //Slut main-funktion.


/* Funktionen listaVal skriver ut de val som finns f�r det aktuella scenariot i spelet. 
Som argument f�r funktionen textens identifikationsnummer som en int-variabel, 
samt den inl�sta textfilen.
Funktionen kontrollerar att anv�ndaren matar in en korrekt siffra f�r val, 
och returnerar sedan det identifikationsnummer som tillh�r n�sta text som ska skrivas ut. */
int listaVal(int a, FILE *f){
	int valRaknare = 1, checkVal, anvandareVal = 0, nastaIdNum;
	char valInput[2];
	_Bool korrektVal = 1;
	char s[N]; //Likt variabeln s i main-funktionen
	rewind(f);
	while(TTS(s, N, f)){
		//L�ser in den aktuella radens identifikationsnummer till checkVal
		sscanf(s+1, "%d", &checkVal);
		/* J�mf�r identifikationsnummer i checkVal med identifikationsnummer i funktionens parameter 'a'
		adderat med v�rdet i valRaknare, som b�rjar p� 1.
		Ex: Om texten som visats har identifikationsnummer T1000, s� ska valen ha V1001, V1002 osv. */
		if(s[0] == 'V' && checkVal == a+valRaknare){
			/* Skriver ut val-siffra f�r inmatning fr�n anv�ndare, efter skrivs valets text och nyradstecken
			valRaknare �kar med 1 f�r att kontrollera om det finns fler val. */
			printf(FORM_GREEN "[%d]" FORM_END "  - ", valRaknare); //Valnumren skrivs ut med gr�n text
			skrivUtText(s+LT, strlen(s)-LT, 0, 1);
			printf("\n");
			valRaknare++;
			rewind(f);
		}
	}
	valRaknare--; //Backar valRaknare med 1 (sista valet hittades inte)
	if(debugMode){printf("\n[Funktion listaVal - valr�knare v�rde: %d]\n", valRaknare);}
	//Do-sats som upprepas om anv�ndaren inte matar in ett korrekt val.
	do{
		if(korrektVal){ //Triggar alltid f�rsta g�ngen
			printf(ADD_LINE "Ange val genom at sl� in en siffra: ");
		}
		else {
			//Fel val! skrivs i r�tt
			printf(ADD_LINE FORM_RED "Fel val!" FORM_END "\nAnge val genom at sl� in en siffra: ");
		}
		//L�ser in text fr�n anv�ndaren med radInput
		radInput(valInput, 2);
		/*Testar om f�rsta tecknet anv�ndaren har matat in �r en siffra
		I s� fall scannas siffran in till variabeln anvandareVal, annars s�tts anvandareVal till -1
		f�r att l�ta do/while-loopen g� �nnu ett varv */
		if(isdigit(valInput[0])){
			sscanf(valInput, "%d", &anvandareVal);
		}
		else{
			anvandareVal = -1;
			
		}
		//korrektVal s�tts till 0, f�r att skriva ut "Fel val!" ovan, om anv�ndaren skulle skriva in ett felaktigt val
		korrektVal = 0; 
	//Det inmatade v�rdet fr�n anv�ndaren ska vara mellan 1 och v�rdet i valRaknare
	} while(anvandareVal <=0 || anvandareVal > valRaknare);

	rewind(f);
		while(TTS(s, N, f)){
		/* checkVal s�tts till den aktuella radens identifikationsnummer och j�mf�rs 
		med identifikationsnummer i funktionens parameter 'a' adderat med anv�ndarinmatningen 
		i anvandareVal */
		sscanf(s+1, "%d", &checkVal);
		//Val-rader i textfilen ska b�rja med tecknet 'V'
		if(s[0] == 'V' && checkVal == a+anvandareVal){
			//N�r korrekt rad hittas s�tts nastaIdNum till identifikationsnumret f�r n�sta rad som ska anv�ndas i spelet
			sscanf(s+6,"%d", &nastaIdNum);
		}
	}
	if(debugMode){printf("\n[Funktion listaVal returnerar: %d]\n", nastaIdNum);}
	return nastaIdNum;
}

/* Funktionen skrivUtText skriver ut text till sk�rmen, ett tecken i taget. 
Mellan varje teckenutskrift sker en f�rdr�jning p� lika m�nga millisekunder 
som macrot TEXTHASTIGHET �r definierat till. 

Som argument f�r funktionen textstr�ngen som ska skrivas ut, en int-variabel 
med antal tecken som ska skrivas ut. 

_Bool-variabeln linjer best�mmer om funktionen ska skriva ut �rader� i  b�rjan och slutet av textblocken. */
void skrivUtText(char *string, int n, _Bool linjer, _Bool tabb){
	if(linjer){ //Skriver ut en linje om funktionen har anropats med linjer=1
		printf(ADD_LINE);
	}
	if (tabb){ //Skriver ut ett tabbtecken om funktionen har anropats med tabb=1
		printf("\t");
	}
	//for-loop som k�r 'n' antal varv
	for(int i=0;i<n;i++){
		//Om tecknet '|' hittas i textstr�ngen skrivs ett nyradstecken ut i st�llet.
		if (string[i] == '|'){
			printf("\n");
			if (tabb){
				printf("\t"); //Skriver ut ett tabbtecken efter ny rad om funktionen har anropats med tabb=1
			}
		}
		/* Om tecknet "<" hittas i textstr�ngen inneb�r det att texten ska formateras p� ett visst s�tt 
		Ex: Du �r en stor <u<<r<brunbj�rn> i en djup m�rk skog, vad g�r du?
		"brunbj�rn" kommer formateras till r�d understruken text */
		else if (string[i] == '<'){
			switch(string[++i]){ //i �kar med 1 innan det anv�nds innan string anv�nds i switch-satsen
				case 'r': //R�d text
					printf(FORM_RED);
					i++; //i �kar med 1 i samtliga cases, f�r att hoppa �ver n�sta "<"
					break;
				case 'u': //Understruken text
					printf(FORM_UNDER);
					i++;
					break;
				case 'c': //Cyan text
					printf(FORM_CYAN);
					i++;
					break;
				case 'g': //Gr�n text
					printf(FORM_GREEN);
					i++;
					break;
				case 'y': //Gul text
					printf(FORM_YELLOW);
					i++;
					break;
				case 'b': //Bl� text
					printf(FORM_BLUE);
					i++;
					break;
				case 'l': //Lila text
					printf(FORM_MAGENTA);
					i++;
					break;
			}
		}
		//Om tecknet '>' hittas textstr�ngen i textstr�ngen inneb�r det att speciell formatering av text ska avslutas
		else if (string[i] == '>'){
			printf(FORM_END); //FORM_END �r macrot f�r '\033[0m' som �terst�ller textformatering
		}
		else{//Om tecknet i string[i] inte �r ett specialtecken som har triggat ovanst�ende case- eller if-satser
			//putchar skriver ut ett tecken
			putchar(string[i]);
		}
		/* Pauserar programmet TEXTHASTIGHET millisekunder
		Ju h�gre v�rdet TEXTHASTIGHET �r definierat till, detso l�ngsammare skrivs texten ut. */
		Sleep(TEXTHASTIGHET);
	}
	if(linjer){ 
		printf(ADD_LINE);
	}
}

/*
Funktionen textSwitchar anv�nds f�r att k�ra vissa kommandon som kan anv�ndas i spelet. 
Som t.ex. att avsluta spelet eller att byta f�rg p� texten. 
Som argument f�r funktionen den sifferkod som finns efter den aktuella textradens identifikationsnummer. 
Funktionen returnerar 1 eller 0 beroende p� om spelet ska avslutas eller inte. */
_Bool textSwitchar(int s){
	if(debugMode){printf("[Switch-funktion: Nummer: %d]\n", s);}
	//break; beh�vs inte f�r switch-satsen, funktionen avslutas n�r returv�rde skickas.
	switch(s){
		case 9999:
			//Spelet avslutas.
			return 0; 
		case 9992:
			//All text byter f�rg till vit, standardf�rg.
			system("color 07");
			return 1;
		case 9993:
			//All text byter f�rg till bl�.
			system("color 09");
			return 1;
		case 9994:
			//All text byter f�rg till gr�n.
			system("color 02");
			return 1;
		case 9995:
			//All text byter f�rg till r�d.
			system("color 04");
			return 1;
		case 9996:
			//All text byter f�rg till bl� och spelet avslutas.
			system("color 09");
			return 0;
		case 9997:
			//All text byter f�rg till till gr�n och spelet avslutas.
			system("color 02");
			return 0;
		case 9998:
			//All text byter f�rg till �d och spelet avslutas.
			system("color 04");
			return 0;
			//0000 anv�nds f�r att enbart skriva ut texten, utan extra funktionalitet.
		case 0000:
			return 1;
		default:
			return 1;
	}
}