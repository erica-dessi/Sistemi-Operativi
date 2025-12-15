#include "../headers/master.h"

/**
 * @file master.c
 * @brief Programma principale per la gestione di una simulazione tramite memoria condivisa e semafori.
 *
 * Questo programma utilizza la memoria condivisa e i semafori per sincronizzare l'inizio e la gestione
 * della simulazione. La simulazione attende un segnale di avvio, e durante il ciclo di esecuzione
 * aggiorna i valori dei semafori a intervalli regolari.
 */

// VARIABILI GLOBALI
SimulationParams params;
int tempo_passato = 0;
int simulazione_in_corso = 1;
int queue;
int num_scissioni = 0;
int num_scissioni_ultimo_secondo = 0;
int num_scorie = 0;
int num_scorie_ultimo_secondo = 0;
int num_attivazioni = 0;
int num_attivazioni_ultimo_secondo = 0;
int causa_terminazione = 0;
int inibitore_attivo = 1;
pid_t pid_inibitore;
shmseg2 *memoria2;
int sem_sezione_critica_inib;
int sem_blocca_master;
int sem_blocca_inib;
int avvia_inibitore;
int sem_scissione;

int main(int argc, char *argv[])
{
    ignore(SIGCHLD);
    

    set_handler(inibitore_handler, SIGINT);

    // Input da tastiera per scegliere se avviare il processo inibitore
    do
    {
        printf("Vuoi avviare il processo inibitore? (1 = Sì, 0 = No): ");
        if (scanf("%d", &avvia_inibitore) != 1)
        {
            fprintf(stderr, "Errore: input non valido. Inserisci 1 o 0.\n");
            // Pulisce il buffer di input
            while (getchar() != '\n')
                ;
            continue;
        }
        if (avvia_inibitore != 0 && avvia_inibitore != 1)
        {
            fprintf(stderr, "Errore: devi inserire 1 (sì) o 0 (no).\n");
        }
    } while (avvia_inibitore != 0 && avvia_inibitore != 1);

    const char *filename = "conf/config.txt";
    params = read_params_from_file(filename);

    set_handler(alarm_handler, SIGALRM);
    queue = create_queue("src/master.c");
    int sem = create_sem("src/master.c");
    int start_sem = create_sem("src/alimentazione.c");
    int attivatore_sem = create_sem("src/attivatore.c");
    sem_sezione_critica_inib = create_sem("lib/semaphore.c");
    sem_blocca_master = create_sem("lib/semaphore.h");
    sem_blocca_inib = create_sem("lib/code.c");
    sem_scissione = create_sem("lib/conf.c");

    increase_sem(sem);
    increase_sem(start_sem);
    increase_sem(sem_sezione_critica_inib);
    increase_sem(sem_scissione);

    int m1 = create_shared_memory("src/master.c", sizeof(shmseg));

    int m2 = create_shared_memory("src/inibitore.c", sizeof(shmseg2));

    shmseg *memoria = attach_shared_memory(m1);
    memoria2 = attach_shared_memory2(m2);

    memoria->id_queue = queue;
    memoria->id_semaphore = sem;
    memoria->id_attivatore_sem = attivatore_sem;
    memoria->id_start = start_sem;
    memoria->sem_sezione_critica_inib = sem_sezione_critica_inib;
    memoria->sem_blocca_master = sem_blocca_master;
    memoria->sem_blocca_inib = sem_blocca_inib;
    memoria->sem_scissione = sem_scissione;


    sleep(1);

    pid_t pid_attivatore = start("bin/attivatore");
    pid_t pid_alimentazione = start("bin/alimentazione");

    if (avvia_inibitore)
    {
        pid_inibitore = start("bin/inibitore");
        dprintf(1, "Processo inibitore avviato.\n");
    }
    else
    {
        dprintf(1, "Processo inibitore non avviato.\n");
    }

    for (int i = 0; i < params.n_atomi_init; i++)
    {
        new_atomo(params.n_atom_max);
    }

    for (int i = 3; i > 0; i--)
    {
        sleep(1);
        dprintf(1, "AVVIO SIMULAZIONE IN %d\n", i);
    }
    decrease_sem(sem);

    decrease_sem(start_sem);

    alarm(1); // Inizia il timer impostando un allarme ogni secondo.

    while (simulazione_in_corso && causa_terminazione == 0)
    {
        memoria2->energia_totale_ultimo_secondo += read_type_message_nb(queue, 5);
        num_scissioni_ultimo_secondo += read_type_message_nb(queue, 9);
        num_scorie_ultimo_secondo += read_type_message_nb(queue, 10);
        num_attivazioni_ultimo_secondo += read_type_message_nb(queue, 12);
        causa_terminazione = read_type_message_nb(queue, 15);
    }

    switch (causa_terminazione)
    {
    case 0:
        dprintf(1, "TIMEOUT\n\n");
        break;
    case 1:
        dprintf(1, "EXPLODE\n\n");
        break;
    case 2:
        dprintf(1, "BLACKOUT\n\n");
        break;
    case 3:
        dprintf(1, "MELTDOWN\n\n");
        dprintf(1, "Attendo terminazione atomi\n\n");
        break;
    }

    increase_sem(sem);

    while (memoria2->atomi_attivi)
    {
        memoria2->atomi_attivi += read_type_message_nb(queue, 4); // Aspetta fino a quando ci sono atomi attivi.
    }

    increase_sem(sem_blocca_inib);

    waitpid(pid_alimentazione, NULL, 0);
    waitpid(pid_inibitore, NULL, 0);

    sleep(1);

    remove_queue(queue);
    remove_sem(sem);
    remove_sem(attivatore_sem);
    remove_sem(start_sem);
    remove_sem(sem_sezione_critica_inib);
    remove_sem(sem_blocca_inib);
    remove_sem(sem_blocca_master);
    remove_sem(sem_scissione); 
    remove_shared_memory(m1);
    remove_shared_memory(m2);
    printf("FINE SIMULAZIONE\n");

    exit(EXIT_SUCCESS);
}

