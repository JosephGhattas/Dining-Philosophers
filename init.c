/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jghattas <jghattas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 13:48:12 by jghattas          #+#    #+#             */
/*   Updated: 2025/06/05 16:34:36 by jghattas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

void	init_philos(t_init *init)
{
	int	i;

	i = 0;
	while (i < init->count)
	{
		init->philos[i].id = i + 1;
		init->philos[i].start_time = init->start_time;
		init->philos[i].last_meal_time = (timestamp_ms() - init->start_time);
		init->philos[i].meals_eaten = 0;
		init->philos[i].left_fork = &init->forks[i];
		init->philos[i].right_fork = &init->forks[(i + 1) % init->count];
		init->philos[i].totalnbr = init->count;
		init->philos[i].died = &init->died;
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
