#include "philo.h"

void	print_state(t_philo *philo, const char *state)
{
	pthread_mutex_lock(philo->print_mutex);
	printf("%ld %d %s\n", timestamp_ms(), philo->id, state);
	pthread_mutex_unlock(philo->print_mutex);
}

int	observer(t_philo *philo)
{
	size_t	curr_time;

	curr_time = timestamp_ms();
	if ((curr_time - philo->last_meal_time) >= philo->time_to_die)
		return (1);
	return (0);
}

void	get_fork(t_philo *philo, t_fork *fork)
{
	if (fork->dirty == 1 && &fork->owner != &philo->id)
		fork->owner = philo->id;
}

void	check_forks(t_philo	*philo)
{
	int	left_ready;
	int	right_ready;

	while (1)
	{
		pthread_mutex_lock(&philo->left_fork->mutex);
		if (philo->left_fork->owner == philo->id)
			left_ready = 1;
		else if (philo->left_fork->dirty == 1)
			get_fork(philo, philo->left_fork);
		pthread_mutex_unlock(&philo->left_fork->mutex);
		pthread_mutex_lock(&philo->right_fork->mutex);
		if (philo->right_fork->owner == philo->id)
			right_ready = 1;
		else if (philo->right_fork->dirty == 1)
			get_fork(philo, philo->right_fork);
		pthread_mutex_unlock(&philo->right_fork->mutex);
		if (left_ready == 1 && right_ready == 1)
		{
			print_state(philo, "has both forks");
			right_ready = 0;
			left_ready = 0;
			break;
		}
	}
}

void	eat(t_philo *philo)
{
	print_state(philo, "is eating");
	usleep(philo->time_to_eat);
	philo->last_meal_time = timestamp_ms();
	philo->meals_eaten++;
	philo->left_fork->dirty = 1;
	philo->right_fork->dirty = 1;
}

void	think(t_philo *philo)
{
	print_state(philo, "is thinking");
	usleep((philo->time_to_die) - (philo->time_to_sleep));
}

void	sleep_philo(t_philo *philo)
{
	print_state(philo, "is sleeping");
	usleep(philo->time_to_sleep);
}
void	running_philo(t_philo *philo)
{
	int	obv;

	obv = observer(philo);
	think(philo);
	check_forks(philo);
 	pthread_mutex_lock(&philo->right_fork->mutex);
	pthread_mutex_lock(&philo->left_fork->mutex);
	eat(philo);
	pthread_mutex_unlock(&philo->right_fork->mutex);
	pthread_mutex_unlock(&philo->left_fork->mutex);
	sleep_philo(philo);
	if (obv == 1)
	{
		print_state(philo, "died");
		exit (1);
	}
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->meals_goal == 0)
	{
		while (1)
		{
			running_philo(philo);
			if (philo->died == 1)
				break;
		}
	}
	else
		while(philo->meals_eaten < philo->meals_goal)
			running_philo(philo);
	return (NULL);
} 
