#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/*Definitioner för textformatering, skriv ut i printf-satser
Kräver system("cls") initialt i koden, för Windows

ANSI-escape-koder: Används för t.ex formatering av text
ESC + [ + kod används för att mata in ANSI-escape-koder
\033 (oktalt) är escape-koden för tecknet Escape, det går också att använda \x1b för det hexadecimala värdet

ANSI-escape + n m 
där n är 31 - 37: Sätter olika färger på texten
där n är 91 - 97: Sätter olika färger på texten, högintensivt
där n är 40 - 47: Sätter olika färger på textens bakgrund
där n är 0: Återställer
m används för att indikera slut på formateringskoder

Exempel: \033[91;43m TEXT ger intensiv röd teckenfärg med gul bakgrund

Källa:
https://en.wikipedia.org/wiki/Escape_character 
https://en.wikipedia.org/wiki/ANSI_escape_code*/

//Ljusa färger
#define FORM_GREY "\033[90m" 		//Grå text
#define FORM_RED "\033[91m" 		//Röd text
#define FORM_GREEN "\033[92m" 	//Grön text
#define FORM_YELLOW "\033[93m" 	//Gul text
#define FORM_BLUE "\033[94m" 		//Blå text
#define FORM_MAGENTA "\033[95m" //Lila text
#define FORM_CYAN "\033[96m" 		//Cyan text
#define FORM_WHITE "\033[97m" 		//Vit text

//Mörka färger
#define FORM_BLACK "\033[30m" 					//Svart text
#define FORM_RED_DARK "\033[31m" 			//Röd text
#define FORM_GREEN_DARK "\033[32m" 		//Grön text
#define FORM_YELLOW_DARK "\033[33m" 	//Gul text
#define FORM_BLUE_DARK "\033[34m" 			//Blå text
#define FORM_MAGENTA_DARK "\033[35m" 	//Lila text
#define FORM_CYAN_DARK "\033[36m" 		//Cyan text

//Textbakgrunder
#define FORM_BLACK_BG "\033[40m" 		//Svart textbakgrund
#define FORM_RED_BG "\033[41m" 			//Röd textbakgrund
#define FORM_GREEN_BG "\033[42m" 		//Grön textbakgrund
#define FORM_YELLOW_BG "\033[43m" 	//Gul textbakgrund
#define FORM_BLUE_BG "\033[44m" 			//Blå textbakgrund
#define FORM_MAGENTA_BG "\033[45m" 	//Lila textbakgrund
#define FORM_CYAN_BG "\033[46m" 		//Cyan textbakgrund
#define FORM_WHITE_BG "\033[47m"		//Vit textbakgrund

//Annan textformatering
#define FORM_UNDER "\033[4m" 		//Understruken textformatering
#define FORM_INTENSIVE "\033[1m" //Bold/intensiv textformatering
#define FORM_END "\033[0m" 			//Återställer textformatering

//För utskrift av linjer med printf
#define ADD_LINE "\n-----------------------------------------------------------------\n"


//Funktionsdeklarationer för funktioner i libtxt.c
_Bool removeNewLine(char[]);
void clearBuffer(void);
void clearBufferFil(FILE *f);
_Bool radInput(char[], int);
_Bool textfilTillString(char[], int, FILE *f);
_Bool TTS(char *a, int n, FILE *f);