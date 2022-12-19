#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "header.h"

void InizioLettura(MonitorMeteo* p){

	/* TBD: Effettuare inizio lettura */
	enter_monitor(&(p->m));

	if(p->lettori > 0){
		wait_condition(&(p->m), CV_LETT);
	}

	p->lettori = p->lettori + 1;

	signal_condition(&(p->m), CV_LETT);

	leave_monitor(&(p->m));
}

void FineLettura(MonitorMeteo* p) {

	/* TBD: Effettuare fine lettura */
	enter_monitor(&(p->m));

	p->lettori = p->lettori - 1;

	if(p->lettori == 0){

		signal_condition(&(p->m), CV_SCRITT);

	}

	leave_monitor(&(p->m));

}



//Procedure di inizio e fine scrittura

void InizioScrittura(MonitorMeteo* p){

	/* TBD: Effettuare inizio scrittura */
	enter_monitor(&(p->m));

	if(p->lettori > 0 || p->scrittori > 0){
		wait_condition(&(p->m), CV_SCRITT);
	}

	p->scrittori = p->scrittori + 1;


	leave_monitor(&(p->m));
}

void FineScrittura(MonitorMeteo* p){

	/* TBD: Effettuare fine scrittura */
	
	enter_monitor(&(p->m));

	if (queue_condition(&(p->m), CV_SCRITT) > 0){ 

		signal_condition(&(p->m), CV_SCRITT);

	} else if (queue_condition(&(p->m), CV_LETT) > 0){ 

		signal_condition(&(p->m), CV_LETT);

	}

	leave_monitor(&(p->m));
}



void Servizio(MonitorMeteo* p){

	srand(time(0));

	int i;
	for(i=0; i<20; i++) {

		/* TBD: Richiamare InizioScrittura e FineScrittura */

		InizioLettura(p);

		p->meteo.temperatura = (rand()%101)-50;
		p->meteo.umidita = rand()%101;
		p->meteo.pioggia = rand()%2;

		printf("<%d> scrittura: Temperatura=%d, Umidità=%d, Pioggia=%s\n", getpid(), p->meteo.temperatura, p->meteo.umidita, (p->meteo.pioggia ? "si" : "no") );

		FineScrittura(p);

		sleep(2);
	}

}

void Utente(MonitorMeteo* p) {

	int i;
	for(i=0; i<10; i++) {

		/* TBD: Richiamare InizioLettura e FineLettura */

		InizioLettura(p);

		printf("<%d> lettura: Temperatura=%d, Umidità=%d, Pioggia=%s\n", getpid(), p->meteo.temperatura, p->meteo.umidita, (p->meteo.pioggia ? "si" : "no") );

		FineLettura(p);

		sleep(1);
	}

}


