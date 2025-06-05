/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jghattas <jghattas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/16 09:46:27 by jghattas          #+#    #+#             */
/*   Updated: 2025/06/05 11:02:03 by jghattas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>

# define MAX_PHILOS 250


typedef struct s_fork
{
	pthread_mutex_t	mutex;
	int				owner;			// philosopher id
	int				dirty;			// 1 if dirty, 0 if clean
}	t_fork;

typedef struct s_mutexes
{
	pthread_mutex_t died_mutex;
	pthread_mutex_t	print_mutex;
}	t_mutexes;

typedef struct s_philo
{
	int				id;
	long			last_meal_time;			
	t_fork			*left_fork;
	t_fork			*right_fork;
	t_mutexes		*mutexes;
	pthread_mutex_t meal_time_mutex;
	size_t			time_to_die;
	size_t			time_to_eat;
	size_t			time_to_sleep;
	size_t			meals_goal;
	size_t			meals_eaten;
	int				totalnbr;
	int				died;
}	t_philo;

int		check_arg(int argc, char **argv);
long	timestamp_ms(void);
void	init_forks(t_fork *forks, int count);
void	init_philos(t_philo *philos, t_fork *forks, int count);
void	init_philo_params(t_philo *philos, int count, int argc, char **argv);
void	init_mutexes(t_philo *philo, t_mutexes *mutexes, int count);
int		ft_atoi(const char *str);
void	*philo_routine(void *arg);
void	print_state(t_philo *philo, const char *state);
void	maybe_give_fork(t_philo *philo, t_fork *fork);
void	*observer(void *arg);

void	get_fork(t_philo *philo, t_fork *fork);
void	check_forks(t_philo	*philo);
void	smart_sleep(long duration_us, t_philo *philo);
void	eat(t_philo *philo);
void	think(t_philo *philo);
void	sleep_philo(t_philo *philo);

int		check_dead(t_philo *philo, int i);
void	destroy_mutexes(t_philo *philo, t_mutexes *mutexes, int count);

#endif
