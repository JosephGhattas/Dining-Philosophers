/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jghattas <jghattas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 09:04:02 by jghattas          #+#    #+#             */
/*   Updated: 2025/05/30 14:28:23 by jghattas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_state(t_philo *philo, const char *state)
{
	pthread_mutex_lock(philo->died_mutex);
    if (*philo->died) {
        pthread_mutex_unlock(philo->died_mutex);
        return;
    }
    pthread_mutex_unlock(philo->died_mutex);
	pthread_mutex_lock(philo->print_mutex);
	printf("%ld ms Philospher %d %s\n", timestamp_ms(), philo->id, state);
	pthread_mutex_unlock(philo->print_mutex);
}

void *observer(void *arg)
{
	t_philo *philo = (t_philo *)arg;
	long curr_time;
	const int count = philo[0].totalnbr;
	int i;
	size_t time_since_meal;

	while (1)
	{
		usleep(1000);
		i = 0;
		while (i < count)
		{
			pthread_mutex_lock(philo[i].meal_time_mutex);
			curr_time = timestamp_ms();
			time_since_meal = curr_time - philo[i].last_meal_time;
			if (time_since_meal > philo[i].time_to_die)
			{
				pthread_mutex_lock(philo[i].died_mutex);
				if (*philo[i].died == 0)
                {
                    *philo[i].died = 1;
                    pthread_mutex_lock(philo[i].print_mutex);
                    printf("%ld %d died\n", curr_time, philo[i].id);
                    pthread_mutex_unlock(philo[i].print_mutex);
                }
				pthread_mutex_unlock(philo[i].died_mutex);
				pthread_mutex_unlock(philo[i].meal_time_mutex);
				return (NULL);
			}
			pthread_mutex_unlock(philo[i].meal_time_mutex);
			i++;
		}
	}
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
		pthread_mutex_lock(philo->died_mutex);
		if (*(philo->died) == 1)
		{
			pthread_mutex_unlock(philo->died_mutex);
			break;
		}
		pthread_mutex_unlock(philo->died_mutex);
		running_philo(philo);
		pthread_mutex_lock(philo->meal_time_mutex);
		if (philo->meals_goal > 0 && philo->meals_eaten >= philo->meals_goal)
		{
			pthread_mutex_unlock(philo->meal_time_mutex);
			break;
		}
		pthread_mutex_unlock(philo->meal_time_mutex);
	}
	return (NULL);
} 
