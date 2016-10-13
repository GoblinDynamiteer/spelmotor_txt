#include "strings_text_v1.h"

/*
VERSION 1

�ndringar:
---------------------------------------------------------
2016-10-05
F�rsta utg�van
---------------------------------------------------------
2016-10-13
Uppdaterat kommentarer, inga �ndringar i koden
---------------------------------------------------------

Information:
Filen inneh�ller funktioner f�r inl�sning av text, 
samt kopiering fr�n textfil till char-variabel.

Funktionerna �r lika eller identiska till de st�dfunktioner som finns
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

