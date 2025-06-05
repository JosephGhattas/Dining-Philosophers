/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jghattas <jghattas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 09:04:02 by jghattas          #+#    #+#             */
/*   Updated: 2025/06/05 11:21:20 by jghattas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_state(t_philo *philo, const char *state)
{
	pthread_mutex_lock(&philo->mutexes->died_mutex);
	if (philo->died == 1)
	{
		pthread_mutex_unlock(&philo->mutexes->died_mutex);
		return ;
	}
	pthread_mutex_unlock(&philo->mutexes->died_mutex);
	pthread_mutex_lock(&philo->mutexes->print_mutex);
	printf("%ld ms Philospher %d %s\n", timestamp_ms(), philo->id, state);
	pthread_mutex_unlock(&philo->mutexes->print_mutex);
}

void *observer(void *arg)
{
	t_philo *philo = (t_philo *)arg;
	const int count = philo[0].totalnbr;
	int i;
	int	flag;

	while (1)
	{
		usleep(1000);
		i = 0;
		while (i < count)
		{
			pthread_mutex_lock(&philo[i].meal_time_mutex);
			flag = check_dead(philo, i);
			pthread_mutex_unlock(&philo[i].meal_time_mutex);
			if (flag == 1)
				break;
			i++;
		}
		if (flag == 1)
			break;
	}
	return(NULL);
}

void	running_philo(t_philo *philo)
{
	think(philo);
	check_forks(philo);
	if (philo->id % 2 == 0)
	{
    	pthread_mutex_lock(&philo->left_fork->mutex);
    	pthread_mutex_lock(&philo->right_fork->mutex);
	}
	else
	{
		pthread_mutex_lock(&philo->right_fork->mutex);
    	pthread_mutex_lock(&philo->left_fork->mutex);
	}
	eat(philo);
	pthread_mutex_unlock(&philo->left_fork->mutex);
	pthread_mutex_unlock(&philo->right_fork->mutex);
	sleep_philo(philo);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	while (1)
	{
		running_philo(philo);
		pthread_mutex_lock(&philo->mutexes->died_mutex);
		if ((philo->died) == 1)
		{
			pthread_mutex_unlock(&philo->mutexes->died_mutex);
			break;
		}
		pthread_mutex_unlock(&philo->mutexes->died_mutex);
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
