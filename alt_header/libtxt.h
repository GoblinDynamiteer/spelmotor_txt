#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define FORM_GREY ""
#define FORM_RED ""
#define FORM_GREEN ""
#define FORM_YELLOW ""
#define FORM_BLUE ""
#define FORM_MAGENTA ""
#define FORM_CYAN ""
#define FORM_WHITE ""


#define FORM_BLACK ""
#define FORM_RED_DARK ""
#define FORM_GREEN_DARK ""
#define FORM_YELLOW_DARK ""
#define FORM_BLUE_DARK ""
#define FORM_MAGENTA_DARK ""
#define FORM_CYAN_DARK ""


#define FORM_BLACK_BG ""
#define FORM_RED_BG ""
#define FORM_GREEN_BG ""
#define FORM_YELLOW_BG ""
#define FORM_BLUE_BG ""
#define FORM_MAGENTA_BG ""
#define FORM_CYAN_BG ""
#define FORM_WHITE_BG ""

//Annan textformatering
#define FORM_UNDER "" 
#define FORM_INTENSIVE "" 
#define FORM_END "" 

//För utskrift av linjer med printf
#define ADD_LINE "\n-----------------------------------------------------------------\n"


//Funktionsdeklarationer för funktioner i libtxt.c
_Bool removeNewLine(char[]);
void clearBuffer(void);
void clearBufferFil(FILE *f);
_Bool radInput(char[], int);
_Bool textfilTillString(char[], int, FILE *f);
_Bool TTS(char *a, int n, FILE *f);
void printLine(char t, int n, _Bool r);
int inputInt(const char a[]);
double inputDouble(const char a[]);