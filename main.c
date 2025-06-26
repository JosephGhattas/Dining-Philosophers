	/* ************************************************************************** */
	/*                                                                            */
	/*                                                        :::      ::::::::   */
	/*   main.c                                             :+:      :+:    :+:   */
	/*                                                    +:+ +:+         +:+     */
	/*   By: jghattas <jghattas@student.42.fr>          +#+  +:+       +#+        */
	/*                                                +#+#+#+#+#+   +#+           */
	/*   Created: 2025/04/24 13:47:58 by jghattas          #+#    #+#             */
	/*   Updated: 2025/06/24 12:52:32 by jghattas         ###   ########.fr       */
	/*                                                                            */
	/* ************************************************************************** */

	#include "philo.h"

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

	void	init_all(t_init *init, int argc, char **argv)
	{
		init->died = 0;
		init->count = ft_atoi(argv[1]);
		init_mutexes(init->philos, &init->mutexes, init->count);
		init_forks(init->forks, init->count);
		init_philos(init->philos, init->forks, init->count, &init->died, init->start_time);
		init_philo_params(init->philos, init->count, argc, argv);
	}

	void	start_threads(t_init	*init)
	{
		create_threads(init->threads, init->philos, init->count);
		pthread_create(&init->monitor, NULL, observer, (void *)init->philos);
		join_threads(init->threads, init->count);
		pthread_join(init->monitor, NULL);
	}

	int	main(int argc, char **argv)
	{
		t_init			init;
		struct timeval	tv;

		if (check_arg(argc, argv) != 0)
			return (printf ("Wrong Arguments Syntax\n"));
		gettimeofday(&tv, NULL);
		init.start_time = tv.tv_sec * 1000 + tv.tv_usec / 1000;;
		init_all(&init, argc, argv);
		start_threads(&init);
		destroy_mutexes(init.philos, &init.mutexes, init.count);
		return (0);
	}
