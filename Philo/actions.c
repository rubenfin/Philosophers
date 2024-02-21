/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   actions.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: rfinneru <rfinneru@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/27 10:31:06 by rfinneru      #+#    #+#                 */
/*   Updated: 2024/02/21 13:40:21 by rfinneru      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	take_fork_even(t_philo *philo)
{
	use_mutex(philo->right_fork, LOCK);
	taken_fork(philo);
	if (get_bool(&philo->data->terminate_lock, &philo->data->terminate))
	{
		use_mutex(philo->right_fork, UNLOCK);
		return ;
	}
	use_mutex(philo->left_fork, LOCK);
	taken_fork(philo);
	if (get_bool(&philo->data->terminate_lock, &philo->data->terminate))
	{
		use_mutex(philo->left_fork, UNLOCK);
		use_mutex(philo->right_fork, UNLOCK);
		return ;
	}
}

void	take_fork_uneven(t_philo *philo)
{
	use_mutex(philo->left_fork, LOCK);
	taken_fork(philo);
	if (get_bool(&philo->data->terminate_lock, &philo->data->terminate))
	{
		use_mutex(philo->left_fork, UNLOCK);
		return ;
	}
	use_mutex(philo->right_fork, LOCK);
	taken_fork(philo);
	if (get_bool(&philo->data->terminate_lock, &philo->data->terminate))
	{
		use_mutex(philo->right_fork, UNLOCK);
		use_mutex(philo->left_fork, UNLOCK);
		return ;
	}
}

void	taken_fork(t_philo *philo)
{
	if (get_bool(&philo->data->terminate_lock, &philo->data->terminate))
		return ;
	use_mutex(&philo->data->print_lock, LOCK);
	printf("%ld %ld has taken a fork\n", get_time_passed(philo->data),
		philo->philo_num);
	use_mutex(&philo->data->print_lock, UNLOCK);
}

void	eat(t_philo *philo)
{
	if (get_bool(&philo->data->terminate_lock, &philo->data->terminate))
		return ;
	if (philo->philo_num % 2 == 0)
		take_fork_even(philo);
	else
		take_fork_uneven(philo);
	if (get_bool(&philo->data->terminate_lock, &philo->data->terminate))
	{
		return ;
	}
	use_mutex(&philo->data->eat_lock, LOCK);
	use_mutex(&philo->data->print_lock, LOCK);
	printf("%ld %ld is eating\n", get_time_passed(philo->data),
		philo->philo_num);
	use_mutex(&philo->data->print_lock, UNLOCK);
	set_long(&philo->data->time_lock, &philo->last_meal_ms, get_curr_time_ms());
	philo->times_eaten++;
	if (philo->times_eaten == philo->data->total_to_eat)
		set_bool(&philo->data->terminate_lock, &philo->full, true);
	use_mutex(&philo->data->eat_lock, UNLOCK);
	get_usleep(philo->data->time_to_eat);
	use_mutex(philo->right_fork, UNLOCK);
	use_mutex(philo->left_fork, UNLOCK);
}

void	sleeping(t_philo *philo)
{
	if (get_bool(&philo->data->terminate_lock, &philo->data->terminate))
		return ;
	use_mutex(&philo->data->print_lock, LOCK);
	if (get_bool(&philo->data->terminate_lock, &philo->data->terminate))
	{
		use_mutex(&philo->data->print_lock, UNLOCK);
		return ;
	}
	printf("%ld %ld is sleeping\n", get_time_passed(philo->data),
		philo->philo_num);
	use_mutex(&philo->data->print_lock, UNLOCK);
	get_usleep(philo->data->time_to_sleep);
}

void	thinking(t_philo *philo)
{
	if (get_bool(&philo->data->terminate_lock, &philo->data->terminate))
		return ;
	use_mutex(&philo->data->print_lock, LOCK);
	if (get_bool(&philo->data->terminate_lock, &philo->data->terminate))
	{
		use_mutex(&philo->data->print_lock, UNLOCK);
		return ;
	}
	printf("%ld %ld is thinking\n", get_time_passed(philo->data),
		philo->philo_num);
	use_mutex(&philo->data->print_lock, UNLOCK);
}

void	died(t_philo *philo)
{
	use_mutex(&philo->data->print_lock, LOCK);
	printf("%ld %ld died\n", get_time_passed(philo->data), philo->philo_num);
	use_mutex(&philo->data->print_lock, UNLOCK);
}
