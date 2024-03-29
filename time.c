/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philo_utils.c                                      :+:    :+:            */
/*                                                     +:+                    */
/*   By: rfinneru <rfinneru@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/26 16:12:00 by rfinneru      #+#    #+#                 */
/*   Updated: 2024/03/01 12:04:44 by rfinneru      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	get_usleep(size_t milliseconds)
{
	size_t	start;
	size_t	big;

	start = get_curr_time_ms();
	big = milliseconds / 2;
	usleep(big * 1000);
	while ((get_curr_time_ms() - start) < milliseconds)
		usleep(100);
}

long	get_curr_time_ms(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + ((time.tv_usec / 1000)));
}

long	get_time_passed(t_data *data)
{
	struct timeval	time;
	long			time_passed;

	gettimeofday(&time, NULL);
	use_mutex(&data->time_lock, LOCK);
	time_passed = (get_curr_time_ms() - data->time_started);
	use_mutex(&data->time_lock, UNLOCK);
	return (time_passed);
}

long	last_meal(t_philo *philo)
{
	long	last_eaten;

	use_mutex(&philo->data->time_lock, LOCK);
	last_eaten = get_curr_time_ms() - philo->last_meal_ms;
	use_mutex(&philo->data->time_lock, UNLOCK);
	return (last_eaten);
}
