/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 11:13:17 by marvin            #+#    #+#             */
/*   Updated: 2025/04/23 11:13:17 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "philo.h"

int main(int argc, char **argv) {
    int i;
    int count;

    count = 5;
    i = 0;
    pthread_t threads[MAX_PHILOS];
    t_philo philos[MAX_PHILOS];
    t_fork forks[MAX_PHILOS];
    pthread_mutex_t print_mutex;

    if (argc == 2)
        count = atoi(argv[1]);

    pthread_mutex_init(&print_mutex, NULL);
    init_forks(forks, count);
    init_philos(philos, forks, count, &print_mutex);

    while (i < count)
    {
        pthread_create(&threads[i], NULL, philo_routine, &philos[i]);
        i++;
    }
    i = 0;
    while (i < count)
    {    
        pthread_join(threads[i], NULL);
        i++;
    }
    return 0;
}

