/**
 * @file queue.h
 * @brief Funzionalità per la gestione delle code di messaggi.
 *
 * Questo file contiene le dichiarazioni delle funzioni per creare, leggere, inviare e rimuovere messaggi
 * tramite code di messaggi IPC System V.
 */

#ifndef QUEUE_H
#define QUEUE_H

/**
 * @brief Crea una nuova coda di messaggi.
 *
 * Genera una coda di messaggi utilizzando il percorso specificato per ottenere una chiave IPC.
 *
 * @param pathname Percorso utilizzato per ottenere la chiave della coda di messaggi
 * @return L'identificatore della coda in caso di successo, -1 in caso di errore
 */
int create_queue(char *pathname);

/**
 * @brief Legge un messaggio dalla coda.
 *
 * Legge un messaggio dalla coda di messaggi identificata da `id`.
 *
 * @param id Identificatore della coda di messaggi
 * @return Il messaggio letto in caso di successo, -1 in caso di errore
 */
int read_message(int id);

/**
 * @brief Invia un messaggio a una coda specificata.
 *
 * Invia un messaggio alla coda identificata da `id`.
 *
 * @param id Identificatore della coda di messaggi
 * @param msg Messaggio da inviare
 * @return 0 in caso di successo, -1 in caso di errore
 */
int send_message(int id, int msg);

/**
 * @brief Invia un messaggio di un tipo specifico a una coda.
 *
 * Invia un messaggio con un tipo specificato alla coda di messaggi identificata da `id`.
 *
 * @param id Identificatore della coda di messaggi
 * @param msg Messaggio da inviare
 * @param tipo Tipo di messaggio (intero positivo per identificare il tipo)
 * @return 0 in caso di successo, -1 in caso di errore
 */
int send_type_message(int id, int msg, int tipo);

/**
 * @brief Legge un messaggio di un tipo specifico da una coda.
 *
 * Legge il primo messaggio di un tipo specificato dalla coda identificata da `id`.
 *
 * @param id Identificatore della coda di messaggi
 * @param tipo Tipo di messaggio da leggere
 * @return Il messaggio letto in caso di successo, -1 in caso di errore
 */
int read_type_message(int id, int tipo);

/**
 * @brief Legge un messaggio di un tipo specifico dalla coda senza bloccare.
 *
 * Legge un messaggio di un tipo specificato dalla coda identificata da `id` in modalità non bloccante.
 *
 * @param id Identificatore della coda di messaggi
 * @param tipo Tipo di messaggio da leggere
 * @return Il messaggio letto in caso di successo, -1 se non ci sono messaggi disponibili o in caso di errore
 */
int read_type_message_nb(int id, int tipo);

/**
 * @brief Rimuove la coda di messaggi.
 *
 * Elimina la coda di messaggi identificata da `id`.
 *
 * @param id Identificatore della coda di messaggi
 * @return 0 in caso di successo, -1 in caso di errore
 */
int remove_queue(int id);

int somma_messaggi_di_tipo(int queue_id, long tipo);

#endif
