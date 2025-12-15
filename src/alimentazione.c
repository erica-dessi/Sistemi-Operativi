#include "../headers/alimentazione.h"

// VARIABILI GLOBALI
SimulationParams params;

int queue;

int main(int argc, char *argv[])
{
    // INIZIALIZZAZIONE
    const char *filename = "conf/config.txt";
    params = read_params_from_file(filename);
    ignore(SIGINT);
    ignore(SIGUSR2);
    ignore(SIGCHLD);

    struct timespec time;
    time.tv_sec = params.step / 1000000;
    time.tv_nsec = (params.step % 1000000) * 1000;

    int m1 = create_shared_memory("src/master.c", sizeof(shmseg));
    shmseg *memoria = attach_shared_memory(m1);
    queue = memoria->id_queue;
    int sem = memoria->id_semaphore;
    int start = memoria->id_start;

    wait_for_zero_sem(start);

    // CICLO DELLA SIMULAZIONE
    while (sem_getvalue(sem) == 0)
    {
        nanosleep(&time, NULL);
        for (int i = 0; i < params.n_nuovi_atomi && sem_getvalue(sem) == 0; i++)
        {
            int numero_atomico = calcolo_numero_atomico(params.n_atom_max);
            if (sem_getvalue(memoria->sem_scissione) != 0){
                new_atomo(numero_atomico);
            }
        }
    }

    exit(EXIT_SUCCESS);
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
