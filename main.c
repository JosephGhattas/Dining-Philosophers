/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jghattas <jghattas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 13:47:58 by jghattas          #+#    #+#             */
/*   Updated: 2025/05/30 14:24:08 by jghattas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_arg(int argc, char **argv)
{
	int	num_philos;

	if (argc != 5 && argc != 6)
		return (printf ("Enter Correct Number of Arguments\n"), 1);
	num_philos = ft_atoi(argv[1]);
	if (num_philos <= 0 || num_philos > MAX_PHILOS)
		return (printf ("Unacceptable Number of Philosophers\n"), 1);
	if (ft_atoi(argv[2]) < 0)
		return (printf ("Invalid time argument: time must be positive\n"), 1);
	if (argc == 6 && ft_atoi(argv[5]) < 0)
		return (printf ("Unacceptable value for the number of meals\n"), 1);
	return (0);
}

void	create_mutexes(pthread_mutex_t *print_mutex, pthread_mutex_t *meal_time_mutex, pthread_mutex_t *died_mutex)
{
	pthread_mutex_init(print_mutex, NULL);
	pthread_mutex_init(meal_time_mutex, NULL);
	pthread_mutex_init(died_mutex, NULL);
	
}

void	delete_mutexes(pthread_mutex_t *print_mutex, pthread_mutex_t *meal_time_mutex, pthread_mutex_t *died_mutex)
{
	pthread_mutex_destroy(print_mutex);
	pthread_mutex_destroy(meal_time_mutex);
	pthread_mutex_destroy(died_mutex);
}
void	create_threads(pthread_t *threads, t_philo *philos, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		if (pthread_create(&threads[i], NULL, philo_routine, &philos[i]) != 0)
		{
			printf("Error creating thread %d\n", i);
			exit(1);
		}
		i++;
	}
}

void	join_threads(pthread_t *threads, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		pthread_join(threads[i], NULL);
		i++;
	}
}

int	main(int argc, char **argv)
{
	pthread_t		threads[MAX_PHILOS];
	t_philo			philos[MAX_PHILOS];
	t_fork			forks[MAX_PHILOS];
	pthread_t		monitor;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	meal_time_mutex;
	pthread_mutex_t died_mutex;
	int				count;
	int				died;

	died = 0;
	if (check_arg(argc, argv) == 1)
		return (printf ("Wrong Arguments Syntax\n"));
	count = ft_atoi(argv[1]);
	create_mutexes(&print_mutex, &meal_time_mutex, &died_mutex);
	init_forks(forks, count);
	init_philos(philos, forks, count, &print_mutex, &meal_time_mutex);
	init_philo_params(philos, count, argc, argv, &died_mutex, &died);
	pthread_create(&monitor, NULL, observer, (void *)philos);
	create_threads(threads, philos, count);
	join_threads(threads, count);
	pthread_join(monitor, NULL);
	delete_mutexes(&print_mutex, &meal_time_mutex, &died_mutex);
	return (0);
}
