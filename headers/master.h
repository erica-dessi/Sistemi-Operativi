#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include "../lib/handler.h"
#include "../lib/code.h"
#include "../lib/semaphore.h"
#include "../lib/shared_memory.h"
#include "../lib/conf.h"

/**
 * @brief Lancia un eseguibile in un processo figlio.
 *
 * @param pathname Il percorso dell'eseguibile da eseguire.
 * @return Il PID del processo figlio.
 */
int start(char *pathname);

/**
 * @brief Gestisce il timer e la stampa della simulazione tramite il segnale SIGALRM.
 *
 * @param signum Il numero del segnale ricevuto.
 */
void alarm_handler(int signum);

void aggiorna_simulazione();

/**
 * @brief Stampa lo stato attuale della simulazione, inclusi il tempo rimanente,
 *        il numero di atomi attivi e l'energia totale accumulata.
 */
void stato_simulazione();

int new_atomo(int n_atomico);

void inibitore_handler(int signum);
