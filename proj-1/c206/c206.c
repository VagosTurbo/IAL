/* ******************************* c206.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c206 - Dvousměrně vázaný lineární seznam                            */
/*  Návrh a referenční implementace: Bohuslav Křena, říjen 2001               */
/*  Vytvořil: Martin Tuček, říjen 2004                                        */
/*  Upravil: Kamil Jeřábek, září 2020                                         */
/*           Daniel Dolejška, září 2021                                       */
/*           Daniel Dolejška, září 2022                                       */
/* ************************************************************************** */
/*
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int. Seznam bude jako datová
** abstrakce reprezentován proměnnou typu DLList (DL znamená Doubly-Linked
** a slouží pro odlišení jmen konstant, typů a funkcí od jmen u jednosměrně
** vázaného lineárního seznamu). Definici konstant a typů naleznete
** v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ obousměrně
** vázaný lineární seznam:
**
**      DLL_Init ........... inicializace seznamu před prvním použitím,
**      DLL_Dispose ........ zrušení všech prvků seznamu,
**      DLL_InsertFirst .... vložení prvku na začátek seznamu,
**      DLL_InsertLast ..... vložení prvku na konec seznamu,
**      DLL_First .......... nastavení aktivity na první prvek,
**      DLL_Last ........... nastavení aktivity na poslední prvek,
**      DLL_GetFirst ....... vrací hodnotu prvního prvku,
**      DLL_GetLast ........ vrací hodnotu posledního prvku,
**      DLL_DeleteFirst .... zruší první prvek seznamu,
**      DLL_DeleteLast ..... zruší poslední prvek seznamu,
**      DLL_DeleteAfter .... ruší prvek za aktivním prvkem,
**      DLL_DeleteBefore ... ruší prvek před aktivním prvkem,
**      DLL_InsertAfter .... vloží nový prvek za aktivní prvek seznamu,
**      DLL_InsertBefore ... vloží nový prvek před aktivní prvek seznamu,
**      DLL_GetValue ....... vrací hodnotu aktivního prvku,
**      DLL_SetValue ....... přepíše obsah aktivního prvku novou hodnotou,
**      DLL_Previous ....... posune aktivitu na předchozí prvek seznamu,
**      DLL_Next ........... posune aktivitu na další prvek seznamu,
**      DLL_IsActive ....... zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce explicitně
 * uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c206.h"

bool error_flag;
bool solved;

/**
 * Vytiskne upozornění na to, že došlo k chybě.
 * Tato funkce bude volána z některých dále implementovaných operací.
 */
void DLL_Error(void) {
	printf("*ERROR* The program has performed an illegal operation.\n");
	error_flag = true;
}

/**
 * Provede inicializaci seznamu list před jeho prvním použitím (tzn. žádná
 * z následujících funkcí nebude volána nad neinicializovaným seznamem).
 * Tato inicializace se nikdy nebude provádět nad již inicializovaným seznamem,
 * a proto tuto možnost neošetřujte.
 * Vždy předpokládejte, že neinicializované proměnné mají nedefinovanou hodnotu.
 *
 * @param list Ukazatel na strukturu dvousměrně vázaného seznamu
 */
void DLL_Init( DLList *list ) {
	list->firstElement = list->activeElement = list->lastElement = NULL;
}

/**
 * Zruší všechny prvky seznamu list a uvede seznam do stavu, v jakém se nacházel
 * po inicializaci.
 * Rušené prvky seznamu budou korektně uvolněny voláním operace free.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Dispose( DLList *list ) {
	
	// uvolnenie kazdeho elementu
	while (list->firstElement != NULL)
	{
		DLLElementPtr tmp = list->firstElement;
		list->firstElement = list->firstElement->nextElement;
		free(tmp);
	}
	
	// nakoniec zrusenie zvysnych pointerov
	list->activeElement = list->lastElement = NULL;
}

/**
 * Vloží nový prvek na začátek seznamu list.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na začátek seznamu
 */
