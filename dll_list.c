/*
    Adam Ondrejka,xondre12(2BIT) IAL 1.DU

*/

/* c206.c **********************************************************}
{* Téma: Dvousměrně vázaný lineární seznam
**
**                   Návrh a referenční implementace: Bohuslav Křena, říjen 2001
**                            Přepracované do jazyka C: Martin Tuček, říjen 2004
**                                            Úpravy: Kamil Jeřábek, září 2019
**
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int.
** Seznam bude jako datová abstrakce reprezentován proměnnou
** typu tDLList (DL znamená Double-Linked a slouží pro odlišení
** jmen konstant, typů a funkcí od jmen u jednosměrně vázaného lineárního
** seznamu). Definici konstant a typů naleznete v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu
** s výše uvedenou datovou částí abstrakce tvoří abstraktní datový typ
** obousměrně vázaný lineární seznam:
**
**      DLInitList ...... inicializace seznamu před prvním použitím,
**      DLDisposeList ... zrušení všech prvků seznamu,
**      DLInsertFirst ... vložení prvku na začátek seznamu,
**      DLInsertLast .... vložení prvku na konec seznamu,
**      DLFirst ......... nastavení aktivity na první prvek,
**      DLLast .......... nastavení aktivity na poslední prvek,
**      DLCopyFirst ..... vrací hodnotu prvního prvku,
**      DLCopyLast ...... vrací hodnotu posledního prvku,
**      DLDeleteFirst ... zruší první prvek seznamu,
**      DLDeleteLast .... zruší poslední prvek seznamu,
**      DLPostDelete .... ruší prvek za aktivním prvkem,
**      DLPreDelete ..... ruší prvek před aktivním prvkem,
**      DLPostInsert .... vloží nový prvek za aktivní prvek seznamu,
**      DLPreInsert ..... vloží nový prvek před aktivní prvek seznamu,
**      DLCopy .......... vrací hodnotu aktivního prvku,
**      DLActualize ..... přepíše obsah aktivního prvku novou hodnotou,
**      DLSucc .......... posune aktivitu na další prvek seznamu,
**      DLPred .......... posune aktivitu na předchozí prvek seznamu,
**      DLActive ........ zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce
** explicitně uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam 
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako
** procedury (v jazyce C procedurám odpovídají funkce vracející typ void).
**/

#include "c206.h"

int solved;
int errflg;

void DLError() {
/*
** Vytiskne upozornění na to, že došlo k chybě.
** Tato funkce bude volána z některých dále implementovaných operací.
**/	
    printf ("*ERROR* The program has performed an illegal operation.\n");
    errflg = TRUE;             /* globální proměnná -- příznak ošetření chyby */
    return;
}

void DLInitList (tDLList *L) {
/*
** Provede inicializaci seznamu L před jeho prvním použitím (tzn. žádná
** z následujících funkcí nebude volána nad neinicializovaným seznamem).
** Tato inicializace se nikdy nebude provádět nad již inicializovaným
** seznamem, a proto tuto možnost neošetřujte. Vždy předpokládejte,
** že neinicializované proměnné mají nedefinovanou hodnotu.
**/
    L->First = NULL;
    L->Last = NULL; 
    L->Act = NULL; //vsetko NULL...
}

void DLDisposeList (tDLList *L) {
/*
** Zruší všechny prvky seznamu L a uvede seznam do stavu, v jakém
** se nacházel po inicializaci. Rušené prvky seznamu budou korektně
** uvolněny voláním operace free. 
**/

    tDLElemPtr element = L->First;

    while( L->First != NULL ) {
        L->First = L->First->rptr;
        free(element); //postupne uvolnime vsetky elementy zo zoznamu
        element = L->First;
    }

    L->First = NULL;
    L->Last = NULL; 
    L->Act = NULL;
}

void DLInsertFirst (tDLList *L, int val) {
/*
** Vloží nový prvek na začátek seznamu L.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
    tDLElemPtr element = (tDLElemPtr) malloc(sizeof(struct tDLElem));
    
    if ( element == NULL ) { //nepodaril sa malloc
        DLError();
        return;
    }

    element->lptr = NULL; 
    element->rptr = L->First;
    element->data = val; //nastavime hodnotu

    if ( L->First != NULL ) L->First->lptr = element;
    
    else L->Last = element;

    L->First = element; //vkladali sme na zaciatok
    
}

void DLInsertLast(tDLList *L, int val) {
/*
** Vloží nový prvek na konec seznamu L (symetrická operace k DLInsertFirst).
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/ 	
     tDLElemPtr element = (tDLElemPtr) malloc(sizeof(struct tDLElem));
    
    if ( element == NULL ) { //ach ten malloc...
        DLError();
        return;
    }

    element->lptr = L->Last;
    element->rptr = NULL;
    element->data = val; //...

    if ( L->Last != NULL ) L->Last->rptr = element;

    else L->First = element;

    L->Last = element; //vkladali sme na koniec
}

void DLFirst (tDLList *L) {
/*
** Nastaví aktivitu na první prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/
    L->Act = L->First; //Aktivny bude prvy
}

void DLLast (tDLList *L) {
/*
** Nastaví aktivitu na poslední prvek seznamu L.
** Funkci implementujte jako jediný příkaz (nepočítáme-li return),
** aniž byste testovali, zda je seznam L prázdný.
**/

    L->Act = L->Last; //...
}

void DLCopyFirst (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu prvního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/

    if( L->First == NULL ) { //je co kopirovat?
        DLError();
        return;
    }

    else *val = L->First->data; //je co kopirovat !
}

