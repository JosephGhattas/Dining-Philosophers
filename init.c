/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jghattas <jghattas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 13:48:12 by jghattas          #+#    #+#             */
/*   Updated: 2025/04/24 14:35:57 by jghattas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long timestamp_ms(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void init_forks(t_fork *forks, int count)
{
	int	i;

	i = 0;
    while (i < count)
	{
        pthread_mutex_init(&forks[i].mutex, NULL);
        forks[i].owner = i + 1;
        forks[i].dirty = 0;
		i++;
	}
	if (i == count)
		forks[i].dirty = 1;
}

void init_philos(t_philo *philos, t_fork *forks, int count, pthread_mutex_t *print_mutex)
{
	int	i;

	i = 0;
    while (i < count)
	{
        philos[i].id = i + 1;
        philos[i].last_meal_time = timestamp_ms();
        philos[i].meals_eaten = 0;
        philos[i].left_fork = &forks[i];
        philos[i].right_fork = &forks[(i + 1) % count];
        philos[i].print_mutex = print_mutex;
		i++;
	}
}
