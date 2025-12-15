/**
 * @file semaphore.h
 * @brief Definizioni e dichiarazioni per la gestione dei semafori.
 *
 * Questo file di intestazione contiene le dichiarazioni delle funzioni per creare, incrementare,
 * decrementare e rimuovere semafori, così come per ottenere e impostare i valori dei semafori.
 */

#ifndef SEMAPHORE_H
#define SEMAPHORE_H

/**
 * @brief Crea un nuovo semaforo.
 *
 * Utilizza un percorso per generare una chiave unica per il semaforo e crea un nuovo semaforo
 * utilizzando tale chiave.
 *
 * @param pathname Percorso per generare la chiave unica del semaforo
 * @return Identificatore del semaforo creato (int)
 */
int create_sem(char *pathname);

/**
 * @brief Incrementa il valore del semaforo.
 *
 * Incrementa di 1 il valore del semaforo specificato.
 *
 * @param semid Identificatore del semaforo da incrementare
 * @return Stato dell'operazione (int)
 */
int increase_sem(int semid);

/**
 * @brief Decrementa il valore del semaforo.
 *
 * Decrementa di 1 il valore del semaforo specificato.
 *
 * @param semid Identificatore del semaforo da decrementare
 * @return Stato dell'operazione (int)
 */
int decrease_sem(int semid);

/**
 * @brief Rimuove un semaforo.
 *
 * Dealloca e rimuove il semaforo specificato dall'identificatore.
 *
 * @param semid Identificatore del semaforo da rimuovere
 * @return Stato dell'operazione (int)
 */
int remove_sem(int semid);

/**
 * @brief Attende che il semaforo raggiunga il valore zero.
 *
 * Blocca l'esecuzione fino a quando il valore del semaforo specificato è uguale a zero.
 *
 * @param semid Identificatore del semaforo
 * @return Stato dell'operazione (int)
 */
int wait_for_zero_sem(int semid);

/**
 * @brief Ottiene il valore attuale del semaforo.
 *
 * Restituisce il valore corrente del semaforo specificato.
 *
 * @param semid Identificatore del semaforo
 * @return Valore del semaforo (int)
 */
int sem_getvalue(int semid);

/**
 * @brief Conta il numero di processi in attesa sul semaforo.
 *
 * Restituisce il numero di processi che sono bloccati in attesa di accedere al semaforo specificato.
 *
 * @param semid Identificatore del semaforo
 * @return Numero di processi in attesa (int)
 */
int how_many_sem(int semid);

/**
 * @brief Imposta il valore del semaforo.
 *
 * Assegna un valore specificato al semaforo, sovrascrivendo il valore corrente.
 *
 * @param semid Identificatore del semaforo
 * @param value Nuovo valore da assegnare al semaforo
 * @return Stato dell'operazione (int)
 */
int sem_setvalue(int semid, int value);

#endif
