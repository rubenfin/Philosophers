/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philosophers.h                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: rfinneru <rfinneru@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/26 12:37:21 by rfinneru      #+#    #+#                 */
/*   Updated: 2024/02/21 14:33:46 by rfinneru      ########   odam.nl         */
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

typedef pthread_mutex_t	t_mtx;
typedef struct s_data	t_data;

typedef enum s_opcode
{
	LOCK,
	UNLOCK,
	INIT,
	DESTROY,
	CREATE,
	JOIN,
}						t_opcode;

typedef struct s_philo
{
	long				philo_num;
	long				last_meal_ms;
	long				times_eaten;
	bool				full;
	bool				dead;
	pthread_t			th_id;
	t_mtx				*right_fork;
	t_mtx				*left_fork;
	t_data				*data;
}						t_philo;

struct					s_data
{
	long				num_of_philophers;
	long				time_to_die;
	long				time_to_eat;
	long				time_to_sleep;
	long				total_to_eat;
	long				time_started;
	bool				all_threads_ready;
	bool				terminate;
	t_mtx				print_lock;
	t_mtx				eat_lock;
	t_mtx				init_lock;
	t_mtx				time_lock;
	t_mtx				terminate_lock;
	t_mtx				*forks;
	t_philo				*philos;
};

/*
SAFE MUTEX USAGE
*/
void					use_mutex(t_mtx *mutex, t_opcode opcode);

/*
INITIALIZING
*/
int						init_struct(int ac, char **av, t_data *data);
void					init_philos(t_data *data);

/*
ACTIONS
*/
void					take_fork_even(t_philo *philo);
void					take_fork_uneven(t_philo *philo);
void					taken_fork(t_philo *philo);
void					eat(t_philo *philo);
void					sleeping(t_philo *philo);
void					thinking(t_philo *philo);
void					died(t_philo *philo);

/*
GETTERS AND SETTERS
*/
bool					get_bool(t_mtx *mutex, bool *value);
void					set_bool(t_mtx *mutex, bool *value, bool changed);
long					get_long(t_mtx *mutex, long *value);
void					set_long(t_mtx *mutex, long *value, long changed);

/*
UTILS
*/
long					ft_atol(const char *nptr);
void					get_usleep(size_t milliseconds);
long					get_curr_time_ms(void);
long					get_time_passed(t_data *data);
long					last_meal(t_philo *philo);

#endif