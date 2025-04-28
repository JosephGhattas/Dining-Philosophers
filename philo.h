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

typedef struct s_philo
{
	int				id;
	long			last_meal_time;			
	t_fork			*left_fork;
	t_fork			*right_fork;
	pthread_mutex_t	*print_mutex;
	size_t			time_to_die;
	size_t			time_to_eat;
	size_t			time_to_sleep;
	size_t			meals_goal;
	size_t			meals_eaten;
	int				died;
}	t_philo;

int		check_arg(int argc, char **argv);
long	timestamp_ms(void);
void	init_forks(t_fork *forks, int count);
void	init_philos(t_philo *philos, t_fork *forks, int count, pthread_mutex_t *print_mutex);
void	init_philo_params(t_philo *philos, int count, int argc, char **argv);

int		ft_atoi(const char *str);
void	*philo_routine(void *arg);
long	timestamp_ms(void);
void	print_state(t_philo *philo, const char *state);
void	maybe_give_fork(t_philo *philo, t_fork *fork);

#endif