void DLCopyLast (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu posledního prvku seznamu L.
** Pokud je seznam L prázdný, volá funkci DLError().
**/
    if( L->Last == NULL ) { 
        DLError();
        return;
    }

    else *val = L->Last->data; //...
}

void DLDeleteFirst (tDLList *L) {
/*
** Zruší první prvek seznamu L. Pokud byl první prvek aktivní, aktivita 
** se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/
    tDLElemPtr firstElement = L->First;
    
    if ( firstElement != NULL ) { //nikomu neverim...

        if ( L->Act == firstElement ) L->Act = NULL; //uz nieje aktivny

        if ( L->Last == firstElement ) L->Last = NULL; //ak by sme mali iba jeden
        
        else firstElement->rptr->lptr = NULL; //je ich tam viac

        L->First = firstElement->rptr;

        free(firstElement); //upratat 
    }
}	

void DLDeleteLast (tDLList *L) {
/*
** Zruší poslední prvek seznamu L. Pokud byl poslední prvek aktivní,
** aktivita seznamu se ztrácí. Pokud byl seznam L prázdný, nic se neděje.
**/ 

    tDLElemPtr lastElement = L->Last;

    if (lastElement != NULL) {
        
        if ( L->Act == lastElement ) L->Act = NULL;

        if ( L->First == lastElement) L->First = NULL;

        else lastElement->lptr->rptr = NULL;

        L->Last = lastElement->lptr;

        free(lastElement); //upraceme ...
    }
}

void DLPostDelete (tDLList *L) {
/*
** Zruší prvek seznamu L za aktivním prvkem.
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** posledním prvkem seznamu, nic se neděje.
**/
	if(L->Act != NULL) { //ma sa nieco diat?
        
        if (L->Act != L->Last) { //je posledny?

            tDLElemPtr postElement = L->Act->rptr; 

            L->Act->rptr = postElement->rptr; 

            if ( postElement == L->Last ) L->Last = L->Act; //presunieme aktivitu ...

            else postElement->rptr->lptr = L->Act; //nastavime aktivitu ...

            free(postElement);
        }
    }
}

void DLPreDelete (tDLList *L) {
/*
** Zruší prvek před aktivním prvkem seznamu L .
** Pokud je seznam L neaktivní nebo pokud je aktivní prvek
** prvním prvkem seznamu, nic se neděje.
**/
    if(L->Act != NULL) { 
        
        if (L->Act != L->First) {

            tDLElemPtr preElement = L->Act->lptr;

            L->Act->lptr = preElement->lptr;

            if ( preElement == L->First ) L->First = L->Act;

            else preElement->lptr->rptr = L->Act;

            free(preElement);
        }
    }
}

void DLPostInsert (tDLList *L, int val) {
/*
** Vloží prvek za aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
    if(L->Act != NULL) {
        
        tDLElemPtr newElement =  (tDLElemPtr) malloc(sizeof(struct tDLElem));

        if(newElement == NULL) { 
            DLError();
            return;
        }
        
        else {
            newElement->rptr = L->Act->rptr;
            newElement->lptr = L->Act;
            newElement->data = val; //nastavime hodnoty noveho prvku

            if ( L->Act == L->Last ) L->Last = newElement; //ak je aktivny prvok posledny

            else L->Act->rptr->lptr = newElement;

            L->Act->rptr = newElement;
            
        }
    }
}

void DLPreInsert (tDLList *L, int val) {
/*
** Vloží prvek před aktivní prvek seznamu L.
** Pokud nebyl seznam L aktivní, nic se neděje.
** V případě, že není dostatek paměti pro nový prvek při operaci malloc,
** volá funkci DLError().
**/
      if(L->Act != NULL) {
        
        tDLElemPtr newElement =  (tDLElemPtr) malloc(sizeof(struct tDLElem));

        if(newElement == NULL) {
            DLError();
            return;
        }
        
        else {
            newElement->lptr = L->Act->lptr;
            newElement->rptr = L->Act;
            newElement->data = val;

            if ( L->Act == L->First ) L->First = newElement; //ak je aktivny prvok prvy

            else L->Act->lptr->rptr = newElement;

            L->Act->lptr = newElement;
            
        }
    }
}

void DLCopy (tDLList *L, int *val) {
/*
** Prostřednictvím parametru val vrátí hodnotu aktivního prvku seznamu L.
** Pokud seznam L není aktivní, volá funkci DLError ().
**/
    if (L->Act == NULL) {
        DLError();
        return;
    }

    *val = L->Act->data; //skopirujeme data

}

void DLActualize (tDLList *L, int val) {
/*
** Přepíše obsah aktivního prvku seznamu L.
** Pokud seznam L není aktivní, nedělá nic.
**/
    if ( L->Act != NULL ) L->Act->data = val; //najprv musime overit ci je niaky aktivny, potom prepiseme hodnoty
}

void DLSucc (tDLList *L) {
/*
** Posune aktivitu na následující prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
**/
	if (L->Act != NULL) L->Act = L->Act->rptr; //posunieme sa...
	
}


void DLPred (tDLList *L) {
/*
** Posune aktivitu na předchozí prvek seznamu L.
** Není-li seznam aktivní, nedělá nic.
** Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
**/
	if (L->Act != NULL) L->Act = L->Act->lptr; //....
}

int DLActive (tDLList *L) {
/*
** Je-li seznam L aktivní, vrací nenulovou hodnotu, jinak vrací 0.
** Funkci je vhodné implementovat jedním příkazem return.
**/
    return L->Act != NULL; //...
}

/* Konec c206.c*/
