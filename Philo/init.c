/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   init.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: rfinneru <rfinneru@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/27 11:57:57 by rfinneru      #+#    #+#                 */
/*   Updated: 2024/02/21 18:22:17 by rfinneru      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	init_struct(int ac, char **av, t_data *data)
{
	int	i;
	data->num_of_philophers = ft_atol(av[1]);
	data->time_to_die = ft_atol(av[2]);
	data->time_to_eat = ft_atol(av[3]);
	data->time_to_sleep = ft_atol(av[4]);
	if (data->num_of_philophers < 1 || data->time_to_die < 0
		|| data->time_to_eat < 0 || data->time_to_sleep < 0)
	{
		printf("Invalid input\n");
		return (0);
	}
	if (ac == 6)
	{
		data->total_to_eat = ft_atol(av[5]);
		if (data->total_to_eat < 1)
		{
			printf("Invalid input\n");
			return (0);
		}
	}
	else
		data->total_to_eat = -1;
	data->all_threads_ready = false;
	data->terminate = false;
	data->philos = malloc(sizeof(t_philo) * data->num_of_philophers);
	i = -1;
	while (++i < data->num_of_philophers)
		data->philos[i].th_id = (pthread_t)malloc(sizeof(pthread_t));
	i = -1;
	data->forks = malloc(sizeof(t_mtx) * data->num_of_philophers);
	while (++i < data->num_of_philophers)
		use_mutex(&data->forks[i], INIT);
	use_mutex(&data->print_lock, INIT);
	use_mutex(&data->init_lock, INIT);
	use_mutex(&data->eat_lock, INIT);
	use_mutex(&data->terminate_lock, INIT);
	use_mutex(&data->time_lock, INIT);
	return (1);
}

void	init_philos(t_data *data)
{
	long i;

	i = -1;
	while (++i < data->num_of_philophers)
	{
		data->philos[i].data = data;
		data->philos[i].philo_num = i + 1;
		data->philos[i].full = false;
		data->philos[i].dead = false;
		data->philos[i].times_eaten = 0;
		if (data->philos[i].philo_num < data->num_of_philophers)
			data->philos[i].left_fork = &data->forks[data->philos[i].philo_num];
		else
			data->philos[i].left_fork = &data->forks[0];
		data->philos[i].right_fork = &data->forks[i];
		data->philos[i].last_meal_ms = 0;
	}
}