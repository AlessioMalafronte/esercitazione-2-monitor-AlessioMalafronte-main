#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "header.h"


void Utente(MonitorSchedulatore * s) {

	int i;

	for(i=0; i<TOTALE_RICHIESTE; i++) {

		/*
		  Creazione e inserimento di una richiesta
		*/

		richiesta r;

		r.posizione = rand() % TOTALE_POSIZIONI;
		r.processo = getpid();

		InserisciRichiesta(s, &r);

		printf("Utente %d ha inserito una richiesta alla posizione %d\n", r.processo, r.posizione);
	}
}


void Schedulatore(MonitorSchedulatore * s) {

	int i;

	/*
	  Variabile locale contenente la posizione dell'ultima richiesta
	  che è stata servita (inizialmente si assume la posizione 0)
	*/

	int posizione_corrente = 0;



	/*
	  Array locale che rappresenta il disco. Ogni locazione del
	  disco ospiterà il PID del processo che ha richiesto l'ultima
	  operazione in quella posizione
	*/

	pid_t disco[TOTALE_POSIZIONI];



	for(i=0; i<TOTALE_RICHIESTE*TOTALE_UTENTI; i++) {

		richiesta r;



		/*
		  Lo Schedulatore preleva una richiesta, che verrà copiata
		  nella variabile locale "r"
		*/

		printf("Schedulatore in attesa di richieste...\n");

		PrelevaRichiesta(s, &r);

		printf("Schedulatore ha ricevuto una richiesta, attende %d secondi...\n", abs((int)r.posizione - posizione_corrente));



		/*
		  Lo Schedulatore attende alcuni secondi (in base alla posizione
		  della richiesta), ed aggiorna la posizione del disco con il
		  PID del processo richiedente
		*/

		sleep(abs((int)r.posizione - posizione_corrente));

		posizione_corrente = r.posizione;

		disco[posizione_corrente] = r.processo;



		printf("Disco aggiornato alla posizione %d, nuovo valore %d\n", r.posizione, r.processo);

	}
}


void InizializzaMonitor(MonitorSchedulatore * s) {

	/* TBD: Inizializzare la struttura dati */

	s->testa = 0;
	s->coda = 0;

	init_monitor(&s->m, 2);
}


void RimuoviMonitor(MonitorSchedulatore * s) {

	/* TBD: Deallocare il sotto-oggetto monitor */

	remove_monitor(&(s->m));
}


void InserisciRichiesta(MonitorSchedulatore * s, richiesta * r) {

	/* TBD: Effettuare l'ingresso nel monitor */


	/*
	  TBD: Verifica se la coda di richieste sia piena.
	  In caso affermativo, il processo è posto in attesa.
	*/

	enter_monitor(&(s->m));

	if((s->testa + 1) % DIMENSIONE_CODA == s->coda){

		wait_condition(&(s->m), CV_P);
	}



	printf("Utente %d sta inserendo una richiesta\n", getpid());

	s->coda_richieste[s->testa].posizione = r->posizione;
	s->coda_richieste[s->testa].processo = r->processo;

	s->testa = (s->testa + 1) % DIMENSIONE_CODA;



	/* TBD: Effettuare signal_condition() */

	signal_condition(&(s->m), CV_C);

	/* TBD: Uscire dal monitor */

	leave_monitor(&(s->m));
}


void PrelevaRichiesta(MonitorSchedulatore * s, richiesta * r) {

	/* TBD: Effettuare l'ingresso nel monitor */
	enter_monitor(&(s->m));


	/*
	  TBD: Verifica se la coda di richieste sia vuota.
	  In caso affermativo, il processo è posto in attesa.
	*/

	if(s->testa == s->coda){
		wait_condition(&(s->m), CV_C);
	}


	printf("Processo schedulatore sta prelevando una richiesta\n");

	r->posizione = s->coda_richieste[s->coda].posizione;
	r->processo = s->coda_richieste[s->coda].processo;

	s->coda = (s->coda + 1) % DIMENSIONE_CODA;


	/* TBD: Effettuare signal_condition() */

	signal_condition(&(s->m), CV_P);

	/* TBD: Uscire dal monitor */

	leave_monitor(&(s->m));
	
}

 
