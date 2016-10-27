#include <stdio.h>
#include <string.h>
#include "struct.h"

int main(){

	struct textString string[50];
	FILE *language_file = fopen("lang.dat", "wb");
	
	//Title
	string[0].id = 0;
	strcpy(string[0].string_en, "SPELMOTOR TXT - GAME ENGINE TXT");
	strcpy(string[0].string_sv, "SPELMOTOR TXT");
	
	//Enter filename-text
	string[1].id = 1;
	strcpy(string[1].string_sv, "Ange filnamn (med filändelse) för textfil.\nMata in EOF (Ctrl+Z) för att avbryta.\n"
			"Om ingen fil anges laddas: ");
	strcpy(string[1].string_en, "Input filename (with extension) for text file.\nInput EOF (Ctrl+Z) to cancel.\n"
			"If nothing is entered, the default file will load: ");
	
	//filename input
	string[2].id = 2;
	strcpy(string[2].string_sv, "\nFilnamn: ");
	strcpy(string[2].string_en, "\nFile name: ");
	
	
	string[3].id = 3;
	strcpy(string[3].string_sv, "Spel som kommer köras är: ");
	strcpy(string[3].string_en, "\nGame that will load: ");
	
	
	//printf("%s", string[1].string_en);

	fwrite(&string, structSize, 4, language_file);
	return 0;
}