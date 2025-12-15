#ifndef SIMULATION_PARAMS_H
#define SIMULATION_PARAMS_H

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    int energy_demand;
    int n_atomi_init;
    int n_atom_max;
    int min_n_atomico;
    int n_nuovi_atomi;
    int sim_duration;
    int energy_explode_threshold;
    long long step;
} SimulationParams;

SimulationParams read_params_from_file(const char *filename);

#endif
