/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   actions.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: rfinneru <rfinneru@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/19 11:20:58 by rfinneru      #+#    #+#                 */
/*   Updated: 2024/01/25 13:55:20 by rfinneru      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*print_fork(t_general *general, t_philo *philosopher)
{
	pthread_mutex_lock(&general->died_lock);
	if (general->terminate)
	{
		pthread_mutex_unlock(&general->died_lock);
		return (NULL);
	}
	pthread_mutex_unlock(&general->died_lock);
	pthread_mutex_lock(&general->print_lock);
	printf("%u ", get_time_passed(general));
	printf("%u has taken a fork\n", philosopher->philo_num);
	pthread_mutex_unlock(&general->print_lock);
	return (NULL);
}

void	*print_eating(t_general *general, t_philo *philosopher)
{
	if (philosopher->philo_num % 2 == 1)
	{
		pthread_mutex_lock(philosopher->l_fork);
		printf("at ms%u philo : %d, has locked l_fork %p\n",
			get_time_passed(general), philosopher->philo_num,
			philosopher->l_fork);
		print_fork(general, philosopher);
		pthread_mutex_lock(philosopher->r_fork);
		printf("at ms%u philo : %d, has locked r_fork %p\n",
			get_time_passed(general), philosopher->philo_num,
			philosopher->r_fork);
		print_fork(general, philosopher);
	}
	else
	{
		pthread_mutex_lock(philosopher->r_fork);
		printf("at ms%u philo : %d, has locked r_fork %p\n",
			get_time_passed(general), philosopher->philo_num,
			philosopher->r_fork);
		print_fork(general, philosopher);
		pthread_mutex_lock(philosopher->l_fork);
		printf("at ms%u philo : %d, has locked l_fork %p\n",
			get_time_passed(general), philosopher->philo_num,
			philosopher->l_fork);
		print_fork(general, philosopher);
	}
	pthread_mutex_lock(&general->init_mutex);
	philosopher->times_eaten++;
	if (philosopher->times_eaten == general->num_to_eat)
	{
		printf("\n\n%u is full, times eaten = %u\n\n",
		philosopher->philo_num, philosopher->times_eaten);
		philosopher->full = true;
	}
	pthread_mutex_unlock(&general->init_mutex);
	pthread_mutex_lock(&general->died_lock);
	if (general->terminate)
	{
		pthread_mutex_unlock(philosopher->r_fork);
		pthread_mutex_unlock(philosopher->l_fork);
		pthread_mutex_unlock(&general->died_lock);
		return (NULL);
	}
	pthread_mutex_unlock(&general->died_lock);
	pthread_mutex_lock(&general->print_lock);
	printf("%u ", get_time_passed(general));
	printf("%u is eating\n", philosopher->philo_num);
	pthread_mutex_unlock(&general->print_lock);
	philosopher->msec_last_meal = get_curr_time();
	get_sleep(general->time_to_eat);
	pthread_mutex_lock(&general->init_mutex);
	pthread_mutex_unlock(&general->init_mutex);
	pthread_mutex_unlock(philosopher->r_fork);
	printf("at ms%u philo : %d, has unlocked r_fork %p\n",
		get_time_passed(general), philosopher->philo_num, philosopher->r_fork);
	pthread_mutex_unlock(philosopher->l_fork);
	printf("at ms%u philo : %d, has unlocked l_fork %p\n",
		get_time_passed(general), philosopher->philo_num, philosopher->l_fork);
	return (NULL);
}

void	*print_sleeping(t_general *general, t_philo *philosopher)
{
	pthread_mutex_lock(&general->print_lock);
	printf("%u ", get_time_passed(general));
	printf("%u is sleeping\n", philosopher->philo_num);
	pthread_mutex_unlock(&general->print_lock);
	get_sleep(general->time_to_sleep);
	printf("%u ", get_time_passed(general));
	printf("%u has woken up\n", philosopher->philo_num);
	get_last_meal_time(general, *philosopher);
	return (NULL);
}

void	*print_thinking(t_general *general, t_philo *philosopher)
{
	pthread_mutex_lock(&general->died_lock);
	if (general->terminate)
	{
		pthread_mutex_unlock(&general->died_lock);
		return (NULL);
	}
	pthread_mutex_unlock(&general->died_lock);
	pthread_mutex_lock(&general->print_lock);
	printf("%u ", get_time_passed(general));
	printf("%u is thinking\n", philosopher->philo_num);
	pthread_mutex_unlock(&general->print_lock);
	return (NULL);
}

void	*print_died(t_general *general, t_philo *philosopher)
{
	static int	i;

	i++;
	if (i == 1)
	{
		pthread_mutex_lock(&general->print_lock);
		printf("%u ", get_time_passed(general));
		printf("%u died\n", philosopher->philo_num);
		pthread_mutex_unlock(&general->print_lock);
		return (NULL);
	}
	return (NULL);
}
