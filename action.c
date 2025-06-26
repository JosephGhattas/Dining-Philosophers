/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jghattas <jghattas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 14:07:51 by jghattas          #+#    #+#             */
/*   Updated: 2025/06/24 11:49:38 by jghattas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	timestamp_ms(void)
{
	long			current_time;
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	current_time = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	return (current_time);
}

void	smart_sleep(size_t time_in_ms)
{
	long	start;
	long	now;

	start = timestamp_ms();
	while (1)
	{
		now = timestamp_ms();
		if ((now - start) >= (long)time_in_ms)
			break ;
		usleep(500);
	}
}

void	eat(t_philo *philo)
{
	if (is_dead(philo) == -1)
		return ;
	pthread_mutex_lock(&philo->meal_time_mutex);
	philo->last_meal_time = (timestamp_ms() - philo->start_time);
	pthread_mutex_unlock(&philo->meal_time_mutex);
	print_state(philo, "is eating");
	pthread_mutex_lock(&philo->meal_time_mutex);
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->meal_time_mutex);
	smart_sleep(philo->time_to_eat);
}

void	think(t_philo *philo)
{
	if (is_dead(philo) == -1)
		return ;
	print_state(philo, "is thinking");
}

void	sleep_philo(t_philo *philo)
{
	if (is_dead(philo) == -1)
		return ;
	print_state(philo, "is sleeping");
	smart_sleep(philo->time_to_sleep);
}
