/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   actions.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: rfinneru <rfinneru@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/27 10:31:06 by rfinneru      #+#    #+#                 */
/*   Updated: 2024/02/29 15:40:47 by rfinneru      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

// void	grab_uneven(t_philo *philo)
// {
// 	pthread_mutex_lock(philo->right_fork);
// 	printf("%ld %ld has taken a fork\n", get_time_passed(philo->data),
		// philo->philo_num);
// 	pthread_mutex_lock(philo->left_fork);
// 	print_action(philo, FORK);
// }

// void	grab_even(t_philo *philo)
// {
// 	pthread_mutex_lock(philo->left_fork);
// 	print_action(philo, FORK);
// 	pthread_mutex_lock(philo->right_fork);
// 	print_action(philo, FORK);
// }

void	eat(t_philo *philo)
{
	// if (philo->philo_num % 2 == 0)
	// 	grab_even(philo);
	// else
	// 	grab_uneven(philo);
	pthread_mutex_lock(philo->left_fork);
	print_action(philo, FORK);
	pthread_mutex_lock(philo->right_fork);
	print_action(philo, FORK);
	pthread_mutex_lock(&philo->data->eat_lock);
	philo->last_meal_ms = get_curr_time_ms();
	pthread_mutex_unlock(&philo->data->eat_lock);
	if (get_bool(&philo->data->terminate_lock, &philo->data->terminate))
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		return ;
	}
	pthread_mutex_lock(&philo->data->print_lock);
	printf("%ld %ld is eating\n", get_time_passed(philo->data),
		philo->philo_num);
	pthread_mutex_unlock(&philo->data->print_lock);
	get_usleep(philo->data->time_to_eat);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	philo->times_eaten++;
	if (philo->times_eaten == philo->data->total_to_eat)
		set_bool(&philo->data->terminate_lock, &philo->full, true);
}