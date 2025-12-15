/**
 * @file shared_memory.h
 * @brief Definizioni e dichiarazioni per la gestione della memoria condivisa.
 *
 * Questo file di intestazione contiene la struttura `shmseg` utilizzata per identificare
 * e accedere alla memoria condivisa, e le dichiarazioni delle funzioni per la creazione,
 * connessione e rimozione della memoria condivisa.
 */

#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

/**
 * @struct shmseg_
 * @brief Struttura per rappresentare un segmento di memoria condivisa.
 *
 * La struttura `shmseg` contiene gli identificatori necessari per la comunicazione
 * e sincronizzazione tramite memoria condivisa e semafori tra i processi.
 */
typedef struct shmseg_
{
    int id_queue;
    int id_semaphore;
    int id_attivatore_sem;
    int id_start;
    int sem_blocca_master;
    int sem_blocca_inib;
    int sem_sezione_critica_inib;
    int sem_scissione;
} shmseg;

typedef struct shmseg2_
{
    int atomi_attivi;
    int energia_totale;
    int energia_totale_ultimo_secondo;
    int energia_prelevata;
    int energia_assorbita;
    int inibitore_attivo;
    int energia_assorbita_ultimo_sec;
} shmseg2;

/**
 * @brief Crea un nuovo segmento di memoria condivisa.
 *
 * Questa funzione utilizza un percorso e una dimensione specificata per creare un segmento
 * di memoria condivisa e restituisce il suo identificatore.
 *
 * @param pathname Percorso per identificare univocamente la memoria condivisa
 * @param size Dimensione del segmento di memoria condivisa
 * @return Identificatore del segmento di memoria condivisa (int)
 */
int create_shared_memory(char *pathname, size_t size);

/**
 * @brief Collega il processo corrente al segmento di memoria condivisa.
 *
 * La funzione collega il processo corrente al segmento di memoria condivisa specificato dall'identificatore
 * e restituisce un puntatore alla struttura `shmseg` per accedere agli identificatori condivisi.
 *
 * @param id Identificatore del segmento di memoria condivisa
 * @return Puntatore alla struttura `shmseg` collegata alla memoria condivisa
 */
shmseg *attach_shared_memory(int id);
shmseg2 *attach_shared_memory2(int id);

/**
 * @brief Rimuove un segmento di memoria condivisa.
 *
 * La funzione rimuove il segmento di memoria condivisa specificato dall'identificatore, liberando
 * la risorsa condivisa.
 *
 * @param id Identificatore del segmento di memoria condivisa da rimuovere
 * @return Stato di successo o fallimento dell'operazione (int)
 */
int remove_shared_memory(int id);

#endif
