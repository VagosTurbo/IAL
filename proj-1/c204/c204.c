/* ******************************* c204.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c204 - Převod infixového výrazu na postfixový (s využitím c202)     */
/*  Referenční implementace: Petr Přikryl, listopad 1994                      */
/*  Přepis do jazyka C: Lukáš Maršík, prosinec 2012                           */
/*  Upravil: Kamil Jeřábek, září 2019                                         */
/*           Daniel Dolejška, září 2021                                       */
/* ************************************************************************** */
/*
** Implementujte proceduru pro převod infixového zápisu matematického výrazu
** do postfixového tvaru. Pro převod využijte zásobník (Stack), který byl
** implementován v rámci příkladu c202. Bez správného vyřešení příkladu c202
** se o řešení tohoto příkladu nepokoušejte.
**
** Implementujte následující funkci:
**
**    infix2postfix ... konverzní funkce pro převod infixového výrazu
**                      na postfixový
**
** Pro lepší přehlednost kódu implementujte následující pomocné funkce:
**    
**    untilLeftPar ... vyprázdnění zásobníku až po levou závorku
**    doOperation .... zpracování operátoru konvertovaného výrazu
**
** Své řešení účelně komentujte.
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c204.h"

bool solved;

/**
 * Pomocná funkce untilLeftPar.
 * Slouží k vyprázdnění zásobníku až po levou závorku, přičemž levá závorka bude
 * také odstraněna.
 * Pokud je zásobník prázdný, provádění funkce se ukončí.
 *
 * Operátory odstraňované ze zásobníku postupně vkládejte do výstupního pole
 * znaků postfixExpression.
 * Délka převedeného výrazu a též ukazatel na první volné místo, na které se má
 * zapisovat, představuje parametr postfixExpressionLength.
 *
 * Aby se minimalizoval počet přístupů ke struktuře zásobníku, můžete zde
 * nadeklarovat a používat pomocnou proměnnou typu char.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
void untilLeftPar( Stack *stack, char *postfixExpression, unsigned *postfixExpressionLength ) {

	// ak je zasobnik prazdny tak skip
	if (Stack_IsEmpty(stack)){
		return;
	}

	char top;
	Stack_Top(stack, &top);

	// zapisuje kym na vrchu nieje lava zatvorka
	if (top == '('){
		Stack_Pop(stack);
		return;
	}

	postfixExpression[(*postfixExpressionLength)++] = top;
	Stack_Pop(stack);
	untilLeftPar(stack, postfixExpression, postfixExpressionLength);
}

/**
 * Pomocná funkce doOperation.
 * Zpracuje operátor, který je předán parametrem c po načtení znaku ze
 * vstupního pole znaků.
 *
 * Dle priority předaného operátoru a případně priority operátoru na vrcholu
 * zásobníku rozhodneme o dalším postupu.
 * Délka převedeného výrazu a taktéž ukazatel na první volné místo, do kterého
 * se má zapisovat, představuje parametr postfixExpressionLength, výstupním
 * polem znaků je opět postfixExpression.
 *
 * @param stack Ukazatel na inicializovanou strukturu zásobníku
 * @param c Znak operátoru ve výrazu
 * @param postfixExpression Znakový řetězec obsahující výsledný postfixový výraz
 * @param postfixExpressionLength Ukazatel na aktuální délku výsledného postfixového výrazu
 */
void doOperation( Stack *stack, char c, char *postfixExpression, unsigned *postfixExpressionLength ) {
	// ak je zasobnik prazdny tak pushni operator
	if (Stack_IsEmpty(stack)){
		Stack_Push(stack, c);
		return;
	}

	char top;
	Stack_Top(stack, &top);

	// ak je na vrchu lava zatvorka tak pushne na stack
	if (top == '('){
		Stack_Push(stack, c);
		return;
	}

	// pushne na stack len ak je to operator s vyssiou prioritou ak no vrchu zasobniku
	if ((c == '*' || c == '/') && (top == '+' || top == '-')){
		Stack_Push(stack, c);
		return;
	}

	// zapis do postfix vrchol zasobniku a opakuj kym sa c nebude moct dat do zasobnika
	postfixExpression[(*postfixExpressionLength)++] = top;
	Stack_Pop(stack);
	doOperation(stack, c, postfixExpression, postfixExpressionLength);
}

