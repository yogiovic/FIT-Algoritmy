/**
 *
 *     Projekt Formálne jazyky a prekladače 2019
 *
 *@brief      Lexikálny analyzátor (scanner)
 *
 *@author     xondre12
 *@date       09.11.2019
 */
#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include "scanner.h"
#include "token.h"
#include "dynString.h"

/**
 *@brief Súbor so vstupom (stdin)
 *
 */
FILE * f;

/**
 *@brief Nastaví vstupný súbor
 * 
 *@param input Súbor z ktorého budeme čítať 
 */
void setInput(FILE *input)
{

	f = input;
}
/**
 * @brief Nastaví vstupný súbor a inicializuje zásobníky
 * 
 * @param input Súbor z ktorého sa číta
 */
void scannerSetup(FILE *input)
{

	setInput(input);
	stackInit(&indent);
	stackInit(&dedent);
}

/**
 *@brief Zistí či sa jedná o identifikátor alebo keyword
 * 
 *@param str Dynamicky alokované pole na ukladanie reťazcov
 *@param t Token ktorý budeme spracovávať
 *@return 0 značí že t je identifikátor 1 značí keyword
 */
int isKeyword(String *str, Token *t)
{

	if (!strcmp(str->content, "def\0")) t->keyword = DEF_KEYWORD;
	else if (!strcmp(str->content, "else\0")) t->keyword = ELSE_KEYWORD;
	else if (!strcmp(str->content, "if\0")) t->keyword = IF_KEYWORD;
	else if (!strcmp(str->content, "None\0")) t->keyword = NONE_KEYWORD;
	else if (!strcmp(str->content, "pass\0")) t->keyword = PASS_KEYWORD;
	else if (!strcmp(str->content, "return\0")) t->keyword = RETURN_KEYWORD;
	else if (!strcmp(str->content, "while\0")) t->keyword = WHILE_KEYWORD;
	else if (!strcmp(str->content, "inputs\0")) t->keyword = INPUTS_KEYWORD;
	else if (!strcmp(str->content, "inputi\0")) t->keyword = INPUTI_KEYWORD;
	else if (!strcmp(str->content, "inputf\0")) t->keyword = INPUTF_KEYWORD;
	else if (!strcmp(str->content, "print\0")) t->keyword = PRINT_KEYWORD;
	else if (!strcmp(str->content, "len\0")) t->keyword = LEN_KEYWORD;
	else if (!strcmp(str->content, "inputs\0")) t->keyword = INPUTS_KEYWORD;
	else if (!strcmp(str->content, "substr\0")) t->keyword = SUBSTR_KEYWORD;
	else if (!strcmp(str->content, "ord\0")) t->keyword = ORD_KEYWORD;
	else if (!strcmp(str->content, "chr\0")) t->keyword = CHR_KEYWORD;

	else //nejedná sa o keyword, takže to bude identifikátor
	{

		t->tokenType = TOKEN_IDENTIFICATOR;
		t->stringContent = str->content;
		return 0;
	}

	t->tokenType = TOKEN_KEYWORD;
	return 1;
}

/**
 *@brief Stavový automat ktorý jednotlivým lexémom generuje odpovedajúce tokeny, poprípade TOKEN_ERROR
 *       ak sa jedná o chybnú štruktúru alebo nepodporovaný znak
 *@param f Súbor so vstupom
 *@param dynamicString dynamicky alokované pole na ukladanie reťazcov
 *@return Token t odpovedajúci reprezentujúci lexém alebo chybu (TOKEN_ERROR)
 */
