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

void smart_sleep(long duration_us, t_philo *philo) {
    long start = timestamp_ms() * 1000;  // ms → μs
    long elapsed;
    
    while ((elapsed = (timestamp_ms() * 1000 - start)) < duration_us) {
        usleep(500);  // Half the check interval
        pthread_mutex_lock(philo->died_mutex);
        if (*philo->died) {
            pthread_mutex_unlock(philo->died_mutex);
            return;
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
}

void	think(t_philo *philo)
{
	 pthread_mutex_lock(philo->died_mutex);
    if (*philo->died) {
        pthread_mutex_unlock(philo->died_mutex);
        return;
    }
    pthread_mutex_unlock(philo->died_mutex);
	print_state(philo, "is thinking");
	usleep(100);
}

void	sleep_philo(t_philo *philo)
{
	print_state(philo, "is sleeping");
	smart_sleep(philo->time_to_sleep, philo);
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
