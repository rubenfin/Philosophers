/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   inits.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: rfinneru <rfinneru@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/24 14:57:34 by rfinneru      #+#    #+#                 */
/*   Updated: 2024/01/25 14:46:29 by rfinneru      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	init_struct(int argc, char *argv[], t_general *general)
{
	struct timeval	time;
	unsigned int	i;

	general->num_of_philos = ft_atoi(argv[1]);
	general->time_to_die = ft_atoi(argv[2]);
	general->time_to_eat = ft_atoi(argv[3]);
	general->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		general->num_to_eat = ft_atoi(argv[5]);
	else
		general->num_to_eat = -1;
	gettimeofday(&time, NULL);
	general->time_started_sec = time.tv_sec;
	general->time_started_usec = time.tv_usec;
	general->terminate = false;
	general->threads_ready = false;
	i = 0;
	general->forks = malloc(sizeof(pthread_mutex_t) * general->num_of_philos);
	while (i < general->num_of_philos)
	{
		use_mutex(&general->forks[i], INIT);
		i++;
	}
	general->total_philos_full = 0;
}

void	init_philosopher(t_philo *philo, t_general *general, int i)
{
	if (i < general->num_of_philos)
		philo->philo_num = i;
	else
		philo->philo_num = -1;
	philo->times_eaten = 0;
	philo->msec_last_meal = 0;
	philo->times_eaten = 0;
	philo->full = false;
	philo->r_fork = &general->forks[i - 1];
	if (philo->philo_num != general->num_of_philos)
		philo->l_fork = &general->forks[i];
	else
		philo->l_fork = &general->forks[0];
}
