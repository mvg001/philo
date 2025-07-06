/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils_3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvassall <mvassall@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 16:39:42 by mvassall          #+#    #+#             */
/*   Updated: 2025/07/06 15:58:40 by mvassall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <pthread.h>
#include <stdlib.h>
#include <string.h>

t_cfg_philo *init_cfg_philo(int *args)
{
    t_cfg_philo *cfg;

    cfg = malloc(sizeof(t_cfg_philo));
    if (cfg == NULL)
        return (NULL);
    memset(cfg, 0, sizeof(t_cfg_philo));
    cfg->philos = malloc(args[N_PHILOSOPHERS] * sizeof(t_philo));
    if (cfg->philos == NULL)
        return (free(cfg), NULL);
    memset(cfg->philos, 0, args[N_PHILOSOPHERS] * sizeof(t_philo));
    cfg->n_philosophers = args[N_PHILOSOPHERS];
    cfg->time_to_die_us = args[TIME_TO_DIE] * 1000;
    cfg->time_to_eat_us = args[TIME_TO_EAT] * 1000;
    cfg->time_to_sleep_us = args[TIME_TO_SLEEP] * 1000;
    cfg->n_eating_rounds = args[N_EATING_ROUNDS];
    pthread_mutex_init(&cfg->print_mutex, NULL);
    pthread_mutex_init(&cfg->m_dead_counter, NULL);
    cfg->dead_counter = 0;
    cfg->start_ts = get_time_us();
    return (cfg);
}

void    start_all_philos(t_cfg_philo *cfg)
{
    int i;
    
    i = -1;
    while (++i < cfg->n_philosophers)
    {
        cfg->philos[i].id = i + 1;
        cfg->philos[i].status = PHI_THINKING;
        cfg->philos[i].n_eating_counter = 0;
        cfg->philos[i].cfg = cfg;
        pthread_mutex_init(&cfg->philos[i].mtx, NULL);
        pthread_mutex_lock(&cfg->philos[i].mtx);
        cfg->philos[i].death_ts = get_time_us() + cfg->time_to_die_us;
        pthread_create(&cfg->philos[i].thread, NULL,
            philosopher_routine, cfg->philos + i);
    }
}

int has_someone_died(t_cfg_philo *cfg)
{
    int n_dead;

    pthread_mutex_lock(&cfg->m_dead_counter);
    n_dead = cfg->dead_counter;
    pthread_mutex_unlock(&cfg->m_dead_counter);
    return (n_dead != 0);    
}

void    increment_dead_counter(t_cfg_philo *cfg)
{
    pthread_mutex_lock(&cfg->m_dead_counter);
    cfg->dead_counter++;
    pthread_mutex_unlock(&cfg->m_dead_counter);
}

int  min(uint64_t a, uint64_t b)
{
    if (a < b)
        return (a);
    return (b);
}
