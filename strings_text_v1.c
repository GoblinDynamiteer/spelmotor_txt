#include "strings_text_v1.h"

/*
VERSION 1

Ändringar:
---------------------------------------------------------
2016-10-05
Första utgåvan
---------------------------------------------------------
2016-10-13
Uppdaterat kommentarer, inga ändringar i koden
---------------------------------------------------------

Information:
Filen innehåller funktioner för inläsning av text, 
samt kopiering från textfil till char-variabel.

Funktionerna är lika eller identiska till de stödfunktioner som finns
i boken 'C från början' av Jan Skansholm.

Anledningen till att ej använda författarens bibliotek som finns
tillgängligt på bokens hemsida, är att jag ville skriva funktionerna 
själv för att få en större förståelse för hur de fungerar och 
varför de bör användas.

Jag använder andra namn på funktionerna, än de namn
författaren har valt.

Innan varje funktion finns en förklaring för vad den gör



removeNewLine
Kontrollerar om det sista tecknet i en textsträng är ett nyradstecken.
Om så är fallet byts det ut till ett nolltecken och funtkionen ger returvärdet 1.
Annars ges returvärdet 0, vilket innebär att det kan finnas tecken kvar i bufferten.

Bokens version kallas remove_nl och finns på sida 199 */
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
så att texten inte hamnar i nästa inmatning. Systemet samlar in textinmatning
en användare gör i bufferten, som inte läses in förrän returslag ges.

Buffert kan finnas vid till exempel fgets(s, n, stdin) om användaren skriver
fler tecken än n.

Bokens version kallas skip_line och finns på sida 190 */
void clearBuffer(void){
	int i;
	while ((i = getchar()) != '\n' && i != EOF)
		;
}

/*
clearBufferFil
Liknande funktionen clearBuffer, fast för läsning från textfil.
Som parameter får funktionen den aktuella textfilen.

Bokens version kallas fskip_line och finns på sida 226 */
void clearBufferFil(FILE *f){
	int i;
	while ((i = fgetc(f)) != '\n' && i != EOF)
		;
}

/*
radInput
Funktion som läser in text från användaren till en char-variabel med fgets
Så länge inläsningen lyckas, så ger funktionen returvärdet 1. Annars 0.

removeNewLine anropas, om radInput får tillbaka returvärdet 0 anropas clearBuffer.
removeNewLine returnerar 0 om den inte "hittar" ett nyradstecken i strängens slut, 
vilket innebär att det kan finnas text kvar i bufferten, som töms med clearBuffer.

Bokens version kallas read_line och finns på sida 200 */
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
Lik radInput ovan, dock läses texten från en texfil i stället för stdin
Som parameter får funktionen variabeln som text ska skrivas till, 
antal tecken som ska skrivas, samt filen.

clearBufferFil med filen som parameter används i stället för clearBuffer.

Bokens version kallas fread_line och finns på sida 226 */
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
Anropar textfilTillString och ger dess returvärde som returvärde
Används som namnförkortning textfilTillString.

Jag lyckades inte ordna det med ett macro eller på annat.
*/
_Bool TTS(char *a, int n, FILE *f){
	return textfilTillString(a, n, f);
}

