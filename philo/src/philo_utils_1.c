/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_utils_1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mvassall <mvassall@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/30 13:43:55 by mvassall          #+#    #+#             */
/*   Updated: 2025/07/05 11:21:28 by mvassall         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_is_int(char *str)
{
	unsigned int	count_digits;

	if (str == NULL)
		return (0);
	while (*str && (*str == ' ' || *str == '\t'))
		str++;
	if (*str && (*str == '-' || *str == '+'))
		str++;
	count_digits = 0;
	while (*str && ('0' <= *str && *str <= '9'))
	{
		count_digits++;
		str++;
	}
	while (*str && (*str == ' ' || *str == '\t'))
		str++;
	if (*str != '\0')
		return (0);
	return (count_digits <= 10);
}

int	ft_atoi(const char *str)
{
	int	val;
	int	is_positive;

	while (*str && (*str == ' ' || *str == '\t'))
		str++;
	if (*str == '\0')
		return (0);
	is_positive = 1;
	if (*str == '+' || *str == '-')
	{
		is_positive = (*str != '-');
		str++;
	}
	val = 0;
	while (*str && ('0' <= *str && *str <= '9'))
		val = (val * 10) - (*str++ - '0');
	if (is_positive)
		return (-val);
	return (val);
}

int	check_args(int ac, char **av)
{
	if (ac != 5 && ac != 6)
		return (0);
	if (!ft_is_int(av[1]) || !ft_is_int(av[2]) || !ft_is_int(av[3])
		|| !ft_is_int(av[4]))
		return (0);
	if (ac == 6 && !ft_is_int(av[5]))
		return (0);
	return (1);
}

int	get_args(int ac, char **av, int *values)
{
	values[0] = ft_atoi(av[1]);
	values[1] = ft_atoi(av[2]);
	values[2] = ft_atoi(av[3]);
	values[3] = ft_atoi(av[4]);
	if (ac == 6)
	{
		values[4] = ft_atoi(av[5]);
		if (values[4] < 0)
			return (0);
	}
	if (values[0] < 0 || values[1] < 0 || values[2] < 0 || values[3] < 0)
		return (0);
	return (1);
}

uint64_t	get_time_us(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
			return 0;
	return ((tv.tv_sec * (uint64_t)1000000) + tv.tv_usec);
}

