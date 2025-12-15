#include "../headers/atomo.h"
/**
 * @file atomo.c
 * @brief Gestione della scissione e delle operazioni di simulazione degli atomi.
 *
 * Questo programma simula il comportamento degli atomi che possono scindersi e rilasciare energia.
 * Utilizza memoria condivisa e semafori per sincronizzare i processi e comunica con un processo
 * master per aggiornare il numero di atomi e l'energia generata.
 */

// VARIABILI GLOBALI
int n_atomico;
SimulationParams params;
int queue;
shmseg *memoria;

int main(int argc, char *argv[])
{
    // INIZIALIZZAZIONE

    const char *filename = "conf/config.txt";
    params = read_params_from_file(filename);
    n_atomico = atoi(argv[1]);
    ignore(SIGCHLD);
    ignore(SIGINT);
    ignore(SIGUSR2);

    int m1 = create_shared_memory("src/master.c", sizeof(shmseg));
    memoria = attach_shared_memory(m1);

    //dprintf(1, "AAAAAA");

    queue = memoria->id_queue;
    int sem = memoria->id_semaphore;
    int start = memoria->id_start;
    int attivatore_sem = memoria->id_attivatore_sem;

    send_type_message(queue, 1, 4);

    wait_for_zero_sem(start);

    // CICLO DELLA SIMULAZIONE
    while (sem_getvalue(sem) == 0)
    {
        if (n_atomico < params.min_n_atomico)
        {
            send_type_message(queue, -1, 4);
            send_type_message(queue, 1, 10);
            exit(EXIT_SUCCESS);
        }

        decrease_sem(attivatore_sem);

        if (sem_getvalue(sem) == 0)
        {
            int energy = scissione();
            send_type_message(queue, energy, 5);
            
        }
    }

    send_type_message(queue, -1, 4);
    sem_setvalue(attivatore_sem, 10000);
    exit(EXIT_SUCCESS);
}

int max(int a, int b)
{
    return (a > b) ? a : b;
}

int energy(int n_atomico_figlio)
{
    int energy = n_atomico * n_atomico_figlio - max(n_atomico, n_atomico_figlio);
    return energy;
}

int scissione() 
{

    int n_atomico_figlio = calcolo_numero_atomico(n_atomico);
        n_atomico = n_atomico - n_atomico_figlio;
        
    if (sem_getvalue(memoria->sem_scissione) == 0) {
    // Non fare nulla, la scissione è bloccata
    return 0; // Evita la creazione di nuovi atomi
    }
    else{
        // Altrimenti, prosegui con la creazione degli atomi
        new_atomo(n_atomico_figlio);
        send_type_message(queue, 1, 9);
        return energy(n_atomico_figlio);
    }
}

int new_atomo(int n_atomico)
{
    char buffer[100];
    sprintf(buffer, "%d", n_atomico);
    char *pathname = "bin/atomo";
    int pid = fork();

    switch (pid)
    {
    case -1:
        perror("Error starting the process");
        send_type_message(queue, -1, 4);
        send_type_message(queue, 3, 15);
        exit(EXIT_FAILURE);
        break;
    case 0:
        execlp(pathname, pathname, buffer, NULL);
        perror("Exec fallito");
        exit(EXIT_FAILURE);
    default:
        return pid;
    }
}

int calcolo_numero_atomico(int n_atomico_max)
{
    srand(time(NULL));
    int n_atomico = rand() % (n_atomico_max - 1) + 1;
    return n_atomico;
}
