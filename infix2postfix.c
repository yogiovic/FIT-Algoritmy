/*
	Adam Ondrejka,xondre12(2BIT) IAL 1.DU

*/

/* ******************************* c204.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c204 - Převod infixového výrazu na postfixový (s využitím c202)     */
/*  Referenční implementace: Petr Přikryl, listopad 1994                      */
/*  Přepis do jazyka C: Lukáš Maršík, prosinec 2012                           */
/*  Upravil: Kamil Jeřábek, září 2019                                         */
/* ************************************************************************** */
/*
** Implementujte proceduru pro převod infixového zápisu matematického
** výrazu do postfixového tvaru. Pro převod využijte zásobník (tStack),
** který byl implementován v rámci příkladu c202. Bez správného vyřešení
** příkladu c202 se o řešení tohoto příkladu nepokoušejte.
**
** Implementujte následující funkci:
**
**    infix2postfix .... konverzní funkce pro převod infixového výrazu 
**                       na postfixový
**
** Pro lepší přehlednost kódu implementujte následující pomocné funkce:
**    
**    untilLeftPar .... vyprázdnění zásobníku až po levou závorku
**    doOperation .... zpracování operátoru konvertovaného výrazu
**
** Své řešení účelně komentujte.
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c204.h"
int solved;

/*
** Pomocná funkce untilLeftPar.
** Slouží k vyprázdnění zásobníku až po levou závorku, přičemž levá závorka
** bude také odstraněna. Pokud je zásobník prázdný, provádění funkce se ukončí.
**
** Operátory odstraňované ze zásobníku postupně vkládejte do výstupního pole
** znaků postExpr. Délka převedeného výrazu a též ukazatel na první volné
** místo, na které se má zapisovat, představuje parametr postLen.
**
** Aby se minimalizoval počet přístupů ke struktuře zásobníku, můžete zde
** nadeklarovat a používat pomocnou proměnnou typu char.
*/
void untilLeftPar ( tStack* s, char* postExpr, unsigned* postLen ) {
	
	char stc; //pomocna premenna na porovnavanie
	while ( stackEmpty(s) != 1 ) {
		
		stackTop(s, &stc);
		stackPop(s);
		 
		 if ( stc == '(' ) break; //dokial narazime na lavu zatvorku
		 
		 else {
			 
			 postExpr[(*postLen)++] = stc; //vyberame zo zasobmniku a ukladame
		 }
	}


}

/*
** Pomocná funkce doOperation.
** Zpracuje operátor, který je předán parametrem c po načtení znaku ze
** vstupního pole znaků.
**
** Dle priority předaného operátoru a případně priority operátoru na
** vrcholu zásobníku rozhodneme o dalším postupu. Délka převedeného 
** výrazu a taktéž ukazatel na první volné místo, do kterého se má zapisovat, 
** představuje parametr postLen, výstupním polem znaků je opět postExpr.
*/
void doOperation ( tStack* s, char c, char* postExpr, unsigned* postLen ) {

	if ( stackEmpty(s) == 1 ) { //ak by bol prazdny
		stackPush(s, c);
		return;
	}
	
	else {
		char stc; // pomocna premenna ..
		stackTop(s, &stc);

			if ( stc == '(' ) { //ak je to zatvorka
				stackPush(s, c);
				return;
			}

			else if (( stc == '-' || stc == '+' ) && (c == '*' || c == '/' )) { //na vrchu stacku je operator s mensiou prioritou
				stackPush(s, c);
				return;
			}

			else {
				postExpr[(*postLen)++] = stc; //pridame a inkrementujeme
				stackPop(s);
				doOperation(s, c, postExpr, postLen);
			}
	}


}

/* 
** Konverzní funkce infix2postfix.
** Čte infixový výraz ze vstupního řetězce infExpr a generuje
** odpovídající postfixový výraz do výstupního řetězce (postup převodu
** hledejte v přednáškách nebo ve studijní opoře). Paměť pro výstupní řetězec
** (o velikosti MAX_LEN) je třeba alokovat. Volající funkce, tedy
** příjemce konvertovaného řetězce, zajistí korektní uvolnění zde alokované
** paměti.
**
** Tvar výrazu:
** 1. Výraz obsahuje operátory + - * / ve významu sčítání, odčítání,
**    násobení a dělení. Sčítání má stejnou prioritu jako odčítání,
**    násobení má stejnou prioritu jako dělení. Priorita násobení je
**    větší než priorita sčítání. Všechny operátory jsou binární
**    (neuvažujte unární mínus).
**
** 2. Hodnoty ve výrazu jsou reprezentovány jednoznakovými identifikátory
**    a číslicemi - 0..9, a..z, A..Z (velikost písmen se rozlišuje).
**
** 3. Ve výrazu může být použit předem neurčený počet dvojic kulatých
**    závorek. Uvažujte, že vstupní výraz je zapsán správně (neošetřujte
**    chybné zadání výrazu).
**
** 4. Každý korektně zapsaný výraz (infixový i postfixový) musí být uzavřen 
**    ukončovacím znakem '='.
**
** 5. Při stejné prioritě operátorů se výraz vyhodnocuje zleva doprava.
**
** Poznámky k implementaci
** -----------------------
** Jako zásobník použijte zásobník znaků tStack implementovaný v příkladu c202. 
** Pro práci se zásobníkem pak používejte výhradně operace z jeho rozhraní.
**
** Při implementaci využijte pomocné funkce untilLeftPar a doOperation.
**
** Řetězcem (infixového a postfixového výrazu) je zde myšleno pole znaků typu
** char, jenž je korektně ukončeno nulovým znakem dle zvyklostí jazyka C.
**
** Na vstupu očekávejte pouze korektně zapsané a ukončené výrazy. Jejich délka
** nepřesáhne MAX_LEN-1 (MAX_LEN i s nulovým znakem) a tedy i výsledný výraz
** by se měl vejít do alokovaného pole. Po alokaci dynamické paměti si vždycky
** ověřte, že se alokace skutečně zdrařila. V případě chyby alokace vraťte namísto
** řetězce konstantu NULL.
*/
char* infix2postfix (const char* infExpr) {

	unsigned postLen = 0; //dlzka vyrazu
	char i = *infExpr;

	char tmp; //docasna premenna

	tStack* stack = malloc(sizeof(tStack));

	if ( stack == NULL ) {
		return NULL;
	}

	else stackInit(stack);

	char* postExpr = malloc(MAX_LEN * sizeof(char));

	if ( postExpr == NULL ) { //overime malloc 
		free(stack);
		return NULL;
	}

	while( i != '\0') {
		
		i = *infExpr++; //posuvame sa

		if ( i == '(') stackPush(stack, i); 

		else if ( i == ')' ) untilLeftPar(stack, postExpr, &postLen);
		
		else if (( i >= 'a' && i <= 'z' ) || ( i >= 'A' && i <= 'Z' ) || ( i >= '0' && i <= '9' )) postExpr[postLen++] = i; //operand
	
		else if ( i == '+' || i == '-' || i == '*' || i == '/' ) doOperation(stack, i, postExpr, &postLen); //operator

		else if ( i == '=' ) { //koniec

			while (stackEmpty(stack) != 1) { //kym stack nieje prazdny
				
				stackTop(stack, &tmp);
				stackPop(stack); //popujeme zo stacku ..

				postExpr[postLen++] = tmp;

			}
		
		postExpr[postLen++] = '=';
		break;
		
		}

	}

	postExpr[postLen] = '\0'; //aby sme predisli undefined behaviour
	free(stack); //upraceme
	return postExpr;
}

/* Konec c204.c */