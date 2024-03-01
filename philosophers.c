/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philosophers.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: rfinneru <rfinneru@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/26 12:37:19 by rfinneru      #+#    #+#                 */
/*   Updated: 2024/03/01 13:30:31 by rfinneru      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	use_mutex(t_mtx *mutex, t_opcode opcode)
{
	if (LOCK == opcode)
		pthread_mutex_lock(mutex);
	else if (UNLOCK == opcode)
		pthread_mutex_unlock(mutex);
	else if (INIT == opcode)
		pthread_mutex_init(mutex, NULL);
	else if (DESTROY == opcode)
		pthread_mutex_destroy(mutex);
	else
	{
		printf("WRONG OPCODE IN USE_MUTEX()\n");
		exit(EXIT_FAILURE);
	}
}

void	wait_for_all_threads(t_philo *philo)
{
	while (!get_bool(&philo->data->init_lock, &philo->data->all_threads_ready))
		;
}

void	destroy_mutex(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->num_of_philophers)
		use_mutex(&data->forks[i], DESTROY);
	free(data->forks);
	free(data->philos);
	use_mutex(&data->print_lock, DESTROY);
	use_mutex(&data->init_lock, DESTROY);
	use_mutex(&data->eat_lock, DESTROY);
	use_mutex(&data->terminate_lock, DESTROY);
	use_mutex(&data->time_lock, DESTROY);
}

void	*philosophers(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	wait_for_all_threads(philo);
	set_long(&philo->data->time_lock, &philo->last_meal_ms,
		philo->data->time_started);
	if (philo->data->total_to_eat == 0)
		return (set_bool(&philo->data->terminate_lock, &philo->full, true),
			NULL);
	else if (philo->data->num_of_philophers == 1)
		return (print_action(philo, FORK), NULL);
	else
		print_action(philo, FORK);
	if (philo->philo_num % 2 == 1)
		get_usleep(philo->data->time_to_eat / 2);
	while (!get_bool(&philo->data->terminate_lock, &philo->data->terminate))
	{
		eat(philo);
		if (!print_action(philo, SLEEP))
			break ;
		get_usleep(philo->data->time_to_sleep);
		print_action(philo, THINK);
	}
	return (NULL);
}

int	main(int ac, char **av)
{
	t_data	data;
	int		i;

	i = -1;
	if (ac == 5 || ac == 6)
	{
		if (!init_struct(ac, av, &data))
			return (1);
		init_philos(&data);
		while (++i < data.num_of_philophers)
			pthread_create(&data.philos[i].th_id, NULL, &philosophers,
				&data.philos[i]);
		set_long(&data.time_lock, &data.time_started, get_curr_time_ms());
		set_bool(&data.init_lock, &data.all_threads_ready, true);
		monitor_philos(&data);
		i = -1;
		while (++i < data.num_of_philophers)
			pthread_join(data.philos[i].th_id, NULL);
		destroy_mutex(&data);
	}
	else
		print_err();
	return (0);
}
