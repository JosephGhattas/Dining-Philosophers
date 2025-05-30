/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jghattas <jghattas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 13:48:12 by jghattas          #+#    #+#             */
/*   Updated: 2025/05/30 18:57:42 by jghattas         ###   ########.fr       */
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

void	init_forks(t_fork *forks, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		pthread_mutex_init(&forks[i].mutex, NULL);
		forks[i].owner = i + 1;
		forks[i].dirty = 1;
		i++;
	}
}

void	init_philos(t_philo *philos, t_fork *forks, int count,
	pthread_mutex_t *print_mutex, pthread_mutex_t *meal_time_mutex)
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
		philos[i].meal_time_mutex = meal_time_mutex;
		philos[i].totalnbr = count;
		i++;
	}
}

void	init_philo_params(t_philo *philos, int count, int argc, char **argv, pthread_mutex_t *died_mutex, int *died)
{
	int		i;
	size_t	time_to_die;
	size_t	time_to_eat;
	size_t	time_to_sleep;
	size_t	meals_goal;

	time_to_die = ft_atoi(argv[2]);
	time_to_eat = ft_atoi(argv[3]);
	time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		meals_goal = ft_atoi(argv[5]);
	else
		meals_goal = 0;
	i = 0;
	while (i < count)
	{
		philos[i].time_to_die = time_to_die;
		philos[i].time_to_eat = time_to_eat;
		philos[i].time_to_sleep = time_to_sleep;
		philos[i].meals_goal = meals_goal;
		philos[i].died = died;
		philos[i].died_mutex = died_mutex;
		i++;
	}
}
