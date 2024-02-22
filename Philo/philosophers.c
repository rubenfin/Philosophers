/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philosophers.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: rfinneru <rfinneru@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/26 12:37:19 by rfinneru      #+#    #+#                 */
/*   Updated: 2024/02/22 09:18:23 by rfinneru      ########   odam.nl         */
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

void	*philosophers(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	wait_for_all_threads(philo);
	set_long(&philo->data->time_lock, &philo->data->time_started,
		get_curr_time_ms());
	set_long(&philo->data->time_lock, &philo->last_meal_ms, get_curr_time_ms());
	if (philo->data->num_of_philophers == 1)
	{
		print_action(philo, FORK);
		return (NULL);
	}
	if (philo->philo_num % 2 == 0)
		get_usleep(philo->data->time_to_eat);
	while (!get_bool(&philo->data->terminate_lock, &philo->data->terminate))
	{
		if (!get_bool(&philo->data->terminate_lock, &philo->data->terminate))
			eat(philo);
		if (!get_bool(&philo->data->terminate_lock, &philo->data->terminate))
			sleeping(philo);
		if (!get_bool(&philo->data->terminate_lock, &philo->data->terminate))
			print_action(philo, THINK);
	}
	return (NULL);
}

void	destroy_mutex(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->num_of_philophers)
	{
		use_mutex(&data->forks[i], DESTROY);
		free(&data->philos[i]);
	}
	free(data->forks);
	use_mutex(&data->print_lock, DESTROY);
	use_mutex(&data->init_lock, DESTROY);
	use_mutex(&data->eat_lock, DESTROY);
	use_mutex(&data->terminate_lock, DESTROY);
	use_mutex(&data->time_lock, DESTROY);
}

void	check_alive(t_data *data)
{
	long	i;
	long	full;

	i = 0;
	full = 0;
	get_usleep(data->time_to_die);
	while (i < data->num_of_philophers)
	{
		use_mutex(&data->eat_lock, LOCK);
		if ((last_meal(&data->philos[i]))- data->time_to_die > 0)
		{
			set_bool(&data->terminate_lock, &data->philos[i].dead, true);
			died(&data->philos[i]);
			set_bool(&data->terminate_lock, &data->terminate, true);
			use_mutex(&data->eat_lock, UNLOCK);
			break ;
		}
		if (get_bool(&data->terminate_lock, &data->philos[i].full))
			full++;
		i++;
		if (i == data->num_of_philophers)
		{
			if (full == data->num_of_philophers)
			{
				use_mutex(&data->print_lock, LOCK);
				set_bool(&data->terminate_lock, &data->terminate, true);
				printf("%ld all philos are full\n", get_time_passed(data));
				use_mutex(&data->print_lock, UNLOCK);
				use_mutex(&data->eat_lock, UNLOCK);
				break ;
			}
			i = 0;
			full = 0;
		}
		use_mutex(&data->eat_lock, UNLOCK);
	}
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
		get_usleep(100);
		set_bool(&data.init_lock, &data.all_threads_ready, true);
		check_alive(&data);
		i = -1;
		while (++i < data.num_of_philophers)
			pthread_join(data.philos[i].th_id, NULL);
		destroy_mutex(&data);
	}
	else
		printf("usage: ./philo time_to_die time_to_eat time_to_sleep [number_of_times_each_philo_must_eat]\n");
	return (0);
}
