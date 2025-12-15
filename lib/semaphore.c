/**
 * @file semaphore.c
 * @brief Implementazione delle funzioni per la gestione dei semafori.
 *
 * Questo file contiene le funzioni per creare, incrementare, decrementare e rimuovere semafori,
 * e per ottenere e impostare i valori dei semafori, utilizzando le librerie di sistema IPC per la gestione
 * della sincronizzazione dei processi.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include "semaphore.h"

/**
 * @union semun
 * @brief Unione utilizzata per i controlli del semaforo tramite `semctl`.
 *
 * Contiene i campi necessari per impostare e ottenere valori, buffer di stato e array di configurazione.
 */
union semun
{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
    struct seminfo *__buf;
};

/**
 * @brief Crea un nuovo semaforo.
 *
 * Genera una chiave unica con `ftok` utilizzando il percorso specificato, quindi crea
 * un semaforo con `semget`. Inizializza il valore del semaforo a 0.
 * Se la creazione fallisce, stampa un errore e termina il programma.
 *
 * @param pathname Percorso per generare la chiave unica del semaforo
 * @return Identificatore del semaforo creato (int)
 */
int create_sem(char *pathname)
{
    int semid;
    union semun arg;
    arg.val = 0;
    key_t key = ftok(pathname, 'x');

    semid = semget(key, 1, 0666);

    if (semid == -1)
    {
        semid = semget(key, 1, IPC_CREAT | 0666);

        if (semid == -1)
        {
            perror("Error creating the semaphore");
            exit(1);
        }

        if (semctl(semid, 0, SETVAL, arg) == -1)
        {
            perror("Error initializing the semaphore");
            exit(1);
        }
    }
    return semid;
}

/**
 * @brief Incrementa il valore del semaforo di 1.
 *
 * Esegue un'operazione di incremento (`V operation`) sul semaforo specificato.
 *
 * @param semid Identificatore del semaforo da incrementare
 * @return 0 se l'operazione ha successo, -1 in caso di errore
 */
int increase_sem(int semid)
{
    struct sembuf sem;
    sem.sem_num = 0;
    sem.sem_op = 1;
    sem.sem_flg = 0;

    if (semop(semid, &sem, 1) == -1)
    {
        if (errno == EINTR)
        {
            // Signal was received, but SA_RESTART will restart the system call
            return decrease_sem(semid);
        }
        else
        {
            perror("Error during increase operation on semaphore");
            return -1;
        }
    }
    return 0;
}

/**
 * @brief Decrementa il valore del semaforo di 1.
 *
 * Esegue un'operazione di decremento (`P operation`) sul semaforo specificato.
 *
 * @param semid Identificatore del semaforo da decrementare
 * @return 0 se l'operazione ha successo, -1 in caso di errore
 */
int decrease_sem(int semid)
{
    struct sembuf sem;
    sem.sem_num = 0;
    sem.sem_op = -1;
    sem.sem_flg = 0;

    if (semop(semid, &sem, 1) == -1)
    {
        if (errno == EINTR)
        {
            // Signal was received, but SA_RESTART will restart the system call
            return decrease_sem(semid);
        }
        else
        {
            perror("Error during decrease operation on semaphore");
            return -1;
        }
    }
    return 0;
}

/**
 * @brief Rimuove un semaforo.
 *
 * Rimuove il semaforo specificato dall'identificatore usando `semctl` con l'opzione `IPC_RMID`.
 *
 * @param semid Identificatore del semaforo da rimuovere
 * @return 0 se l'operazione ha successo
 */
int remove_sem(int semid)
{
    if (semctl(semid, 0, IPC_RMID) == -1)
    {
        perror("Error removing semaphore");
        exit(1);
    }

    return 0;
}

/**
 * @brief Attende che il semaforo raggiunga il valore zero.
 *
 * Blocca l'esecuzione del processo fino a quando il valore del semaforo è uguale a zero.
 *
 * @param semid Identificatore del semaforo
 * @return 0 se l'operazione ha successo, -1 in caso di errore
 */
int wait_for_zero_sem(int semid)
{
    struct sembuf sem;
    sem.sem_num = 0;
    sem.sem_op = 0;
    sem.sem_flg = 0;

    if (semop(semid, &sem, 1) == -1)
    {
        if (errno == EINTR)
        {
            // Signal was received, but SA_RESTART will restart the system call
            return decrease_sem(semid);
        }
        else
        {
            perror("Error during decrease operation on semaphore");
            return -1;
        }
    }
    return 0;
}

/**
 * @brief Ottiene il valore attuale del semaforo.
 *
 * Usa `semctl` con `GETVAL` per ottenere il valore corrente del semaforo.
 *
 * @param semid Identificatore del semaforo
 * @return Valore attuale del semaforo (int)
 */
int sem_getvalue(int semid)
{
    return semctl(semid, 0, GETVAL);
}

/**
 * @brief Conta il numero di processi in attesa sul semaforo.
 *
 * Restituisce il numero di processi bloccati in attesa di accedere al semaforo.
 *
 * @param semid Identificatore del semaforo
 * @return Numero di processi in attesa (int)
 */
int how_many_sem(int semid)
{
    int waiting_processes;

    waiting_processes = semctl(semid, 0, GETNCNT);
    if (waiting_processes == -1)
    {
        perror("Error getting the number of waiting processes");
        exit(1);
    }

    return waiting_processes;
}

/**
 * @brief Imposta un nuovo valore per il semaforo.
 *
 * Utilizza `semctl` con `SETVAL` per assegnare un valore specificato al semaforo.
 *
 * @param semid Identificatore del semaforo
 * @param value Nuovo valore da assegnare al semaforo
 * @return 0 se l'operazione ha successo, -1 in caso di errore
 */
int sem_setvalue(int semid, int value)
{
    union semun arg;
    arg.val = value;

    if (semctl(semid, 0, SETVAL, arg) == -1)
    {
        perror("semctl");
        return -1;
    }

    return 0;
}
