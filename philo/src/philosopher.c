/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvassall <mvassall@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 17:39:27 by mvassall          #+#    #+#             */
/*   Updated: 2025/07/09 17:59:58 by mvassall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <pthread.h>
#include <stdint.h>
#include <unistd.h>

static t_exit_status    op_forks(t_philo *phi, t_op_forks op)
{
    int next_i;
    int prev_i;
    int (*f)(pthread_mutex_t *);

    f = pthread_mutex_unlock;
    if (op == FORK_TAKE)
        f = pthread_mutex_lock;
    next_i = i_next(phi->id, phi->cfg->n_philosophers);
    if (phi->cfg->n_philosophers % 2 == 0
        || phi->id != phi->cfg->n_philosophers - 1)
    {
        f(phi->cfg->forks + phi->id);
        op == FORK_TAKE && philo_print(phi, "has taken a fork");
        if (phi->cfg->n_philosophers == 1)
            return (EX_OK);
        f(phi->cfg->forks + next_i);
        op == FORK_TAKE && philo_print(phi, "has taken a fork");
        return (EX_OK);
    }
    prev_i = i_prev(phi->id, phi->cfg->n_philosophers);
    f(phi->cfg->forks + prev_i);
    op == FORK_TAKE && philo_print(phi, "has taken a fork");
    f(phi->cfg->forks + phi->id);
    op == FORK_TAKE && philo_print(phi, "has taken a fork");
    return (EX_OK);        
}
static t_exit_status    eating_loop(t_philo *phi, uint64_t end_eating_ts)
{
    uint64_t    ts;

    while (1)
    {
        if (has_someone_died(phi->cfg))
            return (op_forks(phi, FORK_PUT), EX_PHILOSOPHER_DEAD);
        ts = get_time_us();
        if (ts >= phi->death_ts)
        {
            change_phi_status(phi, PHI_DIED);
            return (op_forks(phi, FORK_PUT), EX_TIMEOUT) ;
        }
        if (ts >= end_eating_ts)
        {
            phi->n_eating_counter++;
            return (op_forks(phi, FORK_PUT), EX_OK);
        }
        usleep(min(end_eating_ts - ts, DELTA_TIME_US));
    }
}

static t_exit_status  eating(t_philo *phi)
{
    uint64_t    end_eating_ts;
    uint64_t    ts;

    if (get_time_us() >= phi->death_ts)
    {
        change_phi_status(phi, PHI_DIED);
        return (EX_TIMEOUT);
    }
    op_forks(phi, FORK_TAKE);
    change_phi_status(phi, PHI_EATING);
    ts = get_time_us();
    phi->death_ts = ts + phi->cfg->time_to_die_us;
    end_eating_ts = ts + phi->cfg->time_to_eat_us;
    return (eating_loop(phi, end_eating_ts));
}

static t_exit_status    sleeping(t_philo *phi)
{
    uint64_t    end_sleeping_ts;
    uint64_t    ts;

    end_sleeping_ts = get_time_us() + phi->cfg->time_to_sleep_us;
    change_phi_status(phi, PHI_SLEEPING);
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
        if (ts >= end_sleeping_ts)
            return (EX_OK);
        usleep(min(end_sleeping_ts - ts, DELTA_TIME_US));       
    }
    return (EX_OK);
}

void *philosopher_routine(void *arg)
{
    t_philo *phi;

    phi = (t_philo *)arg;
    change_phi_status(phi, PHI_THINKING);
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
        change_phi_status(phi, PHI_THINKING);
        usleep(phi->cfg->time_to_think_us);
    }
    return (NULL);
}
