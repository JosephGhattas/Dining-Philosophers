/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jghattas <jghattas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 09:04:02 by jghattas          #+#    #+#             */
/*   Updated: 2025/06/05 16:39:02 by jghattas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_state(t_philo *philo, const char *state)
{
	if (is_dead(philo) == -1)
		return ;
	pthread_mutex_lock(&philo->mutexes->print_mutex);
	printf("%ld ms Philospher %d %s\n", timestamp_ms(), philo->id, state);
	pthread_mutex_unlock(&philo->mutexes->print_mutex);
}

void	*observer(void *arg)
{
	t_philo	*philo;
	int		count;
	int		i;
	int		flag;

	philo = (t_philo *)arg;
	count = philo[0].totalnbr;
	while (1)
	{
		i = 0;
		while (i < count)
		{
			flag = check_dead(philo, i);
			if (flag == 1)
				break ;
			i++;
		}
		if (flag == 1)
			break ;
	}
	return (NULL);
}

void	running_philo(t_philo *philo)
{
	think(philo);
	if (is_dead(philo) == -1)
		return ;
	if (forks_lock(philo) == 2)
	{
		eat(philo);
		pthread_mutex_unlock(&philo->left_fork->mutex);
		pthread_mutex_unlock(&philo->right_fork->mutex);
	}
	sleep_philo(philo);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		if (is_dead(philo) == -1)
			break ;
		running_philo(philo);
		pthread_mutex_lock(&philo->meal_time_mutex);
		if (philo->meals_goal > 0 && philo->meals_eaten >= philo->meals_goal)
		{
			pthread_mutex_unlock(&philo->meal_time_mutex);
			break ;
		}
		pthread_mutex_unlock(&philo->meal_time_mutex);
	}
	return (NULL);
}
