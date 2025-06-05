/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jghattas <jghattas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 14:07:51 by jghattas          #+#    #+#             */
/*   Updated: 2025/06/05 15:46:09 by jghattas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// void	smart_sleep(long duration, t_philo *philo)
// {
//     long start = timestamp_ms();
//     long elapsed;
    
	
// 	if (is_dead(philo) == -1)
// 		return ;
//     while ((elapsed = timestamp_ms() - start) < duration)
// 	{
// 		// printf("\n\n\ndurrr = %ld\n\n\n", timestamp_ms() - start);
//         pthread_mutex_lock(&philo->mutexes->died_mutex);
//         if (philo->died)
// 		{
//             pthread_mutex_unlock(&philo->mutexes->died_mutex);
//             return ;
//         }
//         pthread_mutex_unlock(&philo->mutexes->died_mutex);
//     }
// }

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
	// smart_sleep(philo->time_to_eat, philo);
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
	// smart_sleep(philo->time_to_sleep, philo);
}