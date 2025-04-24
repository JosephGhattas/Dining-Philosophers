/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jghattas <jghattas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/24 13:47:58 by jghattas          #+#    #+#             */
/*   Updated: 2025/04/24 13:48:00 by jghattas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_arg(int argc, char **argv)
{
	int	num_philos;

	if (argc != 5 && argc != 6)
		return(printf("Enter Correct Number of Arguments\n"), 1);
	num_philos = ft_atoi(argv[1]);
	if (num_philos <= 0 || num_philos > MAX_PHILOS)
        return (printf("Unacceptable Number of Philosophers\n"), 1);
	if (ft_atoi(argv[2]) < 0)
		return (printf("Invalid time argument: time must be positive\n"), 1);
	if (argc == 6 && ft_atoi(argv[5]) < 0)
    	return (printf("Unacceptable value for the number of meals\n"), 1);
	printf("argc check ok");
	return (0);
}

int main(int argc, char **argv) {
    int i;
    int count;

    i = 0;
	printf("argc check");
	if (check_arg(argc, argv) == 1)
		return (printf("Wrong Arguments Syntax\n"));
	printf("argc check ok");
	pthread_t threads[MAX_PHILOS];
    t_philo philos[MAX_PHILOS];
    t_fork forks[MAX_PHILOS];
    pthread_mutex_t print_mutex;
	count = ft_atoi(argv[1]);
    pthread_mutex_init(&print_mutex, NULL);
    init_forks(forks, count);
    init_philos(philos, forks, count, &print_mutex);

    while (i < count)
    {
        if (pthread_create(&threads[i], NULL, philo_routine, &philos[i]) != 0)
		{
			printf("Error creating thread %d\n", i);
			return (1);
		}
        i++;
    }
    i = 0;
    while (i < count)
    {    
        pthread_join(threads[i], NULL);
        i++;
    }
	pthread_mutex_destroy(&print_mutex);
    return 0;
}
