/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 11:12:09 by marvin            #+#    #+#             */
/*   Updated: 2025/04/23 11:12:09 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void print_state(t_philo *philo, const char *state) {
    pthread_mutex_lock(philo->print_mutex);
    printf("%ld %d %s\n", timestamp_ms(), philo->id, state);
    pthread_mutex_unlock(philo->print_mutex);
}

int owns_fork(t_philo *philo, t_fork *fork)
{
    return (fork->owner == philo->id);
}

void eat(t_philo *philo)
{
    print_state(philo, "is eating");
    philo->last_meal_time = timestamp_ms();
    usleep(200 * 1000); // placeholder: simulate eating
    philo->meals_eaten++;
    
    pthread_mutex_lock(&philo->left_fork->mutex);
    philo->left_fork->dirty = 1;
    pthread_mutex_unlock(&philo->left_fork->mutex);

    pthread_mutex_lock(&philo->right_fork->mutex);
    philo->right_fork->dirty = 1;
    pthread_mutex_unlock(&philo->right_fork->mutex);
}

void think(t_philo *philo) {
    print_state(philo, "is thinking");
    usleep(100 * 1000);
}

void maybe_give_fork(t_philo *philo, t_fork *fork)
{
    pthread_mutex_lock(&fork->mutex);
    if (fork->owner == philo->id && fork->requested && fork->dirty) {
        fork->owner = (philo->id == philo->left_fork->owner ? 
                      philo->left_fork->owner : philo->right_fork->owner);
        fork->dirty = 0;
        fork->requested = 0;
    }
    pthread_mutex_unlock(&fork->mutex);
}

void sleep_philo(t_philo *philo)
{
    print_state(philo, "is sleeping");
    usleep(150 * 1000);
    maybe_give_fork(philo, philo->left_fork);
    maybe_give_fork(philo, philo->right_fork);

}

void *philo_routine(void *arg) {
    t_philo *philo = (t_philo *)arg;

    while (1) {
        // THINK
        think(philo);

        wait_for_forks(philo);
        print_state(philo, "has taken a fork");
        print_state(philo, "has taken a fork");


        // EAT
        eat(philo);

        // RELEASE FORKS
        pthread_mutex_unlock(&philo->right_fork->mutex);
        pthread_mutex_unlock(&philo->left_fork->mutex);

        // SLEEP
        sleep_philo(philo);
    }

    return NULL;
}

void request_fork(t_philo *philo, t_fork *fork)
{
    pthread_mutex_lock(&fork->mutex);
    if (!owns_fork(philo, fork)) {
        fork->requested = 1;
    }
    pthread_mutex_unlock(&fork->mutex);
}

// Check if we own both forks (if not, request them)
void wait_for_forks(t_philo *philo)
{
    while (1) {
        int left_ready = 0, right_ready = 0;

        pthread_mutex_lock(&philo->left_fork->mutex);
        if (philo->left_fork->owner == philo->id) {
            left_ready = 1;
        } else {
            philo->left_fork->requested = 1;
        }
        pthread_mutex_unlock(&philo->left_fork->mutex);

        pthread_mutex_lock(&philo->right_fork->mutex);
        if (philo->right_fork->owner == philo->id) {
            right_ready = 1;
        } else {
            philo->right_fork->requested = 1;
        }
        pthread_mutex_unlock(&philo->right_fork->mutex);

        if (left_ready && right_ready)
            break;
        usleep(100); // Prevent busy waiting
    }
}