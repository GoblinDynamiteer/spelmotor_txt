#include <windows.h>
#include "strings_text_v1.h" //Eget funktionsbibliotek inneh�llande diverse text- och textfil-funktioner

#define N 1000	//L�ngd f�r char-variabler, och parameter till vissa funktioner
#define TEXTFIL "spel_default.txt" //Standardtextfil med "Bj�rnspelet"
#define L "\n-----------------------------------------------------------------\n" //F�r utskrift av linjer med printf
#define TEXTHASTIGHET 3		//Antal millisekunder att pausera mellan varje teckenutskrift i funktionen skrivUtText
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
Det g�r att starta spelet med en textfil som argument, d� laddas denna fil ex: 'text_dyn.exe piratspel.txt'
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
Fixade kontroll f�r inmatning av anv�ndarval, programmet slutar nu inte att fungera om anv�ndaren matar in n�got annat �n en siffra
Lade till funktionalitet f�r att l�sa in val fr�n textfil som inte ligger i ordning, eller efter texten de tillh�r
Lade till sifferkod 9992, f�r f�rgbyte av text till vit

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
	FILE * textfil = fopen(argv[1], "r");
	char s[N], filnamn[N], restart = 'j';
	int idNum, idCheck, switchCheck;
	/* S�tter teckenkodning till 1252, f�r korrekt utskrift av svenska tecken.
	Blankar sedan sk�rmen */
	system("chcp 1252");
	system("cls");
	/*skrivUtText(s,n,l,t) --> Skriver ut n antal tecken av str�ngen s. 
	Om l == 1 skrivs "linjer" ut innan och efter texten (macro L). 
	om t == 1 skrivs tabbslag ut i b�rjan av texten och efter returslag */
	skrivUtText("SPELMOTOR TXT", 13, 1, 0);

	if(textfil == NULL){ 
		/* Programmet har inte f�tt en textfil som argument, eller har inte kunnat hitta/l�sa textfilen i argumentet. 
		Anv�ndaren uppmanas att skriva in ett namn f�r textfil att spela med. */
		printf("Ange filnamn (med fil�ndelse) f�r textfil.\nMata in EOF (Ctrl+Z) f�r att avbryta\nOm ingen fil anges laddas %s\n\nTextfil: ",TEXTFIL);
		//radInput finns i strings_text_v1.c, och beskrivs d�r.
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
		//TTS finns i strings_text_v1.c, och beskrivs d�r. Textfilens f�rsta rad, som ska vara spelets titel, l�ses till variabeln s.
		TTS(s, N, textfil);
		printf("%sSpel som kommer k�ras �r: %s %s", L, s, L);
		//Programmet pauserar tills anv�ndaren trycker p� en valfri tangent, sedan blankas sk�rmen.
		system("pause");
		system("cls");
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
					skrivUtText("Spelet �r Slut|Tack!", 20, 1, 0);
					break; //Bryter den inre while-loopen
				}
				/* Funktionen listaVal anropas med idNum och textfilen som parametrar.
				Funktionen returnerar ett nytt v�rde som idNum s�tts till. */
				idNum = listaVal(idNum, textfil);
				//rewind() "spolar" tillbaka textfilen till dess b�rjan.
				rewind(textfil);
			}
		}
	rewind(textfil);
	printf("Tryck J f�r att spela igen: ");
	restart = getchar();
	clearBuffer();
	//S�tter textf�rgen till vit
	system("color 7"); 
	}
	return 0;
}


/* Funktionen listaVal skriver ut de val som finns f�r det aktuella scenariot i spelet. 
Som argument f�r funktionen textens identifikationsnummer som en int-variabel, 
samt den inl�sta textfilen
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
			printf("[%d] - ", valRaknare);
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
			printf("%sAnge val genom at sl� in en siffra: ",L);
		}
		else {
			printf("%sFel val!\nAnge val genom at sl� in en siffra: ",L);
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
	if(linjer){
		printf("%s", L);
	}
	if (tabb){
		printf("\t");
	}
	//for-loop som k�r 'n' antal varv
	for(int i=0;i<n;i++){

		//Om tecknet '|' hittas i textstr�ngen skrivs ett nyradstecken ut i st�llet.
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
			//Spelets text byter f�rg till vit, standardf�rg.
			system("color 07");
			return 1;
		case 9993:
			//Spelets text byter f�rg till bl�.
			system("color 09");
			return 1;
		case 9994:
			//Spelets text byter f�rg till gr�n.
			system("color 02");
			return 1;
		case 9995:
			//Spelets text byter f�rg till r�d.
			system("color 04");
			return 1;
		case 9996:
			//Spelets text byter f�rg till bl� och spelet avslutas.
			system("color 09");
			return 0;
		case 9997:
			//Spelets text byter f�rg till gr�n och spelet avslutas.
			system("color 02");
			return 0;
		case 9998:
			//Spelets text byter f�rg till r�d och spelet avslutas.
			system("color 04");
			return 0;
			//0000 anv�nds f�r att enbart skriva ut texten, utan extra funktionalitet.
		case 0000:
			return 1;
		default:
			return 1;
	}
}