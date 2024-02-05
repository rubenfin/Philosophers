/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philosophers.h                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: rfinneru <rfinneru@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/17 12:16:18 by rfinneru      #+#    #+#                 */
/*   Updated: 2024/01/25 13:40:51 by rfinneru      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef enum s_opcode
{
	INIT,
	DESTROY,
	LOCK,
	UNLOCK,
	CREATE,
	JOIN
}					t_opcode;

typedef struct s_philo
{
	unsigned int	philo_num;
	long long		msec_last_meal;
	unsigned int	times_eaten;
	bool			full;
	pthread_mutex_t	*r_fork;
	pthread_mutex_t	*l_fork;
}					t_philo;

typedef struct s_general
{
	unsigned int	num_of_philos;
	long long		time_to_die;
	unsigned int	time_to_eat;
	unsigned int	time_to_sleep;
	unsigned int	num_to_eat;
	unsigned long	time_started_sec;
	unsigned long	time_started_usec;
	unsigned int	total_philos_full;
	bool			terminate;
	bool			threads_ready;
	pthread_mutex_t	check_log;
	pthread_mutex_t	init_mutex;
	pthread_mutex_t	died_lock;
	pthread_mutex_t	print_lock;
	pthread_mutex_t	*forks;
}					t_general;

int					ft_atoi(const char *nptr);
unsigned int		get_time_passed(t_general *general);
void				*print_fork(t_general *general, t_philo *philosopher);
void				*print_eating(t_general *general, t_philo *philosopher);
void				*print_sleeping(t_general *general, t_philo *philosopher);
void				*print_thinking(t_general *general, t_philo *philosopher);
void				*print_died(t_general *general, t_philo *philosopher);

// philo utils
size_t				get_curr_time(void);
int					get_sleep(size_t milliseconds);
unsigned int		get_time_passed(t_general *general);
long long			get_last_meal_time(t_general *general, t_philo philo);
void				wait_for_threads(t_general *general);

// initializing struct
void				init_struct(int argc, char *argv[], t_general *general);
void				init_philosopher(t_philo *philo, t_general *general, int i);
void				init_mutex(t_general *general);
void				destroy_mutex(t_general *general);
void				use_mutex(pthread_mutex_t *mutex, t_opcode opcode);

#endif
