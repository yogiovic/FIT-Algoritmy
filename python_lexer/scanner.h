/**
 *
 *     Projekt IFJ 2019
 *
 *@brief      Hlavičkový súbor pre lexikálny analyzátor (scanner)
 *
 *@author     xondre12
 *@date       09.11.2019
 */
#ifndef _SCANNER_H_
#define _SCANNER_H_
#include "dynString.h"
#include "token.h"

#define START 		        	500 //jednotlivé stavy konečného automatu
#define INDENT       			501
#define IDENTIFICATOR        	503
#define PDOUBLE          		504
#define SCANNER_DOUBLE       	505
#define INT              		506
#define EXPINT          		507
#define EXPDOUBLE        		508
#define SCANNER_STRING       	509
#define ESCAPESEQ            	511
#define HEXESCAPESEQ         	512
#define LINE_COMMENT         	513
#define DOCSTRING        		514
#define DOCSTRING_P 	        515

#define SCANNER_ERROR 1 //návratová hodnota pre lexikálnu chybu

#define WHITESPACE 32 //znaky ktoré je pre prehladnošť výhodnejšie zapisovať takto
#define QUOTE_SIGN 39
#define DOUBLE_QUOTE 34

static int FT = 1; //jedná sa o prvý token?
static int s; //počítadlo medzier
Stack indent, dedent; //pomocné zásobníky pre zisťovanie odsadenia

int isKeyword(String *str,Token *t);

Token getToken(String *dynamicString);

void tokenPrinter(Token* token);

void scannerSetup(FILE *input); 

void setInput(FILE *input);

#endif


