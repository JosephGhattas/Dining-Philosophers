/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jghattas <jghattas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 14:32:50 by jghattas          #+#    #+#             */
/*   Updated: 2025/06/07 08:16:36 by jghattas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	ft_atoi(const char *str)
{
	int	sign;
	int	result;
	int	i;

	i = 0;
	sign = 1;
	result = 0;
	while (str[i] && (str[i] == 32 || (str[i] >= 9 && str[i] <= 13)))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9' && str[i] != '\0')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	if (str[i] != '\0')
		return (-1);
	return (result * sign);
}

int	check_arg(int argc, char **argv)
{
	int	num_philos;

	if (argc < 5 || argc > 6)
		return (printf ("Enter Correct Number of Arguments\n"), 1);
	num_philos = ft_atoi(argv[1]);
	if (num_philos <= 1 || num_philos > MAX_PHILOS)
		return (printf ("Unacceptable Number of Philosophers\n"), 1);
	if (ft_atoi(argv[2]) < 0)
		return (printf ("Invalid time argument: time must be positive\n"), 1);
	if (argc == 6 && (ft_atoi(argv[5]) < 0))
		return (printf ("Unacceptable value for the number of meals\n"), 1);
	if (ft_atoi(argv[3]) < 0 || ft_atoi(argv[4]) < 0)
		return (1);
	return (0);
}

int	kill_philo(t_philo *philo, int i, size_t curr_time)
{
	*(philo->died) = 1;
	pthread_mutex_lock(&philo[i].mutexes->print_mutex);
	printf("%ld ms philosopher %d died\n", curr_time, philo[i].id);
	pthread_mutex_unlock(&philo[i].mutexes->print_mutex);
	pthread_mutex_unlock(&philo[i].mutexes->died_mutex);
	return (1);
}

int	check_dead(t_philo *philo, int i)
{
	size_t	curr_time;
	size_t	time_since_meal;

	curr_time = timestamp_ms();
	pthread_mutex_lock(&philo[i].meal_time_mutex);
	time_since_meal = curr_time - philo[i].last_meal_time;
	pthread_mutex_unlock(&philo[i].meal_time_mutex);
	if (time_since_meal > philo[i].time_to_die)
	{
		pthread_mutex_lock(&philo[i].mutexes->died_mutex);
		if ((*(philo->died)) == 0)
			return (kill_philo(philo, i, curr_time));
		pthread_mutex_unlock(&philo[i].mutexes->died_mutex);
	}
	pthread_mutex_lock(&philo[i].meal_time_mutex);
	if (philo->meals_goal > 0 && philo->meals_eaten >= philo->meals_goal)
	{
		pthread_mutex_unlock(&philo->meal_time_mutex);
		return (1);
	}
	pthread_mutex_unlock(&philo[i].meal_time_mutex);
	return (0);
}

int	is_dead(t_philo *philo)
{
	pthread_mutex_lock(&philo->mutexes->died_mutex);
	if (*(philo->died) == 1)
	{
		pthread_mutex_unlock(&philo->mutexes->died_mutex);
		return (-1);
	}
	pthread_mutex_unlock(&philo->mutexes->died_mutex);
	return (0);
}
