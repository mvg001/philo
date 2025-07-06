/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvassall <mvassall@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 17:39:27 by mvassall          #+#    #+#             */
/*   Updated: 2025/07/06 16:02:09 by mvassall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <pthread.h>
#include <stdint.h>
#include <unistd.h>

static t_exit_status    eating_loop(t_philo *phi, uint64_t end_eating_ts)
{
    uint64_t    ts;

    while (1)
    {
        if (has_someone_died(phi->cfg))
        {
            pthread_mutex_unlock(&phi->mtx);
            return (EX_PHILOSOPHER_DEAD);
        }
        ts = get_time_us();
        if (ts >= phi->death_ts)
        {
            change_phi_status(phi, PHI_DIED);
            pthread_mutex_unlock(&phi->mtx);
            return (EX_TIMEOUT) ;
        }
        if (ts >= end_eating_ts)
        {
            phi->n_eating_counter++;
            pthread_mutex_unlock(&phi->mtx);
            return (EX_OK);
        }
        usleep(min(end_eating_ts - ts, delta_time_us));
    }
}

static t_exit_status  eating(t_philo *phi)
{
    uint64_t    end_eating_ts;
    uint64_t    ts;

    pthread_mutex_lock(&phi->mtx);
    ts = get_time_us();
    phi->death_ts = ts + phi->cfg->time_to_die_us;
    end_eating_ts = ts + phi->cfg->time_to_eat_us;
    change_phi_status(phi, PHI_GOT_A_FORK);
    change_phi_status(phi, PHI_GOT_A_FORK);
    change_phi_status(phi, PHI_EATING);
    return (eating_loop(phi, end_eating_ts));
}

static t_exit_status    sleeping(t_philo *phi)
{
    uint64_t    end_sleeping_ts;
    uint64_t    ts;

    end_sleeping_ts = get_time_us() + phi->cfg->time_to_sleep_us;
    phi->status = PHI_SLEEPING;
    philo_print(phi);
    while (1)
    {
        if (has_someone_died(phi->cfg))
            return (EX_PHILOSOPHER_DEAD);
        ts = get_time_us();
        if (ts >= phi->death_ts)
        {
            phi->status = PHI_DIED;
            philo_print(phi);
            return (EX_TIMEOUT) ;
        }
        if (ts >= end_sleeping_ts)
            return (EX_OK);
        usleep(min(end_sleeping_ts - ts, delta_time_us));       
    }
    return (EX_OK);
}

void *philosopher_routine(void *args)
{
    t_philo *phi;

    phi = (t_philo *)args;
    phi->status = PHI_THINKING;
    philo_print(phi);
    phi->n_eating_counter = 0;
    phi->death_ts = get_time_us() + phi->cfg->time_to_die_us;
    while (1)
    {
        if (eating(phi) != EX_OK || (phi->cfg->n_eating_rounds > 0
            && phi->n_eating_counter >= phi->cfg->n_eating_rounds))
        {
            increment_dead_counter(phi->cfg);
            break ;
        }
        if (sleeping(phi) != EX_OK)
        {
            increment_dead_counter(phi->cfg);
            break ;
        }
        phi->status = PHI_THINKING;
        philo_print(phi);
    }
    return (NULL);
}
