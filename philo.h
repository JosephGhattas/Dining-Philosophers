#ifndef PHILO_H
#define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>

# define MAX_PHILOS 200


typedef struct s_fork {
    pthread_mutex_t mutex;
    int owner;         // philosopher id
    int dirty;        // 1 if dirty, 0 if clean
    int requested;     // 1 if a request has been made for this fork
} t_fork;

typedef struct s_philo {
    int id;
    long last_meal_time;
    int meals_eaten;
    t_fork *left_fork;
    t_fork *right_fork;
    pthread_mutex_t *print_mutex;
} t_philo;


void *philo_routine(void *arg);
long timestamp_ms(void);
void print_state(t_philo *philo, const char *state);
void maybe_give_fork(t_philo *philo, t_fork *fork)

#endif
