/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   monitor.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: rfinneru <rfinneru@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/03/01 12:56:48 by rfinneru      #+#    #+#                 */
/*   Updated: 2024/03/01 13:44:19 by rfinneru      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	check_if_dead(t_data *data, long i)
{
	use_mutex(&data->eat_lock, LOCK);
	if ((last_meal(&data->philos[i])) - data->time_to_die > 0
		&& get_time_passed(data) > data->time_to_die)
	{
		set_bool(&data->terminate_lock, &data->terminate, true);
		printf("%ld %ld has died\n", get_time_passed(data), i + 1);
		use_mutex(&data->eat_lock, UNLOCK);
		return (0);
	}
	use_mutex(&data->eat_lock, UNLOCK);
	return (1);
}

int	check_if_full(t_data *data, long full)
{
	use_mutex(&data->eat_lock, LOCK);
	if (full == data->num_of_philophers)
	{
		use_mutex(&data->print_lock, LOCK);
		set_bool(&data->terminate_lock, &data->terminate, true);
		printf("%ld all philos are full\n", get_time_passed(data));
		use_mutex(&data->print_lock, UNLOCK);
		use_mutex(&data->eat_lock, UNLOCK);
		return (0);
	}
	use_mutex(&data->eat_lock, UNLOCK);
	return (1);
}

void	monitor_philos(t_data *data)
{
	long	i;
	long	full;

	i = 0;
	get_usleep(10);
	while (i < data->num_of_philophers)
	{
		if (!check_if_dead(data, i))
			break ;
		if (get_bool(&data->terminate_lock, &data->philos[i].full))
			full++;
		i++;
		if (i == data->num_of_philophers)
		{
			if (!check_if_full(data, full))
				break ;
			i = 0;
			full = 0;
		}
	}
}
