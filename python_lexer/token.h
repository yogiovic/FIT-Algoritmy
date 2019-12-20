/**
 *
 *      Projekt IFJ 2019
 *
 * @brief      Hlavičkový súbor pre prácu so štruktúrou token
 *
 * @author     xondre12
 * @date       09.11.2019
 */
#ifndef _TOKEN_H_
#define _TOKEN_H_
#include <stdio.h>
#include <stdlib.h>

#define TOKEN_NO_TYPE           600    //Typy tokenov
#define TOKEN_INTEGER           601
#define TOKEN_IDENTIFICATOR     602     
#define TOKEN_DOUBLE            604
#define TOKEN_EXPONENT_INTEGER  605
#define TOKEN_EXPONENT_DOUBLE   606
#define TOKEN_STRING            607
#define TOKEN_EOF               608
#define TOKEN_ERROR             609
#define TOKEN_EOL               610
#define TOKEN_INDENT 			611
#define TOKEN_DEDENT 			612
#define TOKEN_DOCSTRING			613
#define TOKEN_OPERATOR          630 

#define EQUAL_OPERATOR          635   //Dvojznakové operátory
#define ASIGN_OPERATOR          636
#define LESS_OPERATOR           637
#define LESS_EQUAL_OPERATOR     638
#define MORE_OPERATOR           639
#define MORE_EQUAL_OPERATOR     640
#define NOT_EQUAL_OPERATOR      641
#define INT_DIVISION_OPERATOR  	642

#define LEFT_BRACKET_OPERATOR   40    //Jednoznakové operátory
#define RIGHT_BRACKET_OPERATOR  41
#define PLUS_OPERATOR           43
#define MINUS_OPERATOR          45
#define DIVISION_OPERATOR       47
#define TIMES_OPERATOR  		42


#define COLON_OPERATOR          58    //Dvojbodka
#define COMMA_OPERATOR			44

#define TOKEN_KEYWORD           645   //Keywordy
#define DEF_KEYWORD             646
#define END_KEYWORD             647
#define LEN_KEYWORD             648
#define DO_KEYWORD              649
#define IF_KEYWORD              650
#define PASS_KEYWORD            651
#define ELSE_KEYWORD            652
#define WHILE_KEYWORD           653
#define THEN_KEYWORD            654
#define PRINT_KEYWORD           655
#define INPUTS_KEYWORD          656
#define INPUTI_KEYWORD          657
#define INPUTF_KEYWORD          658
#define LENGTH_KEYWORD          659
#define SUBSTR_KEYWORD          660
#define ORD_KEYWORD             661
#define CHR_KEYWORD             662
#define NONE_KEYWORD            663
#define RETURN_KEYWORD          664

/**
 * @brief Štruktúra token ktorá uchováva typ a atribút poprípade pomocné informácie
 * 
 */
typedef struct {
	int tokenType; //typ tokenu
	int isFunction; //jedná sa o definíciu funkcie?
	int operatorSign; //o ktorý operátor sa jedná?
	int integer; //celočíselná hodnota?
	int keyword; //o ktorý keyword sa jedná?
	char* stringContent; //obsah reťazca
	double realNumber; //
} Token;
/**
 * @brief Pomocný zásobník na generovanie tokenov INDENT a DEDENT
 * 
 */
typedef struct
{
    int pole[500];
    int top;
} Stack;


void initToken(Token *t);

Token createErrorToken(Token *t);

Token createToken(int type, Token *t);

void stackInit(Stack *stack);

void stackPush(Stack *stack, int element);

void stackPop(Stack *stack);

int stackTop(Stack *stack);

Token processOperator(int OPERATOR, Token *t);

#endif
