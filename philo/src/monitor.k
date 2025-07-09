/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvassall <mvassall@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 09:50:49 by mvassall          #+#    #+#             */
/*   Updated: 2025/07/07 10:54:09 by mvassall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <pthread.h>
#include <unistd.h>

void monitoring(t_cfg_philo *cfg)
{
    int i_phi;

    while (cfg->dead_counter == 0)
    {
        i_phi = get_next_to_eat(cfg);
        if (i_phi >= 0)
        {
            cfg->philos[i_phi].status = PHI_EATING;
            pthread_mutex_unlock(&cfg->philos[i_phi].mtx);
        }
        usleep(DELTA_TIME_US);
    }
    i_phi = -1;
    while (++i_phi < cfg->n_philosophers)
        if (cfg->philos[i_phi].status == PHI_THINKING)
            pthread_mutex_unlock(&cfg->philos[i_phi].mtx);
    while (cfg->dead_counter != cfg->n_philosophers)
        usleep(DELTA_TIME_US);
}
