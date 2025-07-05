/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvassall <mvassall@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 17:39:27 by mvassall          #+#    #+#             */
/*   Updated: 2025/07/05 19:17:34 by mvassall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <pthread.h>
#include <stdint.h>
#include <unistd.h>

static int  eating_interval(t_philo *phi)
{
    uint64_t    start_ts;
    uint64_t    end_ts;
    long        delay;

    phi->status = EATING;
    start_ts = get_time_us();
    phi->death_ts = start_ts + phi->cfg->time_to_die_us;
    end_ts = start_ts + phi->cfg->time_to_eat_us;
    delay = end_ts - get_time_us();
    while (delay > 0)
    {
        if (delay > delta_time_us)
            delay = delta_time_us;
        usleep(delay);
        if (has_someone_died(phi->cfg))
            return (1);
        delay = end_ts - get_time_us();
    }
    return (0);
}

void *philosopher_routine(void *args)
{
    t_philo *phi;

    phi = (t_philo *)args;
    philo_print(phi);
    while (1)
    {
        pthread_mutex_lock(&phi->mtx);
        if (has_someone_died(phi->cfg) || get_time_us() >= phi->death_ts)
            break ;            
        if (eating_interval(phi) != 0)
            break ;
        phi->
    }
    phi->status = DIED;
    pthread_mutex_unlock(&phi->mtx);
    increment_dead_counter(phi->cfg);
    philo_print(phi);    
    return (NULL);
}
