#include "monitor_hoare.h"

#define NUM_UTENTI 10
#define CV_LETT 0
#define CV_SCRITT 1

typedef struct {
	
	int temperatura;
	unsigned int umidita;
	unsigned short pioggia;

} meteo;

typedef struct {
	meteo meteo;

	Monitor m;
	int lettori;
	int scrittori;

	/* TBD: Aggiungere un sotto-oggetto Monitor e altre variabili di stato
	 * per la sincronizzazione */

} MonitorMeteo;


void Servizio(MonitorMeteo* p);
void Utente(MonitorMeteo* p);
