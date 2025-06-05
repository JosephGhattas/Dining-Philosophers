/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jghattas <jghattas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 13:48:12 by jghattas          #+#    #+#             */
/*   Updated: 2025/06/05 11:30:47 by jghattas         ###   ########.fr       */
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
		i++;
	}
}

void	init_philos(t_philo *philos, t_fork *forks, int count)
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
		philos[i].totalnbr = count;
		i++;
	}
}

void	init_philo_params(t_philo *philos, int count, int argc, char **argv)
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
		philos[i].died = 0;
		i++;
	}
}

void	init_mutexes(t_philo *philo, t_mutexes *mutexes, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		pthread_mutex_init(&philo[i].meal_time_mutex, NULL);
		philo[i].mutexes = mutexes;
		i++;
	}
	pthread_mutex_init(&mutexes->print_mutex, NULL);
	pthread_mutex_init(&mutexes->died_mutex, NULL);
}

void	destroy_mutexes(t_philo *philo, t_mutexes *mutexes, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		pthread_mutex_destroy(&philo[i].meal_time_mutex);
		i++;
	}
	pthread_mutex_destroy(&mutexes->print_mutex);
	pthread_mutex_destroy(&mutexes->died_mutex);
}