void DLL_InsertFirst( DLList *list, int data ) {
	// alokuje pamät pre novy element
	DLLElementPtr new = malloc(sizeof(struct DLLElement));
	if (new == NULL){
		DLL_Error();
		return;
	}
	new->data = data;
	new->previousElement = NULL;

	// ak je prazdne pole tak nastav novy aj ako posledny
	if (list->firstElement == NULL){
		new->nextElement = NULL;
		list->lastElement = new;
	}
	else{
		// ak nieje prazdne pole tak nastav novy ako prvy
		new->nextElement = list->firstElement;
		list->firstElement->previousElement = new;
	}

	list->firstElement = new;
}

/**
 * Vloží nový prvek na konec seznamu list (symetrická operace k DLL_InsertFirst).
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na konec seznamu
 */
void DLL_InsertLast( DLList *list, int data ) {
	// alokuje pamät pre novy element
	DLLElementPtr new = malloc(sizeof(struct DLLElement));
	if (new == NULL){
		DLL_Error();
		return;
	}
	new->data = data;
	new->nextElement = NULL;

	// ak je prazdne pole tak nastav novy aj ako prvy
	if (list->lastElement == NULL){
		new->previousElement = NULL;
		list->firstElement = new;
	}
	else{
		// ak nieje prazdne pole tak nastav novy ako posledny
		new->previousElement = list->lastElement;
		list->lastElement->nextElement = new;
	}

	list->lastElement = new;
}

/**
 * Nastaví první prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_First( DLList *list ) {
	list->activeElement = list->firstElement;
}

/**
 * Nastaví poslední prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz, aniž byste testovali,
 * zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Last( DLList *list ) {
	list->activeElement = list->lastElement;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu prvního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetFirst( DLList *list, int *dataPtr ) {
	// ak je list prazdny tak error
	if (list->firstElement == NULL)
		DLL_Error();
	else
		*dataPtr = list->firstElement->data;
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu posledního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetLast( DLList *list, int *dataPtr ) {
	// ak je list prazdny tak error
	if (list->lastElement == NULL)
		DLL_Error();
	else 
		*dataPtr = list->lastElement->data;
}

/**
 * Zruší první prvek seznamu list.
 * Pokud byl první prvek aktivní, aktivita se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteFirst( DLList *list ) {
	// ak je zoznam prazdny skip
	if (list->firstElement != NULL){

		DLLElementPtr tmp = list->firstElement;
		// ak je prvy element aktivny tak ho zrusime
		if (list->activeElement == list->firstElement)
			list->activeElement = NULL;

		// ak je prvy element aj posledny tak zrusime aj posledny
		if (list->firstElement == list->lastElement){
			list->firstElement = NULL;
			list->lastElement = NULL;
		}
		else{
			list->firstElement = list->firstElement->nextElement;
			list->firstElement->previousElement = NULL;
		}
		free(tmp);
	}
}

/**
 * Zruší poslední prvek seznamu list.
 * Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteLast( DLList *list ) {
	// ak je zoznam prazdny tak skip
	if (list->lastElement != NULL){
	
		DLLElementPtr tmp = list->lastElement;
		//ak je posledny element aktivny tak zrus aktivitu
		if (list->lastElement == list->activeElement)
			list->activeElement = NULL;

		// ak je posledny element aj prvy tak zrus aj prvy
		if (list->firstElement == list->lastElement){
			list->firstElement = NULL;
			list->lastElement = NULL;
		}
		else{
			list->lastElement = list->lastElement->previousElement;
			list->lastElement->nextElement = NULL;
		}
		free(tmp);
	}

}

/**
 * Zruší prvek seznamu list za aktivním prvkem.
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * posledním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteAfter( DLList *list ) {

	// ak nieje aktivny alebo posledny prvok je aktivny tak skip
	if (list->activeElement != NULL || list->lastElement != list->activeElement){
		DLLElementPtr deleted_element = list->activeElement->nextElement;
		
		// ak je element za aktivnym posledny tak nastav aktivny ako posledny
		if (deleted_element == list->lastElement){
			list->activeElement->nextElement = NULL;
			list->lastElement = list->activeElement;
		}
		else{
			list->activeElement->nextElement = deleted_element->nextElement;
			deleted_element->nextElement->previousElement = list->activeElement;
		}
		free(deleted_element);
	}
}

/**
 * Zruší prvek před aktivním prvkem seznamu list .
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * prvním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteBefore( DLList *list ) {
	// ak nieje aktivny alebo prvy prvok je aktivny tak skip
	if (list->activeElement != NULL || list->activeElement != list->firstElement){

		DLLElementPtr deleted_element = list->activeElement->previousElement;
		// ak je element pred aktivnym prvy tak nastav aktivny ako prvy
		if (deleted_element == list->firstElement){
			list->activeElement->previousElement = NULL;
			list->firstElement = list->activeElement;
		}
		else{
			deleted_element->previousElement->nextElement = list->activeElement;
			list->activeElement->previousElement = deleted_element->previousElement;
		}
		free(deleted_element);
	}
}

/**
 * Vloží prvek za aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu za právě aktivní prvek
 */
