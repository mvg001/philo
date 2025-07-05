/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils_2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvassall <mvassall@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 13:43:51 by mvassall          #+#    #+#             */
/*   Updated: 2025/07/05 16:41:31 by mvassall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>

int i_next(int n, int modulus)
{
    return ((n + 1) % modulus);
}

int i_prev(int n, int modulus)
{
    return ((n + modulus - 1) % modulus);
}

char	*get_philo_status_msg(t_philo_status status)
{
    if (status == GOT_A_FORK)
        return ("has taken a fork");
    else if (status == EATING)
        return ("is eating");
    else if (status == SLEEPING)
        return ("is sleeping");
    else if (status == THINKING)
        return ("is thinking");
    else if (status == DIED)
        return ("died");
    return ("invalid status");
}

/**
  returns time stamp in micro seconds from simulation start
*/
uint64_t    get_ts_us(t_cfg_philo *cfg)
{
    return ((get_time_us() - cfg->start_ts));
}

void    philo_print(t_philo *p)
{
    if (p == NULL)
        return ;
    if (pthread_mutex_lock(&p->cfg->print_mutex) != 0)
        return ;
    printf("%ld %d %s\n", get_ts_us(p->cfg) / 1000, p->id,
        get_philo_status_msg(p->status));
    if (p->status == GOT_A_FORK)
        printf("%ld %d %s\n", get_ts_us(p->cfg) / 1000, p->id,
            get_philo_status_msg(p->status));
    pthread_mutex_unlock(&p->cfg->print_mutex);
}
