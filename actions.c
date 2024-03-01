/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   actions.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: rfinneru <rfinneru@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/27 10:31:06 by rfinneru      #+#    #+#                 */
/*   Updated: 2024/03/01 13:26:08 by rfinneru      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	unlock_mutexes(t_mtx *mtx1, t_mtx *mtx2, t_mtx *mtx3)
{
	if (mtx1)
		pthread_mutex_unlock(mtx1);
	if (mtx2)
		pthread_mutex_unlock(mtx2);
	if (mtx3)
		pthread_mutex_unlock(mtx3);
	return (0);
}

int	eat(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	print_action(philo, FORK);
	pthread_mutex_lock(philo->right_fork);
	print_action(philo, FORK);
	pthread_mutex_lock(&philo->data->eat_lock);
	philo->last_meal_ms = get_curr_time_ms();
	pthread_mutex_unlock(&philo->data->eat_lock);
	if (get_bool(&philo->data->terminate_lock, &philo->data->terminate))
		return (unlock_mutexes(philo->right_fork, philo->left_fork, NULL));
	pthread_mutex_lock(&philo->data->print_lock);
	if (get_bool(&philo->data->terminate_lock, &philo->data->terminate))
		return (unlock_mutexes(philo->right_fork, philo->left_fork,
				&philo->data->print_lock));
	printf("%ld %ld is eating\n", get_time_passed(philo->data),
		philo->philo_num);
	pthread_mutex_unlock(&philo->data->print_lock);
	get_usleep(philo->data->time_to_eat);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_unlock(philo->left_fork);
	philo->times_eaten++;
	if (philo->times_eaten == philo->data->total_to_eat)
		set_bool(&philo->data->terminate_lock, &philo->full, true);
	return (1);
}
