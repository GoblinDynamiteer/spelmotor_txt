#include "libtxt.h"

/*
VERSION 5

�ndringar:
---------------------------------------------------------
2016-10-05
F�rsta utg�van
---------------------------------------------------------
2016-10-13
Uppdaterat kommentarer, inga �ndringar i koden
---------------------------------------------------------
2016-10-22
Lade till definitioner f�r textformatering i header
Namnbyte av filer till libtxt.*
Namnbyte av macron
Lade till fler f�rger f�r textformatering
---------------------------------------------------------
2016-10-23
Ny funktion: printLine(t,n)
	skriver ut n st tecken av char t
---------------------------------------------------------
2016-10-25
Ny funktion: inputInt(a)
	Skriver ut text a, l�ser in int som returneras
Ny funktion: inputDouble(a)
	Skriver ut text a, l�ser in double som returneras
---------------------------------------------------------


Information:
Filen inneh�ller funktioner f�r inl�sning av text, textfilshantering, samt
diverse andra text-funktioner

Fler av funktionerna �r lika eller identiska till de st�dfunktioner som finns
i boken 'C fr�n b�rjan' av Jan Skansholm.

Anledningen till att ej anv�nda f�rfattarens bibliotek som finns
tillg�ngligt p� bokens hemsida, �r att jag ville skriva funktionerna 
sj�lv f�r att f� en st�rre f�rst�else f�r hur de fungerar och 
varf�r de b�r anv�ndas.

Jag anv�nder andra namn p� funktionerna, �n de namn
f�rfattaren har valt.

Innan varje funktion finns en f�rklaring f�r vad den g�r



removeNewLine
Kontrollerar om det sista tecknet i en textstr�ng �r ett nyradstecken.
Om s� �r fallet byts det ut till ett nolltecken och funtkionen ger returv�rdet 1.
Annars ges returv�rdet 0, vilket inneb�r att det kan finnas tecken kvar i bufferten.

Bokens version kallas remove_nl och finns p� sida 199 */
_Bool removeNewLine(char a[]){
	int i = strlen(a) -1;
	if (a[i] == '\n'){
		a[i] = '\0';
		return 1;
	}
	return 0;
}

/*  
clearBuffer
Funktionen rensar den textbuffert som i bland finns kvar vid textinmatning, 
s� att texten inte hamnar i n�sta inmatning. Systemet samlar in textinmatning
en anv�ndare g�r i bufferten, som inte l�ses in f�rr�n returslag ges.

Buffert kan finnas vid till exempel fgets(s, n, stdin) om anv�ndaren skriver
fler tecken �n n.

Bokens version kallas skip_line och finns p� sida 190 */
void clearBuffer(void){
	int i;
	while ((i = getchar()) != '\n' && i != EOF)
		;
}

/*
clearBufferFil
Liknande funktionen clearBuffer, fast f�r l�sning fr�n textfil.
Som parameter f�r funktionen den aktuella textfilen.

Bokens version kallas fskip_line och finns p� sida 226 */
void clearBufferFil(FILE *f){
	int i;
	while ((i = fgetc(f)) != '\n' && i != EOF)
		;
}

/*
radInput
Funktion som l�ser in text fr�n anv�ndaren till en char-variabel med fgets
S� l�nge inl�sningen lyckas, s� ger funktionen returv�rdet 1. Annars 0.

removeNewLine anropas, om radInput f�r tillbaka returv�rdet 0 anropas clearBuffer.
removeNewLine returnerar 0 om den inte "hittar" ett nyradstecken i str�ngens slut, 
vilket inneb�r att det kan finnas text kvar i bufferten, som t�ms med clearBuffer.

Bokens version kallas read_line och finns p� sida 200 */
_Bool radInput(char a[], int n){
	if (fgets(a, n, stdin) == NULL){
		return 0;	
	}
	/*om removeNewLine returnerar 0 (! = inverterat) finns det text kvar i buffert, 
	och clearBuffer-funktionen ovan anropas */
	if (!removeNewLine(a)){ 
		clearBuffer();	
	}
	return 1;
}

/*
textfilTillString
Lik radInput ovan, dock l�ses texten fr�n en texfil i st�llet f�r stdin
Som parameter f�r funktionen variabeln som text ska skrivas till, 
antal tecken som ska skrivas, samt filen.

clearBufferFil med filen som parameter anv�nds i st�llet f�r clearBuffer.

Bokens version kallas fread_line och finns p� sida 226 */
_Bool textfilTillString(char *a, int n, FILE *f){
	if (fgets(a, n, f) == NULL){
		return 0;
	}
	if (!removeNewLine(a)){ 
		clearBufferFil(f);
	}
	return 1;
}

/*
TTS
Anropar textfilTillString och ger dess returv�rde som returv�rde
Anv�nds som namnf�rkortning textfilTillString.

Jag lyckades inte ordna det med ett macro eller p� annat.
*/
_Bool TTS(char *a, int n, FILE *f){
	return textfilTillString(a, n, f);
}

/*
printLine
Skriver ut tecknet 't', n st antal g�nger.

Exempel: 
printLine('-', 15);
-->
---------------
*/
void printLine(char t, int n){
	while(n--){
		putchar(t);
	}
}

/* inputInt
Skriver ut texten i char-variabel a, och tar en anv�ndarinl�sning till int-variabel som returneras

Exempel anv�ndning:
int variabel = inputInt("Skriv in ett heltal: ");
 */
int inputInt(const char a[]){
	int temp;
	printf("%s", a);
	scanf("%d", &temp);
	return temp;
}

/* inputDouble
Skriver ut texten i char-variabel a, och tar en anv�ndarinl�sning till double-variabel som returneras

Exempel anv�ndning:
double variabel = inputDouble("Skriv in ett decimaltal: ");
 */
double inputDouble(const char a[]){
	double temp;
	printf("%s", a);
	scanf("%lf", &temp);
	return temp;
}