/**
 * Konverzní funkce infix2postfix.
 * Čte infixový výraz ze vstupního řetězce infixExpression a generuje
 * odpovídající postfixový výraz do výstupního řetězce (postup převodu hledejte
 * v přednáškách nebo ve studijní opoře).
 * Paměť pro výstupní řetězec (o velikosti MAX_LEN) je třeba alokovat. Volající
 * funkce, tedy příjemce konvertovaného řetězce, zajistí korektní uvolnění zde
 * alokované paměti.
 *
 * Tvar výrazu:
 * 1. Výraz obsahuje operátory + - * / ve významu sčítání, odčítání,
 *    násobení a dělení. Sčítání má stejnou prioritu jako odčítání,
 *    násobení má stejnou prioritu jako dělení. Priorita násobení je
 *    větší než priorita sčítání. Všechny operátory jsou binární
 *    (neuvažujte unární mínus).
 *
 * 2. Hodnoty ve výrazu jsou reprezentovány jednoznakovými identifikátory
 *    a číslicemi - 0..9, a..z, A..Z (velikost písmen se rozlišuje).
 *
 * 3. Ve výrazu může být použit předem neurčený počet dvojic kulatých
 *    závorek. Uvažujte, že vstupní výraz je zapsán správně (neošetřujte
 *    chybné zadání výrazu).
 *
 * 4. Každý korektně zapsaný výraz (infixový i postfixový) musí být uzavřen
 *    ukončovacím znakem '='.
 *
 * 5. Při stejné prioritě operátorů se výraz vyhodnocuje zleva doprava.
 *
 * Poznámky k implementaci
 * -----------------------
 * Jako zásobník použijte zásobník znaků Stack implementovaný v příkladu c202.
 * Pro práci se zásobníkem pak používejte výhradně operace z jeho rozhraní.
 *
 * Při implementaci využijte pomocné funkce untilLeftPar a doOperation.
 *
 * Řetězcem (infixového a postfixového výrazu) je zde myšleno pole znaků typu
 * char, jenž je korektně ukončeno nulovým znakem dle zvyklostí jazyka C.
 *
 * Na vstupu očekávejte pouze korektně zapsané a ukončené výrazy. Jejich délka
 * nepřesáhne MAX_LEN-1 (MAX_LEN i s nulovým znakem) a tedy i výsledný výraz
 * by se měl vejít do alokovaného pole. Po alokaci dynamické paměti si vždycky
 * ověřte, že se alokace skutečně zdrařila. V případě chyby alokace vraťte namísto
 * řetězce konstantu NULL.
 *
 * @param infixExpression vstupní znakový řetězec obsahující infixový výraz k převedení
 *
 * @returns znakový řetězec obsahující výsledný postfixový výraz
 */
