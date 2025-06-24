/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jghattas <jghattas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/30 14:07:51 by jghattas          #+#    #+#             */
/*   Updated: 2025/06/24 10:58:18 by jghattas         ###   ########.fr       */
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

int	forks_lock(t_philo *philo)
{
	int	check_r;
	int	check_l;
	int	id;

	check_r = 0;
	check_l = 0;
	id = philo->id;
	if (id % 2 == 0)
	{
		if (pthread_mutex_lock(&philo->left_fork->mutex) == 0)
		{
			check_l++;
			print_state(philo, "has taken left fork");

		}
		if (pthread_mutex_lock(&philo->right_fork->mutex) == 0)
		{	
			check_r++;
			print_state(philo, "has taken right fork");;

		}
	}
	else
	{
		if (pthread_mutex_lock(&philo->right_fork->mutex) == 0)
		{	
			check_r++;
			print_state(philo, "has taken right fork");


		}
		if (pthread_mutex_lock(&philo->left_fork->mutex) == 0)
		{
			check_l++;
			print_state(philo, "has taken left fork");
		}
	}
	if ((check_l + check_r) < 2)
	{	
		if (check_r == 1)
			pthread_mutex_unlock(&philo->right_fork->mutex);
		if (check_l == 1)
			pthread_mutex_unlock(&philo->left_fork->mutex);
	}
	return (check_l + check_r);
}
void	eat(t_philo *philo)
{
	if (is_dead(philo) == -1)
		return ;
	pthread_mutex_lock(&philo->meal_time_mutex);
	philo->last_meal_time = timestamp_ms();
	usleep(100);
	pthread_mutex_unlock(&philo->meal_time_mutex);
	print_state(philo, "is eating");
	pthread_mutex_lock(&philo->meal_time_mutex);
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->meal_time_mutex);
	usleep(philo->time_to_eat * 1000);
}

void	think(t_philo *philo)
{
	if (is_dead(philo) == -1)
		return ;
	print_state(philo, "is thinking");
	usleep(100);
}

void	sleep_philo(t_philo *philo)
{
	if (is_dead(philo) == -1)
		return ;
	print_state(philo, "is sleeping");
	usleep(philo->time_to_sleep * 1000);
}
