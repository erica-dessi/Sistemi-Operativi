/**
 * @file shared_memory.c
 * @brief Implementazione delle funzioni per la gestione della memoria condivisa.
 *
 * Questo file contiene le funzioni per creare, collegare e rimuovere segmenti di memoria condivisa
 * utilizzando le librerie di sistema per la memoria condivisa e le chiavi IPC.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/shm.h>
#include "shared_memory.h"

/**
 * @brief Crea un nuovo segmento di memoria condivisa.
 *
 * Utilizza un percorso e un carattere identificativo per generare una chiave unica tramite `ftok`,
 * quindi crea il segmento di memoria condivisa con `shmget`. Se la creazione fallisce, stampa un
 * errore e termina il programma.
 *
 * @param pathname Percorso per generare la chiave della memoria condivisa
 * @param size Dimensione in byte del segmento di memoria condivisa
 * @return Identificatore del segmento di memoria condivisa creato (int)
 */
int create_shared_memory(char *pathname, size_t size)
{
    key_t key = ftok(pathname, 'x');
    int id = shmget(key, size, IPC_CREAT | 0666);
    if (id == -1)
    {
        perror("shmget error");
        exit(EXIT_FAILURE);
    }
    return id;
}

/**
 * @brief Collega il processo al segmento di memoria condivisa.
 *
 * Utilizza `shmat` per collegare il processo corrente al segmento di memoria condivisa
 * specificato dall'identificatore. Se il collegamento fallisce, stampa un errore e termina il programma.
 *
 * @param id Identificatore del segmento di memoria condivisa
 * @return Puntatore al segmento di memoria condivisa (`shmseg *`)
 */
shmseg *attach_shared_memory(int id)
{
    shmseg *shmp = (shmseg *)shmat(id, NULL, 0);
    if (shmp == (void *)-1)
    {
        perror("shmat");
        exit(EXIT_FAILURE);
    }
    return shmp;
}

shmseg2 *attach_shared_memory2(int id)
{
    shmseg2 *shmp = (shmseg2 *)shmat(id, NULL, 0);
    if (shmp == (void *)-1)
    {
        perror("shmat");
        exit(EXIT_FAILURE);
    }
    return shmp;
}

/**
 * @brief Rimuove un segmento di memoria condivisa.
 *
 * Utilizza `shmctl` con l'opzione `IPC_RMID` per deallocare e rimuovere il segmento di memoria
 * condivisa specificato dall'identificatore. Se la rimozione fallisce, stampa un errore e termina il programma.
 *
 * @param id Identificatore del segmento di memoria condivisa da rimuovere
 * @return Restituisce l'identificatore del segmento rimosso (int)
 */
int remove_shared_memory(int id)
{
    if (shmctl(id, IPC_RMID, 0) == -1)
    {
        perror("shmctl error");
        exit(EXIT_FAILURE);
    }

    return id;
}