void DLL_InsertAfter( DLList *list, int data ) {
	// pokial existuje aktivny elemnet
	if (list->activeElement != NULL){

		// alokuje miesto pre novy element
		DLLElementPtr new = malloc(sizeof(struct DLLElement));
		if (new == NULL){
			DLL_Error();
			return;
		}

		// priradi data a element pred nim
		new->data = data;
		new->previousElement = list->activeElement;

		// ak je aktivny posledny tak prirad novy ako posledny
		if (list->activeElement == list->lastElement){
			list->lastElement = new;
			new->nextElement = NULL;
		}
		else{
			new->nextElement = list->activeElement->nextElement;
		}

		list->activeElement->nextElement = new;
		
	}
}

/**
 * Vloží prvek před aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu před právě aktivní prvek
 */
void DLL_InsertBefore( DLList *list, int data ) {
	// pokial existuje aktivny elemnet
	if (list->activeElement != NULL){

		// alokuje miesto pre novy element
		DLLElementPtr new = malloc(sizeof(struct DLLElement));
		if (new == NULL){
			DLL_Error();
			return;
		}

		// priradi data a element za nim
		new->data = data;
		new->nextElement = list->activeElement;

		// ak je aktivny prvy tak prirad novy ako prvy
		if (list->activeElement == list->firstElement){
			list->firstElement = new;
			new->previousElement = NULL;
		}
		else{
			new->previousElement = list->activeElement->previousElement;
		}

		list->activeElement->previousElement = new;
		
	}
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, volá funkci DLL_Error ().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetValue( DLList *list, int *dataPtr ) {
	// ak list nema aktivny element tak error
	if (list->activeElement == NULL){
		DLL_Error();
	}
	else{
		*dataPtr = list->activeElement->data;
	}
}

/**
 * Přepíše obsah aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, nedělá nic.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Nová hodnota právě aktivního prvku
 */
void DLL_SetValue( DLList *list, int data ) {
	if (list->activeElement != NULL){
		list->activeElement->data = data;
	}
}

/**
 * Posune aktivitu na následující prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Next( DLList *list ) {
	if (list->activeElement != NULL){
		list->activeElement = list->activeElement->nextElement;
	}
}


/**
 * Posune aktivitu na předchozí prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Previous( DLList *list ) {
	if (list->activeElement != NULL){
		list->activeElement = list->activeElement->previousElement;
	}
}

/**
 * Je-li seznam list aktivní, vrací nenulovou hodnotu, jinak vrací 0.
 * Funkci je vhodné implementovat jedním příkazem return.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 *
 * @returns Nenulovou hodnotu v případě aktivity prvku seznamu, jinak nulu
 */
int DLL_IsActive( DLList *list ) {
	return (list->activeElement == NULL) ? 0 : 1;
}

/* Konec c206.c */
