#ifndef _DYN_STRING_H_
#define _DYN_STRING_H_
/**
 *
 *      Projekt IFJ 2019
 *
 * @brief      Hlavičkový súbor pre generáciu medzikódu
 *
 * @author     xoltma00, xondre12
 * @date       09.11.2019
 */

#include <stdbool.h>
#include "token.h"
/**
 * @brief Dynamické pole na ukladanie reťazcov tzv. dynamický string
 * 
 */
typedef struct
{
	char *content; 
	unsigned int length;
	unsigned int allocated;

} String;

void stringClear(String *s);

String* stringInit();

void stringFree(String *s);

int stringAppend(String *s, char c);

char lastChar(String *s);

int stringAppendS(String *s, const char *appended);

#endif

