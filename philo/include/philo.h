/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvassall <mvassall@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 11:01:43 by mvassall          #+#    #+#             */
/*   Updated: 2025/07/05 19:07:06 by mvassall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdint.h>
# include <sys/time.h>
# include <pthread.h>

const int  delta_time_us = 5000; // 5 ms

typedef enum e_philo_status
{
    GOT_A_FORK,
    EATING,
    SLEEPING,
    THINKING,
    DIED,
}   t_philo_status;

typedef enum e_arg_name
{
    N_PHILOSOPHERS = 0,
    TIME_TO_DIE,
    TIME_TO_EAT,
    TIME_TO_SLEEP,
    N_EATING_ROUNDS,
}   t_arg_name;

struct s_philo ;

struct s_cfg_philo
{
    int n_philosophers;
    int time_to_die_us;        // usec
    int time_to_eat_us;        // usec
    int time_to_sleep_us;      // usec
    int n_eating_rounds;
    pthread_mutex_t print_mutex;
    pthread_mutex_t m_dead_counter;
    int dead_counter;
    uint64_t    start_ts;       // usec
    struct s_philo *philos; // array of philosophers
};
typedef struct s_cfg_philo    t_cfg_philo;

struct s_philo
{
    pthread_t       thread;
    int             id;
    pthread_mutex_t mtx;
    uint64_t        death_ts;      // usec
    t_philo_status  status;
    int             n_eating_counter;
    t_cfg_philo           *cfg;    
};
typedef struct s_philo  t_philo;

// philo_utils_1
int	ft_is_int(char *str);
int	ft_atoi(const char *str);
int	check_args(int ac, char **av);
int         get_args(int ac, char **av, int *values);
uint64_t	get_time_us(void);

// philo_utils_2
int i_next(int n, int modulus);
int i_prev(int n, int modulus);
char    *get_philo_status_msg(t_philo_status status);
uint64_t    get_ts_us(t_cfg_philo *cfg);
void    philo_print(t_philo *p);

// philo_utils_3
t_cfg_philo *init_cfg_philo(int *args);
void    start_all_philos(t_cfg_philo *cfg);
int has_someone_died(t_cfg_philo *cfg);
void    increment_dead_counter(t_cfg_philo *cfg);

// philo_1.c
void *philosopher_routine(void *args);
#endif