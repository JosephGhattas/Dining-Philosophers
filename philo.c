/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jghattas <jghattas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 09:04:02 by jghattas          #+#    #+#             */
/*   Updated: 2025/05/16 12:20:04 by jghattas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	print_state(t_philo *philo, const char *state)
{
	pthread_mutex_lock(philo->print_mutex);
	printf("%ld ms Philospher %d %s\n", timestamp_ms(), philo->id, state);
	pthread_mutex_unlock(philo->print_mutex);
}
void *observer(void *arg)
{
	t_philo *philo = (t_philo *)arg;
	size_t curr_time;
	int i;

	while (1)
	{
		i = 0;
		while (i < philo[0].totalnbr)
		{
			pthread_mutex_lock(philo[i].meal_time_mutex);
			curr_time = timestamp_ms();
			if ((curr_time - philo[i].last_meal_time) >= (size_t)philo[i].time_to_die)
			{
				printf("Observer running: checking philo %d\n", i + 1);
				pthread_mutex_lock(philo[i].died_mutex);
				pthread_mutex_lock(philo[i].print_mutex);
				printf("%ld  %d died\n", (long)curr_time, philo[i].id);
				pthread_mutex_unlock(philo[i].print_mutex);
				*(philo[i].died) = 1;
				pthread_mutex_unlock(philo[i].meal_time_mutex);
				pthread_mutex_unlock(philo[i].died_mutex);
				return (NULL);
			}
			pthread_mutex_unlock(philo[i].meal_time_mutex);
			i++;
		}
		usleep(1000);
	}
}



void	get_fork(t_philo *philo, t_fork *fork)
{
	if (fork->dirty == 1 && &fork->owner != &philo->id)
		fork->owner = philo->id;
}

void	check_forks(t_philo	*philo)
{
	int	left_ready;
	int	right_ready;

	while (1)
	{
		pthread_mutex_lock(&philo->left_fork->mutex);
		if (philo->left_fork->owner == philo->id)
			left_ready = 1;
		else if (philo->left_fork->dirty == 1)
			get_fork(philo, philo->left_fork);
		pthread_mutex_unlock(&philo->left_fork->mutex);
		pthread_mutex_lock(&philo->right_fork->mutex);
		if (philo->right_fork->owner == philo->id)
			right_ready = 1;
		else if (philo->right_fork->dirty == 1)
			get_fork(philo, philo->right_fork);
		pthread_mutex_unlock(&philo->right_fork->mutex);
		if (left_ready == 1 && right_ready == 1)
		{
			print_state(philo, "has both forks");
			right_ready = 0;
			left_ready = 0;
			break ;
		}
	}
}

void	eat(t_philo *philo)
{
	pthread_mutex_lock(philo->meal_time_mutex);
	philo->last_meal_time = timestamp_ms();
	print_state(philo, "is eating");
	usleep(philo->time_to_eat);
	philo->meals_eaten++;
	philo->left_fork->dirty = 1;
	philo->right_fork->dirty = 1;
	pthread_mutex_unlock(philo->meal_time_mutex);
}

void	think(t_philo *philo)
{
	print_state(philo, "is thinking");
	usleep(100);
}

void	sleep_philo(t_philo *philo)
{
	print_state(philo, "is sleeping");
	usleep(philo->time_to_sleep);
}

void	running_philo(t_philo *philo)
{
	think(philo);
	check_forks(philo);
	pthread_mutex_lock(&philo->right_fork->mutex);
	pthread_mutex_lock(&philo->left_fork->mutex);
	eat(philo);
	pthread_mutex_unlock(&philo->right_fork->mutex);
	pthread_mutex_unlock(&philo->left_fork->mutex);
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
