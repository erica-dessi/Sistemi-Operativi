#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include "../lib/handler.h"
#include "../lib/code.h"
#include "../lib/semaphore.h"
#include "../lib/shared_memory.h"
#include "../lib/conf.h"

// DICHIARAZIONE DI FUNZIONI

/**
 * @brief Calcola il massimo tra due numeri.
 *
 * @param a Primo numero
 * @param b Secondo numero
 * @return Il maggiore tra i due numeri
 */
int max(int a, int b);

/**
 * @brief Calcola l'energia generata dalla scissione dell'atomo.
 *
 * L'energia è calcolata in base al numero atomico dell'atomo e del suo figlio.
 *
 * @param n_atomico_figlio Numero atomico del nuovo atomo generato dalla scissione
 * @return Energia generata dalla scissione
 */
int energy(int n_atomico_figlio);

/**
 * @brief Esegue la scissione dell'atomo.
 *
 * La scissione genera un nuovo atomo con un numero atomico inferiore e calcola
 * l'energia rilasciata. Viene poi creato un nuovo processo per gestire il nuovo atomo.
 *
 * @return Energia generata dalla scissione
 */
int scissione();

/**
 * @brief Calcola un numero atomico casuale.
 *
 * Questa funzione calcola un numero atomico casuale
 * compreso tra 1 e n_atomico_max (escluso).
 *
 * @param n_atomico_max Il numero atomico massimo.
 * @return Un numero atomico casuale.
 */
int calcolo_numero_atomico(int n_atomico_max);

/**
 * @brief Crea un nuovo atomo e avvia un processo.
 *
 * Questa funzione crea un nuovo atomo con un numero atomico specificato
 * e avvia un nuovo processo eseguendo il programma associato all'atomo.
 *
 * @param n_atomico Il numero atomico del nuovo atomo.
 * @return Il PID del processo creato.
 */
int new_atomo(int n_atomico);

/**
 * @brief gestore segnale di terminazione, imposta a 0 la flag "simulazione in corso"
 */
void term_handler(int signum);