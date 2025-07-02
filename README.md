# Dining Philosophers Project — Threaded C Implementation

This is a custom implementation of the classic **Dining Philosophers** problem in C using **POSIX threads** and **mutexes**, developed as part of the 42 curriculum. The project focuses on concurrency, synchronization, and correct handling of race conditions and edge cases.


# Problem Summary

Philosophers sit around a table and alternate between thinking and eating. Each philosopher needs two forks to eat, but each fork is shared with a neighbor. The challenge is to coordinate the philosophers to avoid:

* **Deadlocks**: no one can eat because all are waiting.
* **Starvation**: some eat infinitely, others never eat.


# Project Architecture and Design

### Threads & Synchronization

* Each philosopher is a thread running a loop of `think -> take forks -> eat -> sleep`.
* Forks are represented by **mutexes**.
* An asymmetric strategy is used:

  * Even-numbered philosophers pick up the **left** fork first.
  * Odd-numbered philosophers pick up the **right** fork first.
* **Smart fork locking** ensures threads back off when unable to lock both forks, minimizing deadlocks.

### Timing and Sleep

* All timing is based on a `timestamp_ms()` utility using `gettimeofday()`.
* A `smart_sleep()` function allows for more responsive sleeping.

### Starvation Detection

* A dedicated **monitor thread** checks if any philosopher has not eaten within `time_to_die`.
* Each philosopher tracks `last_meal_time` using a **mutex-protected** variable.

### Meal Management

* Optional CLI argument allows stopping simulation after each philosopher eats `N` times.
* Each philosopher tracks `meals_eaten` and compares it against `meals_goal`.
* 


## File Structure

```
philo/
├── Makefile
├── philo.h             # Main header file with all structs & declarations
├── main.c              # Initialization and startup logic
├── philo.c             # Philosopher thread routine and monitor
├── forks.c             # Fork locking and unlocking strategy
├── action.c            # Actions: eat, sleep, think
├── init.c              # Initializing philosophers and forks
├── utils.c             # Utility functions: atoi, is_dead, check_arg, etc.
```


## Build & Run

### Compile
```bash
make
```

### Usage
```bash
./philo <num_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [num_times_each_philo_must_eat]
```

### Example
```bash
./philo 5 800 200 200
```


# Features Summary

* [x] Threaded simulation with safe fork access
* [x] Per-philosopher meal tracking
* [x] Custom `smart_sleep()` for accurate delays
* [x] Monitor thread for starvation detection
* [x] Handles edge cases: 1 philosopher, tight timing windows
* [x] Custom `ft_atoi()` and argument parsing with safety checks
* [x] Clean and destroy all mutexes before exit


# Testing

* Memory-checked using **Valgrind**
* Race condition checked with **Helgrind**
* Tested across multiple configurations and stress cases (e.g. 1–200 philosophers)


# Author

**Joseph Ghattas**
Student @ 42 Beirut & Lebanese University
GitHub: [JosephGhattas](https://github.com/JosephGhattas)
