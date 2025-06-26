/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jghattas <jghattas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 11:49:09 by jghattas          #+#    #+#             */
/*   Updated: 2025/06/24 13:33:44 by jghattas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	hungriest(t_philo *philo)
{
	long	curr_time;
	int		i;
	long	max_diff;
	int		hungry_philo;

	i = 0;
	curr_time = (timestamp_ms() - philo->start_time);
	pthread_mutex_lock(&philo[0].meal_time_mutex);
	max_diff = curr_time - philo[0].last_meal_time;
	pthread_mutex_unlock(&philo[0].meal_time_mutex);
	hungry_philo = philo[0].id;
	while (i < philo[0].totalnbr)
	{
		pthread_mutex_lock(&philo[i].meal_time_mutex);
		if(max_diff < (curr_time - philo[i].last_meal_time))
		{
			max_diff = curr_time - philo[i].last_meal_time;
			(hungry_philo = philo[i].id);
		}
		pthread_mutex_unlock(&philo[i].meal_time_mutex);
		i++;
	}
	return (hungry_philo);
}

int	even_forks(t_philo *philo)
{
	int	check_r;
	int	check_l;

	check_r = 0;
	check_l = 0;
	if (pthread_mutex_lock(&philo->left_fork->mutex) == 0)
	{
		check_l++;
		print_state(philo, "has taken left fork");
	}
	if (pthread_mutex_lock(&philo->right_fork->mutex) == 0)
	{	
		check_r += 2;
		print_state(philo, "has taken right fork");
	}
	return (check_l + check_r);
}

int	odd_forks(t_philo *philo)
{
	int	check_r;
	int	check_l;

	check_r = 0;
	check_l = 0;
	if (pthread_mutex_lock(&philo->right_fork->mutex) == 0)
	{	
		check_r += 2;
		print_state(philo, "has taken right fork");
	}
	if (pthread_mutex_lock(&philo->left_fork->mutex) == 0)
	{
		check_l++;
		print_state(philo, "has taken left fork");
	}
	return (check_l + check_r);
}

int	forks_lock(t_philo *philo)
{
	int	locked;

	if (hungriest(philo) != philo->id)
		usleep(500);
	locked = 0;
	if (philo->id % 2 == 0)
		locked = even_forks(philo);
	else
		locked = odd_forks(philo);
	if (locked < 3)
	{	
		if (locked == 2)
			pthread_mutex_unlock(&philo->right_fork->mutex);
		if (locked == 1)
			pthread_mutex_unlock(&philo->left_fork->mutex);
		// usleep(200);
	}
	return (locked);
}



