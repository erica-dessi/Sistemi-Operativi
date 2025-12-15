/**
 * @file handler.c
 * @brief Implementazione delle funzioni per la gestione dei segnali.
 *
 * Questo file contiene le implementazioni delle funzioni che consentono di impostare un gestore personalizzato,
 * ignorare o resettare i gestori dei segnali in un'applicazione.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include "handler.h"

/**
 * @brief Imposta un gestore personalizzato per un segnale specifico.
 *
 * Questa funzione configura un gestore personalizzato per il segnale specificato, utilizzando `sigaction`.
 *
 * @param handler Puntatore alla funzione che gestirà il segnale
 * @param signum Numero del segnale da gestire
 * @return 0 se l'operazione ha successo, -1 in caso di errore
 */
int set_handler(void (*handler)(int signum), int signum)
{
    struct sigaction sa;
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(signum, &sa, NULL) == -1)
    {
        perror("sigaction error");
        return -1;
    }
    return 0;
}

/**
 * @brief Ignora un segnale specifico.
 *
 * Configura il programma per ignorare il segnale indicato utilizzando `sigaction`.
 *
 * @param signum Numero del segnale da ignorare
 * @return 0 se l'operazione ha successo, -1 in caso di errore
 */
int ignore(int signum)
{
    struct sigaction sa;
    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(signum, &sa, NULL) == -1)
    {
        perror("sigaction error");
        return -1;
    }
    return 0;
}

/**
 * @brief Ripristina il gestore predefinito di un segnale.
 *
 * Questa funzione ripristina il comportamento di gestione del segnale alle impostazioni predefinite utilizzando `sigaction`.
 *
 * @param signum Numero del segnale da resettare
 * @return 0 se l'operazione ha successo, -1 in caso di errore
 */
int reset_handler(int signum)
{
    struct sigaction sa;
    sa.sa_handler = SIG_DFL;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    if (sigaction(signum, &sa, NULL) == -1)
    {
        perror("sigaction error");
        return -1;
    }
    return 0;
}
