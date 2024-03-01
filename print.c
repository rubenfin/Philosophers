/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   print.c                                            :+:    :+:            */
/*                                                     +:+                    */
/*   By: rfinneru <rfinneru@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/03/01 12:04:33 by rfinneru      #+#    #+#                 */
/*   Updated: 2024/03/01 13:29:43 by rfinneru      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	print_action(t_philo *philo, t_action_enum action_enum)
{
	static const char	*actions[5] = {"is eating", "is sleeping",
		"is thinking", "has taken a fork", NULL};

	if (get_bool(&philo->data->terminate_lock, &philo->data->terminate))
		return (0);
	use_mutex(&philo->data->print_lock, LOCK);
	if (get_bool(&philo->data->terminate_lock, &philo->data->terminate))
	{
		use_mutex(&philo->data->print_lock, UNLOCK);
		return (0);
	}
	printf("%ld %ld %s\n", get_time_passed(philo->data), philo->philo_num,
		actions[action_enum]);
	use_mutex(&philo->data->print_lock, UNLOCK);
	return (1);
}

void	print_err(void)
{
	printf("usage: ./philo time_to_die time_to_eat time_to_sleep");
	printf(" [number_of_times_each_philo_must_eat]\n");
}
