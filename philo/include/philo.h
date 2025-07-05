/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvassall <mvassall@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/05 11:01:43 by mvassall          #+#    #+#             */
/*   Updated: 2025/07/05 11:22:28 by mvassall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdint.h>
# include <sys/time.h>
# include <stdlib.h>

// philo_utils_1
int	ft_is_int(char *str);
int	ft_atoi(const char *str);
int	check_args(int ac, char **av);
int         get_args(int ac, char **av, int *values);
uint64_t	get_time_us(void);


#endif