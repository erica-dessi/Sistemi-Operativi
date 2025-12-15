/**
 * @file code.c
 * @brief Implementazione delle funzioni per la gestione delle code di messaggi.
 *
 * Questo file contiene le definizioni delle funzioni per creare, inviare, leggere e rimuovere messaggi
 * da code di messaggi utilizzando l'IPC System V.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/msg.h>
#include <unistd.h>
#include "code.h"

#define DEFAULT_TYPE 1
/**
 * @struct mymsg
 * @brief Struttura per i messaggi nella coda.
 *
 * Struttura utilizzata per rappresentare un messaggio all'interno della coda, con un tipo (`mtype`) e un contenuto (`mtext`).
 */
struct mymsg
{
    long mtype;
    int mtext;
};

/**
 * @brief Crea una nuova coda di messaggi.
 *
 * Genera una coda di messaggi utilizzando il percorso specificato per ottenere una chiave IPC.
 *
 * @param pathname Percorso utilizzato per ottenere la chiave della coda di messaggi
 * @return L'identificatore della coda in caso di successo, termina il programma in caso di errore
 */
int create_queue(char *pathname)
{
    key_t key = ftok(pathname, 'x');
    int id = msgget(key, IPC_CREAT | 0666);
    if (id == -1)
    {
        perror("msgget error");
        exit(EXIT_FAILURE);
    }
    return id;
}

/**
 * @brief Legge un messaggio dalla coda con il tipo di default.
 *
 * Legge un messaggio dalla coda di messaggi identificata da `id` con tipo DEFAULT_TYPE.
 *
 * @param id Identificatore della coda di messaggi
 * @return Il contenuto del messaggio letto in caso di successo, termina il programma in caso di errore
 */
int read_message(int id)
{
    struct mymsg messaggio;
    if (msgrcv(id, &messaggio, sizeof(messaggio.mtext), DEFAULT_TYPE, 0) == -1)
    {
        perror("msgrcv error");
        exit(EXIT_FAILURE);
    }
    return messaggio.mtext;
}

/**
 * @brief Invia un messaggio alla coda con il tipo di default.
 *
 * Invia un messaggio con tipo DEFAULT_TYPE alla coda identificata da `id`.
 *
 * @param id Identificatore della coda di messaggi
 * @param msg Messaggio da inviare
 * @return 0 in caso di successo, termina il programma in caso di errore
 */
int send_message(int id, int msg)
{
    struct mymsg messaggio;
    messaggio.mtype = DEFAULT_TYPE;
    messaggio.mtext = msg;
    if (msgsnd(id, &messaggio, sizeof(messaggio.mtext), 0) < 0)
    {
        perror("msgsend error");
        exit(EXIT_FAILURE);
    }
    return 0;
}

/**
 * @brief Invia un messaggio di un tipo specifico alla coda.
 *
 * Invia un messaggio con tipo `tipo` alla coda identificata da `id`.
 *
 * @param id Identificatore della coda di messaggi
 * @param msg Messaggio da inviare
 * @param tipo Tipo di messaggio
 * @return 0 in caso di successo, termina il programma in caso di errore
 */
int send_type_message(int id, int msg, int tipo)
{
    struct mymsg messaggio;
    messaggio.mtype = tipo;
    messaggio.mtext = msg;
    if (msgsnd(id, &messaggio, sizeof(messaggio.mtext), 0) < 0)
    {
        perror("msgsend error");
        exit(EXIT_FAILURE);
    }
    return 0;
}

/**
 * @brief Legge un messaggio di un tipo specifico dalla coda.
 *
 * Legge il primo messaggio di tipo `tipo` dalla coda identificata da `id`.
 *
 * @param id Identificatore della coda di messaggi
 * @param tipo Tipo di messaggio da leggere
 * @return Il contenuto del messaggio letto in caso di successo, termina il programma in caso di errore
 */
int read_type_message(int id, int tipo)
{
    struct mymsg messaggio;
    if (msgrcv(id, &messaggio, sizeof(messaggio.mtext), tipo, 0) == -1)
    {
        perror("msgrcv error");
        exit(EXIT_FAILURE);
    }
    return messaggio.mtext;
}

/**
 * @brief Legge un messaggio di un tipo specifico dalla coda in modalità non bloccante.
 *
 * Legge un messaggio di tipo `tipo` dalla coda identificata da `id` in modalità non bloccante.
 *
 * @param id Identificatore della coda di messaggi
 * @param tipo Tipo di messaggio da leggere
 * @return Il contenuto del messaggio letto in caso di successo, 0 se non ci sono messaggi disponibili
 *         oppure termina il programma in caso di errore
 */
int read_type_message_nb(int id, int tipo)
{
    struct mymsg messaggio;
    if (msgrcv(id, &messaggio, sizeof(messaggio.mtext), tipo, IPC_NOWAIT) == -1)
    {
        if (errno == ENOMSG)
        {
            return 0;
        }
        if (errno != ENOMSG)
        {
            perror("msgrcv error");
            exit(EXIT_FAILURE);
        }
    }
    return messaggio.mtext;
}

/**
 * @brief Rimuove la coda di messaggi.
 *
 * Elimina la coda di messaggi identificata da `id`.
 *
 * @param id Identificatore della coda di messaggi
 * @return 0 in caso di successo, termina il programma in caso di errore
 */
int remove_queue(int id)
{
    if (msgctl(id, IPC_RMID, NULL) == -1)
    {
        perror("msgctl error");
        exit(EXIT_FAILURE);
    }

    return 0;
}

int somma_messaggi_di_tipo(int queue_id, long tipo)
{
    struct mymsg msg;
    int somma = 0;

    while (1)
    {
        // Prova a ricevere un messaggio del tipo specificato senza rimuoverlo se non è valido
        ssize_t ret = msgrcv(queue_id, &msg, sizeof(msg.mtext), tipo, IPC_NOWAIT);
        if (ret == -1)
        {
            if (errno == ENOMSG)
            {
                // Nessun altro messaggio di questo tipo presente
                break;
            }
            else
            {
                perror("Errore nella lettura del messaggio");
                exit(EXIT_FAILURE);
            }
        }

        // Converti il messaggio ricevuto in un intero e aggiungilo alla somma
        int valore = msg.mtext;
        somma += valore;
    }

    return somma;
}