void alarm_handler(int signum)
{
    tempo_passato++;

    if (tempo_passato < params.sim_duration && causa_terminazione == 0)
    {
        decrease_sem(sem_sezione_critica_inib);
        aggiorna_simulazione();
        increase_sem(sem_sezione_critica_inib);


        if(avvia_inibitore) {
            increase_sem(sem_blocca_inib);
            //qua in mezzo agisce l'inibitore
            decrease_sem(sem_blocca_master);
        }

        decrease_sem(sem_sezione_critica_inib);
        stato_simulazione();

        if (memoria2->energia_totale > params.energy_explode_threshold)
        {
            send_type_message(queue, 1, 15);
        }
        else if (memoria2->energia_totale < params.energy_demand)
        {
            send_type_message(queue, 2, 15);
        }
        else
        {
            alarm(1); // Imposta il prossimo allarme se la simulazione non è finita.
        }
        increase_sem(sem_sezione_critica_inib);
    }
    else
    {
        simulazione_in_corso = 0;
    }
}

int start(char *pathname)
{
    pid_t pid = fork();
    switch (pid)
    {
    case -1:
        perror("Error starting the process");
        send_type_message(queue, -1, 4);
        send_type_message(queue, 3, 15);
        exit(EXIT_FAILURE);
        break;
    case 0:
        execlp(pathname, pathname, NULL);
        perror("Exec fallito");
        exit(EXIT_FAILURE);
    default:
        return pid;
    }
}

void aggiorna_simulazione()
{
    num_attivazioni += num_attivazioni_ultimo_secondo;
    num_scissioni += num_scissioni_ultimo_secondo;
    num_scorie += num_scorie_ultimo_secondo;
    memoria2->energia_totale += memoria2->energia_totale_ultimo_secondo;
    memoria2->energia_prelevata += params.energy_demand;
    memoria2->energia_totale -= params.energy_demand;
    memoria2->energia_assorbita;
    memoria2->atomi_attivi += somma_messaggi_di_tipo(queue, 4);
}

void stato_simulazione()
{
    dprintf(1, "\n");
    dprintf(1, "TEMPO RESTANTE: %d secondi\n", params.sim_duration - tempo_passato);
    dprintf(1, "atomi attivi: %d\n", memoria2->atomi_attivi);
    dprintf(1, "Numero Attivazioni: %d, Ultimo secondo: %d\n", num_attivazioni, num_attivazioni_ultimo_secondo);
    dprintf(1, "Energia prodotta: %d, Ultimo secondo: %d\n", memoria2->energia_totale, memoria2->energia_totale_ultimo_secondo);
    dprintf(1, "Energia consumata: %d, Ultimo secondo: %d\n", memoria2->energia_prelevata, params.energy_demand);
    dprintf(1, "Numero scissioni: %d, Ultimo secondo: %d\n", num_scissioni, num_scissioni_ultimo_secondo);
    dprintf(1, "Quantita' scorie: %d, Ultimo secondo: %d\n", num_scorie, num_scorie_ultimo_secondo);

    if(avvia_inibitore ==1){
        if(inibitore_attivo==1){
    dprintf(1,"\n----INIBITORE ATTIVO----\n");
        }
        else{
             dprintf(1,"\n----INIBITORE INATTIVO----\n");
        }
    dprintf(1, "Energia assorbita: %d, Ultimo secondo:%d\n", memoria2->energia_assorbita, memoria2->energia_assorbita_ultimo_sec);
    if(sem_getvalue(sem_scissione)==0)
    {
        dprintf(1, "Scissioni bloccate perchè ci sono troppi atomi attivi.\n\n");
    }
    else{
        dprintf(1,"Scissioni in corso.\n\n");
    }
    }

    memoria2->energia_totale_ultimo_secondo = 0;
    num_scissioni_ultimo_secondo = 0;
    num_scorie_ultimo_secondo = 0;
    num_attivazioni_ultimo_secondo = 0;
}

int new_atomo(int n_atomico)
{
    char buffer[100];
    sprintf(buffer, "%d", n_atomico); // Converti il numero atomico in stringa
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

void inibitore_handler(int signum)
{
    if(avvia_inibitore == 1) {
        if (signum == SIGINT)
        {
            if (inibitore_attivo)
            {
                kill(pid_inibitore, SIGUSR2);
                inibitore_attivo = 0;
            }
            else
            {
                kill(pid_inibitore, SIGUSR2);
                inibitore_attivo = 1;
            }
        }
    }
}
