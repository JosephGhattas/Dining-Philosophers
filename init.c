#include "philo.h"

long timestamp_ms(void) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void init_forks(t_fork *forks, int count) {
    for (int i = 0; i < count; i++) {
        pthread_mutex_init(&forks[i].mutex, NULL);
        forks[i].owner = i;
        forks[i].dirty = 0;
    }
}

void init_philos(t_philo *philos, t_fork *forks, int count, pthread_mutex_t *print_mutex) {
    for (int i = 0; i < count; i++) {
        philos[i].id = i + 1;
        philos[i].last_meal_time = timestamp_ms();
        philos[i].meals_eaten = 0;
        philos[i].left_fork = &forks[i];
        philos[i].right_fork = &forks[(i + 1) % count];
        philos[i].print_mutex = print_mutex;
    }
}