char *infix2postfix( const char *infixExpression ) {
	Stack stack;
	Stack_Init(&stack);

	// alokuje pamat pre postfixovy vyraz
	char *postfixExpression = malloc(MAX_LEN * sizeof(char));
	if (postfixExpression == NULL){
		Stack_Dispose(&stack);
		return NULL;
	}
	
	unsigned postfixExpressionLength = 0;
	int infix_index = 0;
	char c = infixExpression[infix_index];
	while (c != '\0'){
		// ak je operand, pushni ho do vysledneho vyrazu
		if ((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')){
			postfixExpression[postfixExpressionLength++] = c;
		}
		// ak je operator, zpracuj ho
		else if (c == '+' || c == '-' || c == '*' || c == '/'){
			doOperation(&stack, c, postfixExpression, &postfixExpressionLength);
		}
		// ak je lava zatvorka, pushni ju na stack
		else if (c == '('){
			Stack_Push(&stack, c);
		}
		// ak je prava zatvorka, vyprazdni stack az po lavu zatvorku
		else if (c == ')'){
			untilLeftPar(&stack, postfixExpression, &postfixExpressionLength);
		}
		c = infixExpression[++infix_index];
	}
	// vyprazdni stack
	while (!Stack_IsEmpty(&stack)){
		Stack_Top(&stack, &c);
		Stack_Pop(&stack);
		postfixExpression[postfixExpressionLength++] = c;
	}
	// prida na koniec '=' a '\0'
	postfixExpression[postfixExpressionLength++] = '=';
	postfixExpression[postfixExpressionLength] = '\0';

	// uvolni stack
	Stack_Dispose(&stack);
	return postfixExpression;
	
}


/**
 * Pomocná metoda pro vložení celočíselné hodnoty na zásobník.
 *
 * Použitá implementace zásobníku aktuálně umožňuje vkládání pouze
 * hodnot o velikosti jednoho byte (char). Využijte této metody
 * k rozdělení a postupné vložení celočíselné (čtyřbytové) hodnoty
 * na vrchol poskytnutého zásobníku.
 *
 * @param stack ukazatel na inicializovanou strukturu zásobníku
 * @param value hodnota k vložení na zásobník
 */
void expr_value_push( Stack *stack, int value ) {
	// rozdeli hodnotu na 4 byty
	char byte1 = (char)(value & 0xFF);
    char byte2 = (char)((value >> 8) & 0xFF);
    char byte3 = (char)((value >> 16) & 0xFF);
    char byte4 = (char)((value >> 24) & 0xFF);

    // vlozi jednotlive byty na zasobnik
    Stack_Push(stack, byte1);
    Stack_Push(stack, byte2);
    Stack_Push(stack, byte3);
    Stack_Push(stack, byte4);
}

/**
 * Pomocná metoda pro extrakci celočíselné hodnoty ze zásobníku.
 *
 * Využijte této metody k opětovnému načtení a složení celočíselné
 * hodnoty z aktuálního vrcholu poskytnutého zásobníku. Implementujte
 * tedy algoritmus opačný k algoritmu použitému v metodě
 * `expr_value_push`.
 *
 * @param stack ukazatel na inicializovanou strukturu zásobníku
 * @param value ukazatel na celočíselnou proměnnou pro uložení
 *   výsledné celočíselné hodnoty z vrcholu zásobníku
 */
void expr_value_pop( Stack *stack, int *value ) {
	char byte1, byte2, byte3, byte4;

	// extrahuje jednotlive byty zo zasobniku
    Stack_Top(stack, &byte4);
    Stack_Pop(stack);
    Stack_Top(stack, &byte3);
    Stack_Pop(stack);
    Stack_Top(stack, &byte2);
    Stack_Pop(stack);
    Stack_Top(stack, &byte1);
    Stack_Pop(stack);

    // zlozi jednotlive byty do jednej hodnoty
    *value = ((int)byte1 & 0xFF) |
             (((int)byte2 & 0xFF) << 8) |
             (((int)byte3 & 0xFF) << 16) |
             (((int)byte4 & 0xFF) << 24);
}


/**
 * Tato metoda provede vyhodnocení výrazu zadaném v `infixExpression`,
 * kde hodnoty proměnných použitých v daném výrazu jsou definovány
 * v poli `variableValues`.
 *
 * K vyhodnocení vstupního výrazu využijte implementaci zásobníku
 * ze cvičení c202. Dále také využijte pomocných funkcí `expr_value_push`,
 * respektive `expr_value_pop`. Při řešení si můžete definovat libovolné
 * množství vlastních pomocných funkcí.
 *
 * Předpokládejte, že hodnoty budou vždy definovány
 * pro všechy proměnné použité ve vstupním výrazu.
 *
 * @param infixExpression vstpní infixový výraz s proměnnými
 * @param variableValues hodnoty proměnných ze vstupního výrazu
 * @param variableValueCount počet hodnot (unikátních proměnných
 *   ve vstupním výrazu)
 * @param value ukazatel na celočíselnou proměnnou pro uložení
 *   výsledné hodnoty vyhodnocení vstupního výrazu
 *
 * @return výsledek vyhodnocení daného výrazu na základě poskytnutých hodnot proměnných
 */
bool eval( const char *infixExpression, VariableValue variableValues[], int variableValueCount, int *value ) {
    Stack stack;
    Stack_Init(&stack);

    // prevedie infixovy vyraz na postfixovy
    char *postfixExpression = infix2postfix(infixExpression);
    if (postfixExpression == NULL) {
		Stack_Dispose(&stack);
        return false;
    }

	int postfix_index = 0;
	// postupne prejde cely postfixovy vyraz
	while (postfixExpression[postfix_index] != '='){
		char c = postfixExpression[postfix_index];
		if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')){
			// ak je operand, pushni jeho hodnotu na stack
			for (int i = 0; i < variableValueCount; i++){
				if (c == variableValues[i].name)
					expr_value_push(&stack, variableValues[i].value);
			}
		}
		else if (c == '+' || c == '-' || c == '*' || c == '/'){
			// popne 2 operandy zo stacku
			int num1, num2;
			expr_value_pop(&stack, &num2);
			expr_value_pop(&stack, &num1);
			int operation_result;

			// vyhodnoti operator
			if ( c == '+')
				operation_result = num1 + num2;
			else if (c == '-')
				operation_result = num1 - num2;
			else if (c == '*')
				operation_result = num1 * num2;
			else if (c == '/'){
				// osetrenie delenia nulou
				if (num2 == 0){
					Stack_Dispose(&stack);
					free(postfixExpression);
					return false;
				}
				operation_result = num1 / num2;
			}
			// pushne na stack vysledok
			expr_value_push(&stack, operation_result);
		}
		postfix_index++;
	}
	// popne z vrcholu zasobniku finalny vysledok a uvolni pamat
	expr_value_pop(&stack, value);
	Stack_Dispose(&stack);
	free(postfixExpression);
	return true;

}

/* Konec c204.c */
