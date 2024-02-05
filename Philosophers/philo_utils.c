/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo_utils.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: rfinneru <rfinneru@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/22 14:16:16 by rfinneru      #+#    #+#                 */
/*   Updated: 2024/01/25 11:15:40 by rfinneru      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

size_t	get_curr_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + ((time.tv_usec / 1000)));
}

unsigned int	get_time_passed(t_general *general)
{
	struct timeval	time2;
	long			time_passed;

	gettimeofday(&time2, NULL);
	time_passed = (((time2.tv_sec * 1000) - (general->time_started_sec * 1000))
			+ ((time2.tv_usec / 1000) - (general->time_started_usec / 1000)));
	return (time_passed);
}

int	get_sleep(size_t milliseconds)
{
	size_t	start;
	size_t	big;

	start = get_curr_time();
	big = milliseconds / 2;
	usleep(big * 1000);
	while ((get_curr_time() - start) < milliseconds)
		usleep(100);
	return (0);
}

long long	get_last_meal_time(t_general *general, t_philo philo)
{
	long long	last_meal;

	if (philo.msec_last_meal)
	{
		// printf("get_currtime: %zu & philo.msec_lastmeal: %lld\n",
		// get_curr_time(), philo.msec_last_meal);
		last_meal = get_curr_time() - philo.msec_last_meal;
	}
	else
	{
		// printf("timepassed:\n");
		last_meal = get_time_passed(general);
	}
	// printf("last meal fount %lld ago\n", last_meal);
	pthread_mutex_lock(&general->died_lock);
	if (general->time_to_die - last_meal < 0)
		general->terminate = true;
	if (philo.full)
		general->total_philos_full++;
	if (general->total_philos_full == general->num_of_philos)
		general->terminate = true;
	pthread_mutex_unlock(&general->died_lock);
	return (last_meal);
}

void	wait_for_threads(t_general *general)
{
	while (!general->threads_ready)
		;
}
