/*IMPLEMENTAZIONE DELLE PROCEDURE*/

#include "procedure.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

/* TBD: Definire delle macro per identificare le variabili condition */
#define NOT_FULL1 0
#define NOT_FULL2 1
#define NOT_EMPTY 2
#define NUM_CV 3

void inizializza_prod_cons(PriorityProdCons* p){

	/* TBD: Inizializzare le variabili della struttura (testa, coda, ...) */

	p->testa = 0;
	p->coda = 0;
	p->count = 0;
	p->num_produttori_alta_prio;


	/* TBD: Inizializzare il sotto-oggetto "Monitor", chiamando init_monitor(...).
	        Scegliere accuratamente il numero di variabili condition */

	init_monitor(&(p->m), NUM_CV);
}

void rimuovi_prod_cons(PriorityProdCons* p){

	/* TBD: Deallocare il sotto-oggetto "Monitor", chiamando remove_Monitor(...). */
	remove_monitor(&(p->m));
}


void produci_alta_prio(PriorityProdCons* p){

	int value;

	/* TBD: Effettuare l'ingresso nel monitor */

	enter_monitor((&(p->m)));

	//Aumenta il numero di produttori
	printf("Produttore 1 entrato nel monitor...\n");

	/* TBD: Sospendere qui il processo se il vettore di buffer è pieno */

	while(p->count == DIM){
		p->num_produttori_alta_prio++;
		wait_condition(&(p->m), NOT_FULL1);
		p->num_produttori_alta_prio--;
	}
	

	//Produco il valore
	value = rand() % 12 ;

	//Modifico il buffer
	p->buffer[p->testa] = value;
	p->testa = (p->testa + 1) % DIM;
	p->count++;

	//Il buffer non è vuoto
	printf("Produttore 1 con pid %d ha prodotto %d\n",getpid(),value);

	/* TBD: Effettuare la signal_condition per i consumatori in attesa */

	signal_condition(&(p->m), NOT_EMPTY);

	/* TBD: Uscire dal monitor */
	leave_monitor(&(p->m));
}

void produci_bassa_prio(PriorityProdCons* p){

	int value;

	/* TBD: Effettuare l'ingresso nel monitor */
	enter_monitor(&(p->m));

	//Aumenta il numero di produttori
	printf("Produttore 2 entrato nel monitor...\n");

	/* TBD: Sospendere qui il processo nei seguenti due possibili casi:
	 *      1) Il vettore di buffer è pieno; oppure
	 *      2) Un produttore ad alta priorità è già sospeso in attesa.
	 *         Si suggerisce di usare queue_condition() oppure una variabile
	 *         intera "num_lettori_alta_priorita", vedi suggerimenti nel
	 *         commento della funzione consuma().
	 */
	while(p->count == DIM || p->num_produttori_alta_prio > 0){
		wait_condition(&(p->m),NOT_FULL2);
	}

	//Produco il valore
	value = 13 + (rand() % 12) ;

	//Modifico il buffer
	p->buffer[p->testa] = value;
	p->testa = (p->testa + 1) % DIM;
	p->count++;

	//Il buffer non è vuoto
	printf("Produttore 2 con pid %d ha prodotto %d\n",getpid(),value);

	/* TBD: Effettuare la signal_condition per i consumatori in attesa */

	signal_condition(&(p->m), NOT_EMPTY);
	/* TBD: Uscire dal monitor */
	leave_monitor(&(p->m));
}

void consuma(PriorityProdCons* p){

	int value;

	/* TBD: Effettuare l'ingresso nel monitor */
	
	enter_monitor(&(p->m));

	/* TBD: Sospendere qui il processo se il vettore di buffer è vuoto */

	while(p->count == 0){
		wait_condition(&(p->m), NOT_EMPTY);
	}

	value = p->buffer[p->coda];
	p->coda = (p->coda + 1) % DIM;
	p->count--;

	printf("Consumatore con pid %d ha consumato valore %d\n",getpid(),value);


	/* TBD: Effettuare la signal_condition per attivare un produttore.
	 *      Si attivi un produttore di priorità alta se presente, altrimenti,
	 *      si attivi un produttore di priorità bassa.
	 *
	 * 		Per determinare se ci sono produttori ad alta priorità in attesa,
	 * 		si introduca una variabile "num_lettori_alta_priorita", che i produttori
	 *      incrementano subito prima di fare wait_condition(), e che decrementano
	 * 		subito dopo aver eseguito wait_condition().
	 *
	 * 		In alternativa, utilizzare la funzione "queue_condition()".
	 */
	if( p->num_produttori_alta_prio > 0){

		signal_condition(&p->m,NOT_FULL_1);

		}
	else{

		signal_condition(&p->m,NOT_FULL_2);

	}

	/* TBD: Uscire dal monitor */
	leave_monitor(&(p->m));
}
