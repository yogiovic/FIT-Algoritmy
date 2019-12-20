/**
 *
 *      Projekt IFJ 2019
 *
 * @brief      Práca so štruktúrou token
 *
 * @author     xondre12
 * @date       09.11.2019
 */
#include "token.h"
/**
 * @brief Inicializuje štruktúru
 * 
 * @param t Token ktorý bude inicializovovaný
 */
void initToken(Token *t){
   t->tokenType = TOKEN_NO_TYPE;
   t->isFunction = 0;
   t->operatorSign = 0;
   t->integer = 0;
   t->keyword = 0;
   t->realNumber = 0;
   t->stringContent = NULL;
}
/**
 * @brief Nastaví typ tokenu ako chybový token a vráti tento token
 * 
 * @param t Vstupný token z ktorého spravíme chybový
 * @return Token chybový token
 */
Token createErrorToken(Token *t){
    t->tokenType = TOKEN_ERROR;
    return *t;
}

/**
 * @brief Nastaví typ tokenu a vráti token
 * 
 * @param type Typ tokenu na aký chceme token nastaviť
 * @param t Token ktorého typ bude nastavený
 * @return Token ktorý sa vráti
 */
Token createToken(int type, Token *t) {
    
    t->tokenType = type;
    return *t;

}
/**
 * @brief Inicizalizuje zásobník
 * 
 * @param stack Zásobník ktorý bude inicializovaný
 */
void stackInit(Stack *stack) {

    stack->pole[0] = 0;
    stack->top = 0;

}
/**
 * @brief Vloží na zásobník
 * 
 * @param stack Zásobník na ktorý budeme vkladať
 * @param element Prvok ktorý bude vkladaný
 */
void stackPush(Stack *stack, int element) {

    stack->top += 1;
    stack->pole[stack->top] = element;
    
}
/**
 * @brief Vyjmeme zo stacku
 * 
 * @param stack Zásobník z ktorého budeme vyjímať
 */
void stackPop(Stack *stack) {

    stack->top--;
}
/**
 * @brief Vráti hodnotu na vrchu zásobníku
 * 
 * @param stack Zásobník ktorého vrchný prvok nás zaujíma
 * @return int Hodnota na vrchu zásobníku
 */
int stackTop(Stack *stack) {

    return stack->pole[stack->top];;
}

/**
 * @brief Nastaví typ a atribút tokenu
 * 
 * @param OPERATOR Typ operátoru
 * @param t Štruktúra do ktorej ukladáme informácie (t)
 * @return Token 
 */
Token processOperator(int OPERATOR, Token *t) {
    
    t->tokenType = TOKEN_OPERATOR;
    t->operatorSign = OPERATOR;
    return *t;

}
