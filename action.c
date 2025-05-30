/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jghattas <jghattas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 14:07:51 by jghattas          #+#    #+#             */
/*   Updated: 2025/05/30 18:57:26 by jghattas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	get_fork(t_philo *philo, t_fork *fork)
{
	if (fork->dirty == 1 && fork->owner != philo->id)
		fork->owner = philo->id;
}

void	check_forks(t_philo	*philo)
{
	int	left_ready;
	int	right_ready;

	left_ready = 0;
	right_ready = 0;
	while (1)
	{
		pthread_mutex_lock(philo->died_mutex);
        if (*philo->died) {
            pthread_mutex_unlock(philo->died_mutex);
            return;
        }
        pthread_mutex_unlock(philo->died_mutex);
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

void	smart_sleep(long duration_us, t_philo *philo)
{
    long start = timestamp_ms();
    long elapsed;
    
    while ((elapsed = timestamp_ms() - start) < duration_us)
	{
        pthread_mutex_lock(philo->died_mutex);
        if (*philo->died)
		{
            pthread_mutex_unlock(philo->died_mutex);
            return ;
        }
        pthread_mutex_unlock(philo->died_mutex);
    }
}

void	eat(t_philo *philo)
{
	pthread_mutex_lock(philo->meal_time_mutex);
	philo->last_meal_time = timestamp_ms();
	pthread_mutex_unlock(philo->meal_time_mutex);
	print_state(philo, "is eating");
	pthread_mutex_lock(philo->meal_time_mutex);
	philo->meals_eaten++;
	pthread_mutex_unlock(philo->meal_time_mutex);
	philo->left_fork->dirty = 1;
	philo->right_fork->dirty = 1;
	smart_sleep(philo->time_to_eat, philo);
	pthread_mutex_lock(philo->meal_time_mutex);
	philo->last_meal_time = timestamp_ms();
	pthread_mutex_unlock(philo->meal_time_mutex);
}

void	think(t_philo *philo)
{
	pthread_mutex_lock(philo->died_mutex);
    if (*philo->died)
	{
        pthread_mutex_unlock(philo->died_mutex);
        return ;
    }
    pthread_mutex_unlock(philo->died_mutex);
	print_state(philo, "is thinking");
}

void	sleep_philo(t_philo *philo)
{
	print_state(philo, "is sleeping");
	smart_sleep(philo->time_to_sleep, philo);
}