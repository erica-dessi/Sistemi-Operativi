#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "conf.h"

SimulationParams read_params_from_file(const char *filename)
{
    SimulationParams params = {0}; // Inizializza tutti i parametri a zero
    FILE *file = fopen(filename, "r");

    if (file == NULL)
    {
        perror("Errore nell'aprire il file");
        exit(EXIT_FAILURE);
    }

    char line[256];

    while (fgets(line, sizeof(line), file))
    {
        // Ignora commenti che iniziano con //
        char *comment_start = strstr(line, "//");
        if (comment_start != NULL)
        {
            *comment_start = '\0'; // Tronca la linea al commento
        }

        // Ignora righe vuote
        if (strlen(line) == 0 || (line[0] == '\n'))
        {
            continue;
        }

        if (sscanf(line, "ENERGY_DEMAND = %d", &params.energy_demand) == 1)
        {
            continue;
        }
        if (sscanf(line, "N_ATOMI_INIT = %d", &params.n_atomi_init) == 1)
        {
            continue;
        }
        if (sscanf(line, "N_ATOM_MAX = %d", &params.n_atom_max) == 1)
        {
            continue;
        }
        if (sscanf(line, "MIN_N_ATOMICO = %d", &params.min_n_atomico) == 1)
        {
            continue;
        }
        if (sscanf(line, "N_NUOVI_ATOMI = %d", &params.n_nuovi_atomi) == 1)
        {
            continue;
        }
        if (sscanf(line, "SIM_DURATION = %d", &params.sim_duration) == 1)
        {
            continue;
        }
        if (sscanf(line, "ENERGY_EXPLODE_THRESHOLD = %d", &params.energy_explode_threshold) == 1)
        {
            continue;
        }
        if (sscanf(line, "STEP = %lld", &params.step) == 1)
        {
            continue;
        }
    }

    fclose(file);
    return params;
}