Token getToken(String *dynamicString)
{

	int c;
	int STATE = START; //stav automatu
	char strHex[2] = { '0', '0' }; //pomocná premenná pre prácu s hexadecimálnymi escape sekvenciami

	Token t;
	initToken(&t);
	stringClear(dynamicString);

	while (true)
	{

		c = getc(f);

		switch (STATE)
		{

			case START:

				if (dedent.top > 0) //skontrolujeme či je potrebné dogenerovať dedent tokeny. V niektorých prípadoch je potrebné pre jednu zmenu vygenerovať viacej tokenov.
				{

					ungetc(c, f);
					stackPop(&dedent);
					STATE = START;
					return createToken(TOKEN_DEDENT, &t);
				}
                /*MEDZERA*/
				else if (c == WHITESPACE) 
				{

					if (FT) //ak sa na tomto riakdu ešte negeneroval token, zisťujeme zmenu odsadenia
					{

						s++; //počítadlo medzier

						while (true)
						{

							c = getc(f); //načítavame znak po znaku

							if (c < 32 && c != EOF && c != '\n') return createErrorToken(&t);

							if (c == WHITESPACE) s++;

							else if (c == '#') //komentár nesposobuje zmenu odsadenia
							{

								STATE = LINE_COMMENT;
								break;
							}
							else if (c == '\n') //riadok s iba medzerami a odriadkovaním tiež nespôsobuje zmenu odsadenia
							{

								STATE = START;
								s = 0;
								break;
							}
							else if (c != WHITESPACE) //ak príjmeme niečo iné ako medzeru
							{

								ungetc(c, f);
								STATE = INDENT;
								FT = 0;
								break;
							}
						}
					}
					else
					{

						STATE = START;
						break;
					}

					break;
				}
                /*ODRIADKOVANIE*/
				else if (c == '\n')
				{

					STATE = START;
					FT = 1;
					s = 0;
					return createToken(TOKEN_EOL, &t);
				}
                /*EOF*/
				else if (c == EOF)
				{

					STATE = START;
					return createToken(TOKEN_EOF, &t);
				}
                /*NEPODPOROVANÉ ZNAKY*/
				else if (c < 32 || c == '@' || c == '?')
				{

					STATE = START;
					return createErrorToken(&t);
				}
                /*INDENT/DEDENT*/
				else if (FT && c != WHITESPACE && c != '#')
				{

					ungetc(c, f);
					STATE = INDENT;
					break;
				}

				/*SCANNER IDENTIFICATOR*/
				else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_'))
				{

					stringAppend(dynamicString, c);
					STATE = IDENTIFICATOR;
					break;
				}
				/*INTEGER/DOUBLE/ZEROINT*/
				else if (isdigit(c))
				{

					if (c == '0') //ak zaciname 0, musime overit ci nieje prebyocna -> pojde po nej iba bodka
					{
						

						STATE = PDOUBLE; 
						stringAppend(dynamicString, c);
						break;
					}

					STATE = INT;	//ak nezacina 0
					stringAppend(dynamicString, c);
					break;
				}
				/*OPERATORS*/
				else if (c == '(' || c == ')' || c == ':' || c == '+' || c == '-' || c == '*' || c == ',')
				{
					STATE = START;
					return processOperator(c, &t);
				}
				else if (c == '<' || c == '>')
				{

					if (c == '>')
					{

						c = getc(f);

						if (c == '=') //>=
						{

							STATE = START;
							return processOperator(MORE_EQUAL_OPERATOR, &t);
						}
						else
						{
							// >
							ungetc(c, f);
							STATE = START;
							return processOperator(MORE_OPERATOR, &t);
						}
					}
					else
					{
						// c == '<'

						c = getc(f);

						if (c == '=') //<=
						{

							STATE = START;
							return processOperator(LESS_EQUAL_OPERATOR, &t);
						}
						else //<
						{
							
							ungetc(c, f);
							STATE = START;
							return processOperator(LESS_OPERATOR, &t);
						}
					}
				}
				else if (c == '!' || c == '=')
				{

					if (c == '!')
					{

						c = getc(f);

						if (c == '=')
						{

							STATE = START;
							return processOperator(NOT_EQUAL_OPERATOR, &t);
						}
						else    //za vykricnikom moze ist iba =
						{
							

							ungetc(c, f);
							STATE = START;
							return createErrorToken(&t);
						}
					}
					else   // c == '='
					{
						
						c = getc(f);

						if (c == '=') // operator bude ==
						{

							STATE = START;
							return processOperator(EQUAL_OPERATOR, &t);
						}
						else
						{

							ungetc(c, f);
							STATE = START;
							return processOperator(ASIGN_OPERATOR, &t); 
						}
					}
				}
				else if (c == '/') // / delenie
				{

					c = getc(f);

					if (c == '/') // // celociselne delenie
					{

						STATE = START;
						return processOperator(INT_DIVISION_OPERATOR, &t);
					}
					else
					{

						ungetc(c, f);
						STATE = START;
						return processOperator(DIVISION_OPERATOR, &t);
					}

					break;
				}
				else if (c == QUOTE_SIGN)
				{

					STATE = SCANNER_STRING;
					break;
				}
				else if (c == '#')
				{

					STATE = LINE_COMMENT;
					break;
				}
				else if (c == DOUBLE_QUOTE)
				{

					STATE = DOCSTRING;
					break;
				}
				else
				{

					STATE = START;
					return createErrorToken(&t);
				}

				break;
            /*IDENTIFICATOR*/
			case IDENTIFICATOR:

				if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || (c >= '0' && c <= '9'))
				{

					stringAppend(dynamicString, c);
				}
				else
				{

					ungetc(c, f);

					if (isKeyword(dynamicString, &t) == 1)
					{
						//ak je KEYWORD,nastavime KEYWORD a  pozreme sa ci je to DEF

						if (t.keyword == DEF_KEYWORD) t.isFunction = 1;	//ak je to DEF keyword jedná sa o definíciu funkcie
						else t.isFunction = 0;
					}
					return t;
				}

				break;

			case PDOUBLE:	//prijali sme 0, overíme či sa jedná o samotnú 0 alebo desatinné číslo, v prípade prebytočnej 0 vrátime chybový token

				if (c == '.') //bude to double
				{

					char p = getc(f);

					if (!isdigit(p)) //za bodkou musí nasledovať číslo
					{

						ungetc(p, f);
						STATE = START;
						return createErrorToken(&t);
					}

					ungetc(p, f);
					STATE = SCANNER_DOUBLE;
					stringAppend(dynamicString, c);
				}
				else if (isdigit(c)) //prebytočná 0
				{

					STATE = START;
					return createErrorToken(&t);
				}
				else if (c == 'e' || c == 'E') //0 s exponentom
				{

					char x = getc(f);

					if ((isdigit(x) == 0) && x != '-' && x != '+')
					{

						ungetc(x, f);
						STATE = START;
						return createErrorToken(&t);
					}

					ungetc(x, f);
					STATE = EXPINT;
					stringAppend(dynamicString, c);
				}
				else //samotná 0
				{

					ungetc(c, f);
					STATE = START;
					t.integer = (int) strtol(dynamicString->content, NULL, 10);
					return createToken(TOKEN_INTEGER, &t);
				}

				break;

			case INT:

				if (c == '.') //bude to double
				{

					char p = getc(f);

					if (!isdigit(p))
					{

						ungetc(p, f);
						STATE = START;
						return createErrorToken(&t);
					}

					ungetc(p, f);
					STATE = SCANNER_DOUBLE;
					stringAppend(dynamicString, c);
				}
				else if (isdigit(c)) //kým prichádzajú čísla, ukladaj ich
				{

					stringAppend(dynamicString, c);
				}
				else if (isalpha(c) && c != 'E' && c != 'e') //písmeno prísť nemôže, okrem eE
				{

					STATE = START;
					return createErrorToken(&t);
				}
				else if (c == 'e' || c == 'E') //ak prišlo e alebo E
				{

					char x = getc(f);

					if ((isdigit(x) == 0) && x != '-' && x != '+') //môže nasledovať iba +,- alebo číslo 
					{
						ungetc(x, f);
						STATE = START;
						return createErrorToken(&t);
					}

					ungetc(x, f);
					STATE = EXPINT;
					stringAppend(dynamicString, c);
				}
				else 
				{

					ungetc(c, f);
					STATE = START;
					t.integer = (int) strtol(dynamicString->content, NULL, 10);

					return createToken(TOKEN_INTEGER, &t);
				}

				break;

			case SCANNER_DOUBLE:   

				if (isdigit(c))
				{

					stringAppend(dynamicString, c);
				}
				else if (c == 'e' || c == 'E')
				{

					if (isdigit(lastChar(dynamicString))) //pred znakom exponentu musí byť číslo...
					{
						

						STATE = EXPDOUBLE;
						stringAppend(dynamicString, c);
					}
					else return createErrorToken(&t);

					char p = getc(f);

					if ((isdigit(p) == 0) && p != '-' && p != '+')
					{

						ungetc(p, f);
						STATE = START;
						return createErrorToken(&t);
					}

					ungetc(p, f);
				}
				else if (isalpha(c) && c != 'E' && c != 'e')
				{

					STATE = START;
					return createErrorToken(&t);
				}
				else 
				{
					
					ungetc(c, f);
					STATE = START;
					t.realNumber = (double) strtod(dynamicString->content, NULL);

					return createToken(TOKEN_DOUBLE, &t);
				}

				break;

			case EXPDOUBLE:

				if (isdigit(c))
				{

					stringAppend(dynamicString, c);
				}
				else if (c == '+' || c == '-')
				{

					char p = getc(f);

					if (!isdigit(p))
					{

						ungetc(p, f);
						STATE = START;
						return createErrorToken(&t);
					}

					ungetc(p, f);

					stringAppend(dynamicString, c);
				}
				else if (c == '.')
				{

					STATE = START;
					return createErrorToken(&t);
				}
				else if (isalpha(c))
				{

					ungetc(c, f);
					STATE = START;
					return createErrorToken(&t);
				}
				else   //skoncili cisla
				{
					

					ungetc(c, f);

					STATE = START;
					t.realNumber = (double) atof(dynamicString->content);
					return createToken(TOKEN_EXPONENT_DOUBLE, &t);
				}

				break;

			case SCANNER_STRING:

				if (c < 32) return createErrorToken(&t); //nepovolené znaky
				else if (c == '\\') STATE = ESCAPESEQ; //escape sekvencia začína spatným lomítkom
				else if (c == '\n' || c == EOF) return createErrorToken(&t);
				else if (c == QUOTE_SIGN) //ukončovacia úvodzovka
				{

					t.stringContent = dynamicString->content;
					return createToken(TOKEN_STRING, &t);
				}
				else stringAppend(dynamicString, c);

				break;

			case ESCAPESEQ:

				switch (c)
				{

					case 'n':
						stringAppend(dynamicString, '\n');
						STATE = SCANNER_STRING;
						break;

					case 't':
						stringAppend(dynamicString, '\t');
						STATE = SCANNER_STRING;
						break;

					case '\"':
						stringAppend(dynamicString, '\"');
						STATE = SCANNER_STRING;
						break;

					case '\\':
						stringAppend(dynamicString, '\\');
						STATE = SCANNER_STRING;
						break;

					case '\'':
						stringAppend(dynamicString, '\'');
						STATE = SCANNER_STRING;
						break;

					case 'x':
						STATE = HEXESCAPESEQ;
						break;

					default:
						stringAppend(dynamicString, '\\'); //ak nejde o známu escape sekvenciu zapíšeme lomítko a znak
						stringAppend(dynamicString, c);
						STATE = SCANNER_STRING;
						break;
				}

				break;

			case HEXESCAPESEQ:

				if (isdigit(c) || (c >= 'A' && c <= 'F')) //formát xhh kde hh je práve dvojmiestne v rozsahu 00 do FF
				{

					if (c > 'F') return createErrorToken(&t);

					strHex[0] = c;

					c = getc(f);

					if (isdigit(c) || (c >= 'A' && c <= 'F'))
					{

						if (c > 'F') return createErrorToken(&t);

						strHex[1] = c;

						int hex = (int) strtol(strHex, NULL, 16);
						stringAppend(dynamicString, (char) hex);

						c = getc(f);

						if (isalnum(c))
						{

							ungetc(c, f);
							STATE = START;
							return createErrorToken(&t);
						}

						ungetc(c, f);
						STATE = SCANNER_STRING;
						break;
					}
					else
					{

						STATE = START;
						return createErrorToken(&t);
					}
				}
				else return createErrorToken(&t);

				break;

			case LINE_COMMENT:	//všetko ignorujeme, ak je na danom riadku iba komentár negenerujeme nič

				if (c == '\n')
				{

					if (FT)
					{

						STATE = START;
						s = 0;
						break;
					}

					STATE = START;
					FT = 1;
					s = 0;
					return createToken(TOKEN_EOL, &t);	//ak pred komentárom niečo bolo generujeme aj EOL
				}
				else if (c == EOF)
				{

					STATE = START;
					return createToken(TOKEN_EOF, &t);
				}

				break;

			case DOCSTRING:

				if (c == '\"')
				{

					c = getc(f);

					if (c == '\"')
					{
						STATE = DOCSTRING_P;
						break;
					}
					else
					{

						ungetc(c, f);
						STATE = START;
						return createErrorToken(&t);
					}
				}
				else
				{

					ungetc(c, f);
					STATE = START;
					return createErrorToken(&t);
				}

				break;

			case DOCSTRING_P:

				if (c != '\n' && c < 32) return createErrorToken(&t);

				if (c == '\\')
				{

					c = getc(f);

					switch (c)
					{

						case '\"':
							stringAppend(dynamicString, '\"');
							STATE = DOCSTRING_P;
							break;

						default:
							stringAppend(dynamicString, '\\');
							stringAppend(dynamicString, c);
							STATE = DOCSTRING_P;
							break;
					}

					break;
				}
				else if (c == '\"')
				{

					c = getc(f);

					if (c == '\"')
					{

						c = getc(f);

						if (c == '\"')
						{

							char p = getc(f);

							if (p > 32 && p != ')' && p != '#') //za dokumentačným riadkom môže nasledovať iba biely znak poprípade ) v printe alebo komentár
							{

								STATE = START;
								ungetc(p, f);
								return createErrorToken(&t);
							}

							ungetc(p, f);

							t.stringContent = dynamicString->content;
							STATE = START;
							return createToken(TOKEN_DOCSTRING, &t);
						}
						else
						{

							ungetc(c, f);
							STATE = START;
							return createErrorToken(&t);
						}
					}
					else
					{

						ungetc(c, f);
						STATE = START;
						return createErrorToken(&t);
					}
				}
				else
				{

					stringAppend(dynamicString, c);
				}
				break;

			case EXPINT:

				if (isdigit(c)) stringAppend(dynamicString, c);

				else if (c == '+' || c == '-')
				{

					char p = getc(f);

					if (!isdigit(p))
					{

						ungetc(p, f);
						STATE = START;
						return createErrorToken(&t);
					}

					ungetc(p, f);
					stringAppend(dynamicString, c);
				}
				else if (c == '.')
				{

					STATE = START;
					return createErrorToken(&t);
				}
				else if (isalpha(c))
				{

					ungetc(c, f);
					STATE = START;
					return createErrorToken(&t);
				}
				else
				{
					//skoncili cisla

					ungetc(c, f);
					STATE = START;
					t.realNumber = (double) atof(dynamicString->content);
					return createToken(TOKEN_EXPONENT_INTEGER, &t);
				}
				break;

			case INDENT:

				ungetc(c, f);
				STATE = START;
				int v = stackTop(&indent);

				if (s > v) //ak sa zvýši odsadenie generujeme indent
				{

					stackPush(&indent, s);
					v = stackTop(&indent);
					FT = 0;
					return createToken(TOKEN_INDENT, &t);
				}
				else if (s < v) //naopak ak sa zníži generujeme dedent
				{

					int nT; //vrchol pomocného zásobniku s hodnotami odsadenia

					do {

						stackPop(&indent);

						nT = stackTop(&indent);
						FT = 0;

						stackPush(&dedent, 1); //v prípade zmeny odsadenia musíme v niektorých prípadoch dogenerovať naraz viac tokenov,funkcia však vracia naraz iba jeden
                                              //preto sa pri každom volaní pozeráme či netreba dogenerovať dedent
						if (nT == s) break;

						if (nT == 0)
						{

							return createErrorToken(&t);
						}
					} while (nT != s);

					break;
				}
				else if (s == v) //nejde o zmenu odsadenia
				{

					FT = 0;
					break;
				}
                break;
		}
	}
}
