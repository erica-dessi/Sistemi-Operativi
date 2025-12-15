/**
 * @file handler.h
 * @brief Dichiarazioni delle funzioni per la gestione dei segnali.
 *
 * Questo file di intestazione contiene le dichiarazioni di funzioni che permettono di impostare,
 * ignorare e resettare i gestori dei segnali in un'applicazione.
 */

#ifndef HANDLER_H
#define HANDLER_H

/**
 * @brief Imposta un gestore personalizzato per un segnale specifico.
 *
 * Questa funzione associa una funzione di gestione personalizzata a un segnale specificato.
 *
 * @param handler Puntatore alla funzione che gestirà il segnale
 * @param signum Numero del segnale da gestire
 * @return 0 in caso di successo, -1 in caso di errore
 */
int set_handler(void (*handler)(int signum), int signum);

/**
 * @brief Ignora un segnale specifico.
 *
 * Configura il programma per ignorare il segnale indicato, senza interrompere l'esecuzione.
 *
 * @param signum Numero del segnale da ignorare
 * @return 0 in caso di successo, -1 in caso di errore
 */
int ignore(int signum);

/**
 * @brief Ripristina il gestore predefinito di un segnale.
 *
 * Questa funzione riporta il comportamento di gestione del segnale alle impostazioni predefinite.
 *
 * @param signum Numero del segnale da resettare
 * @return 0 in caso di successo, -1 in caso di errore
 */
int reset_handler(int signum);

#endif