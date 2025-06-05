/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jghattas <jghattas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/08 14:32:50 by jghattas          #+#    #+#             */
/*   Updated: 2025/06/05 15:35:53 by jghattas         ###   ########.fr       */
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
        {
        	*(philo->died) = 1;
            pthread_mutex_lock(&philo[i].mutexes->print_mutex);
        	printf("%ld ms philosopher %d died\n", curr_time, philo[i].id);
        	pthread_mutex_unlock(&philo[i].mutexes->print_mutex);
			pthread_mutex_unlock(&philo[i].mutexes->died_mutex);
			return (1);
    	}
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
