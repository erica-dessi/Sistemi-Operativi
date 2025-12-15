#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <signal.h>
#include "../lib/handler.h"
#include "../lib/code.h"
#include "../lib/semaphore.h"
#include "../lib/shared_memory.h"
#include "../lib/conf.h"

/**
 * @brief gestore del segnale di terminazione, imposta a 0 la flag "simulazione_in_corso"
 */

int main()
{
    int m1 = create_shared_memory("src/master.c", sizeof(shmseg));
    shmseg *memoria = attach_shared_memory(m1);
    ignore(SIGINT);
    ignore(SIGUSR2);

    int queue = memoria->id_queue;
    int sem = memoria->id_semaphore;
    int start = memoria->id_start;
    int attivatore_sem = memoria->id_attivatore_sem;

    wait_for_zero_sem(start);

    while (sem_getvalue(sem) == 0)
    {
        if (how_many_sem(attivatore_sem) > 0)
        {
            increase_sem(attivatore_sem);
            send_type_message(queue, 1, 12);
        }
        usleep(500000);
    }

    sem_setvalue(attivatore_sem, 10000);

    exit(EXIT_SUCCESS);
}
