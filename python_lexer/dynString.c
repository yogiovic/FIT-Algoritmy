/**
 *
 *      Projekt IFJ 2019
 *
 * @brief      Dynamické pole
 *
 * @author     xondre12
 * @date       09.11.2019
 */
#include <stdlib.h>
#include <string.h>
#include "dynString.h"
/**
 * @brief Inicializuje štruktúru String
 * 
 * @return Inicializovaná štruktúra
 */
String* stringInit() {
	
	String *s = malloc(sizeof(String));

	if ((s->content = (char *)malloc(8)) == NULL ) return NULL; //ak by sa pokazil malloc
	
	else {
		
		stringClear(s);
		s->allocated = 8;
		return s; 
	}
}
/**
 * @brief Zmaže obsah dynamického stringu
 * 
 * @param s  
 */
void stringClear(String *s) {
	
	s->content[0] = '\0'; // ...
	s->length = 0; //length je 0
	
}
/**
 * @brief Uvoľní pamať dynamického stringu vyhradenú pre znakovú časť reťazca
 * 
 * @param s Dynamický string v ktorom chceme uvolniť
 */
void stringFree(String *s) {
	
	free(s->content);
}
/**
 * @brief Pridá znak na koniec dynamického stringu
 * 
 * @param s Dynamický string do ktorého budeme pridávať
 * @param c Znak ktorý chceme pridať
 * @return int V prípade úspešného pridania 0 inak 1
 */
int stringAppend(String *s, char c) {
	
	if (s->length + 1 >= s->allocated) {
		
		int novoAlokovane = s->length + 8;
		
		if((s->content = (char *)realloc(s->content, novoAlokovane)) == NULL) return 1;

		else s->allocated = novoAlokovane;

	}

	s->content[s->length++] = c; //na zaciatku je length 0 => na 0ty index prvy char
	s->content[s->length] = '\0'; //ukoncime string

	return 0;

}
/**
 * @brief Pridá reťazec na koniec dynamického stringu(reťazca)
 * 
 * @param s Dynamický string do ktorého budeme pridávať
 * @param appended Reťazec ktorý sa bude pridávať
 * @return int V prípade úspešnej konkatenácie sa vráti 0 inak 1
 */
int stringAppendS(String *s, const char *appended) {
	
	int appendedLen = strlen(appended);

	if (s->length + appendedLen + 1 >= s->allocated) {
		
		int newSize = s->length + appendedLen;
		newSize++;

		if (!(s->content = (char *)realloc(s->content, newSize)))	return 1;
		
		s->allocated = newSize;
	}

	s->length = s->length + appendedLen;
	strcat(s->content, appended);
	s->content[s->length] = '\0';
	return 0;
}
/**
 * @brief Vráti predchádzajúci znak
 * 
 * @param s Dynamický string v ktorom hladáme
 * @return char Znak ktorý sa vráti
 */
char lastChar(String *s){
	
	return s->content[s->length - 1];
}

