/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jghattas <jghattas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/24 11:49:09 by jghattas          #+#    #+#             */
/*   Updated: 2025/06/24 12:45:53 by jghattas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
		usleep(100);
	}
	return (locked);
}
