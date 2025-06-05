/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jghattas <jghattas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 14:07:51 by jghattas          #+#    #+#             */
/*   Updated: 2025/06/05 16:41:20 by jghattas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	timestamp_ms(void)
{
	static long		start_time = 0;
	long			current_time;
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	current_time = tv.tv_sec * 1000 + tv.tv_usec / 1000;
	if (start_time == 0)
		start_time = current_time;
	return (current_time - start_time);
}

void	eat(t_philo *philo)
{
	if (is_dead(philo) == -1)
		return ;
	pthread_mutex_lock(&philo->meal_time_mutex);
	philo->last_meal_time = timestamp_ms();
	pthread_mutex_unlock(&philo->meal_time_mutex);
	print_state(philo, "is eating");
	pthread_mutex_lock(&philo->meal_time_mutex);
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->meal_time_mutex);
	usleep(philo->time_to_eat * 1000);
	pthread_mutex_lock(&philo->meal_time_mutex);
	philo->last_meal_time = timestamp_ms();
	pthread_mutex_unlock(&philo->meal_time_mutex);
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
	usleep(philo->time_to_sleep * 1000);
}
