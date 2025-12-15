#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <time.h>
#include "../lib/handler.h"
#include "../lib/code.h"
#include "../lib/semaphore.h"
#include "../lib/shared_memory.h"
#include "../lib/conf.h"

SimulationParams params;
int inibitore_attivo = 1;
int energia_assorbita_ultimo_sec;

shmseg2 *memoria2;

#define MAX_ATOMI_ATTIVI 1000

void inibitore_handler(int signum)
{

    if (inibitore_attivo == 1)
    {
        inibitore_attivo = 0;
    }
    else
    {
        inibitore_attivo = 1;
    }
}

int main()
{
    pid_t pid_inibitore = getpid();
    set_handler(inibitore_handler, SIGUSR2);

    ignore(SIGINT);
    const char *filename = "conf/config.txt";
    params = read_params_from_file(filename);

    int m1 = create_shared_memory("src/master.c", sizeof(shmseg));
    shmseg *memoria = attach_shared_memory(m1);
    int queue = memoria->id_queue;
    int start = memoria->id_start;
    int sem = memoria->id_semaphore;
    int sem_sezione_critica_inib = memoria->sem_sezione_critica_inib;
    int sem_blocca_master = memoria->sem_blocca_master;
    int sem_blocca_inib = memoria->sem_blocca_inib;
    int sem_scissione = memoria->sem_scissione;


    int m2 = create_shared_memory("src/inibitore.c", sizeof(shmseg2));
    memoria2 = attach_shared_memory2(m2);

    wait_for_zero_sem(start);

    while (sem_getvalue(sem) == 0)
    {
        decrease_sem(sem_blocca_inib);

        if (inibitore_attivo)
        {

            decrease_sem(sem_sezione_critica_inib);

            if (((memoria2->energia_totale + memoria2->energia_totale_ultimo_secondo) > params.energy_explode_threshold * 0.75))
    
            {
                
                // Calcola l'energia da assorbire
                memoria2->energia_assorbita_ultimo_sec = (memoria2->energia_totale + memoria2->energia_totale_ultimo_secondo) - (params.energy_explode_threshold / 2);

                memoria2->energia_assorbita += memoria2->energia_assorbita_ultimo_sec;
                // Aggiorna l'energia totale
                memoria2->energia_totale = (memoria2->energia_totale + memoria2->energia_totale_ultimo_secondo) - memoria2->energia_assorbita_ultimo_sec;

                // Debug: stampa l'energia totale prima e dopo l'assorbimento
            }
            else{
                 memoria2->energia_assorbita_ultimo_sec =0;
            }

            

            if (memoria2->atomi_attivi > MAX_ATOMI_ATTIVI) {
              
                
                // Imposta il semaforo a 0 per bloccare la scissione
                if (sem_setvalue(sem_scissione, 0) == -1) {
                    fprintf(stderr, "Errore nell'impostare il valore del semaforo.\n");
                    exit(1);
                }
                

            } else {
                // Imposta il semaforo a 1 per abilitare la scissione
                if (sem_setvalue(sem_scissione, 1) == -1) {
                    
                    fprintf(stderr, "Errore nell'impostare il valore del semaforo.\n");
                    exit(1);
                }
                
            }

            





            increase_sem(sem_sezione_critica_inib);
        }

        increase_sem(sem_blocca_master);
    }

    exit(EXIT_SUCCESS);
}