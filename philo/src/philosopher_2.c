/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvassall <mvassall@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/10 10:58:13 by mvassall          #+#    #+#             */
/*   Updated: 2025/07/10 14:47:09 by mvassall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <pthread.h>
#include <unistd.h>

t_exit_status    pass_time(t_philo *phi, uint64_t end_ts)
{
    uint64_t    ts;

    while (1)
    {
        if (has_someone_died(phi->cfg))
            return (EX_PHILOSOPHER_DEAD);
        ts = get_time_us();
        if (ts >= phi->death_ts)
        {
            change_phi_status(phi, PHI_DIED);
            return (EX_TIMEOUT) ;
        }
        if (ts >= end_ts)
        {
            phi->n_eating_counter++;
            return (EX_OK);
        }
        usleep(min(end_ts - ts, DELTA_TIME_US));
    }
}

int set_fork(t_cfg_philo *cfg, int i, int value)
{
    int rc;

    if (cfg == NULL)
        return (-1);
    pthread_mutex_lock(cfg->m_forks + i);
    rc = 0;
    if (cfg->is_fork_in_use[i] == 0)
    {
        cfg->is_fork_in_use[i] = value;
        rc = 1;        
    }
    else if (value == 0)
    {
        cfg->is_fork_in_use[i] = value;
        rc = 1;
    }
    pthread_mutex_unlock(cfg->m_forks + i);
    return (rc);
}

t_exit_status   take_forks(t_philo *phi)
{
    while (1)
    {
        if (has_someone_died(phi->cfg))
            return (EX_PHILOSOPHER_DEAD);
        if (get_time_us() >= phi->death_ts)
            return (change_phi_status(phi, PHI_DIED), EX_TIMEOUT);
        if (set_fork(phi->cfg, phi->id, 1) == 1)
        {
            if (phi->cfg->n_philosophers == 1)
                return (philo_print(phi, "has taken a fork"), EX_OK);
            if (set_fork(phi->cfg,
                i_prev(phi->id, phi->cfg->n_philosophers), 1) == 1)
            {
                philo_print(phi, "has taken a fork");
                return (philo_print(phi, "has taken a fork"), EX_OK);
            }
            set_fork(phi->cfg, phi->id, 0);
        }
        usleep(DELTA_TIME_US);
    }
    return (EX_OK);
}

t_exit_status drop_forks(t_philo *phi)
{
    set_fork(phi->cfg, phi->id, 0);
    if (phi->cfg->n_philosophers != 1)
    {
        set_fork(phi->cfg,
            i_prev(phi->id, phi->cfg->n_philosophers), 0);
    }
    return (EX_OK);
}