/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvassall <mvassall@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 11:01:43 by mvassall          #+#    #+#             */
/*   Updated: 2025/07/09 17:38:12 by mvassall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdint.h>
# include <sys/time.h>
# include <pthread.h>

# define DELTA_TIME_US 5000 // 5 ms

typedef enum e_exit_status
{
    EX_OK,
    EX_PHILOSOPHER_DEAD,
    EX_TIMEOUT,
}   t_exit_status;

typedef enum e_philo_status
{
    PHI_WAIT,
    PHI_EATING,
    PHI_SLEEPING,
    PHI_THINKING,
    PHI_DIED,
    PHI_FINISH,
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

typedef struct s_exit
{
    int id;
    t_exit_status   ex_status;
} t_exit;

struct s_cfg_philo
{
    int n_philosophers;
    int time_to_die_us;        // usec
    int time_to_eat_us;        // usec
    int time_to_sleep_us;      // usec
    int time_to_think_us;      // usec
    int n_eating_rounds;
    uint64_t    start_ts;       // usec
    pthread_mutex_t print_mutex;
    pthread_mutex_t m_dead_counter;
    int dead_counter;
    struct s_philo *philos; // array of philosophers
    pthread_mutex_t *forks;
};
typedef struct s_cfg_philo    t_cfg_philo;

struct s_philo
{
    t_cfg_philo     *cfg;
    pthread_t       thread;
    int             id;
    uint64_t        death_ts;      // usec
    int             n_eating_counter;
    t_philo_status  status;
};
typedef struct s_philo  t_philo;

typedef enum e_op_forks
{
    FORK_TAKE,
    FORK_PUT,
} t_op_forks;
// philo_utils_1
int	ft_is_int(char *str);
int	ft_atoi(const char *str);
int         get_args(int ac, char **av, int *values);
uint64_t	get_time_us(void);
void	*ft_calloc(size_t n_members, size_t size);

// philo_utils_2
int i_next(int i, int modulus);
int i_prev(int i, int modulus);
uint64_t    get_ts_us(t_cfg_philo *cfg);
int    philo_print(t_philo *p, char *message);
int  min(uint64_t a, uint64_t b);

// philo_utils_3
t_cfg_philo *init_cfg_philo(int *args);
void    start_all_philos(t_cfg_philo *cfg);
int has_someone_died(t_cfg_philo *cfg);
void    increment_dead_counter(t_cfg_philo *cfg);


// philo_utils_4
int change_phi_status(t_philo *phi, t_philo_status new_status);
//static int  check_candidate(t_cfg_philo *cfg, int i);
int get_next_to_eat(t_cfg_philo *cfg);
void clean_up(t_cfg_philo *cfg);

// philosopher.c
void *philosopher_routine(void *arg);

// monitor.c
void monitoring(t_cfg_philo *cfg);
#endif