/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   init.c                                             :+:    :+:            */
/*                                                     +:+                    */
/*   By: rfinneru <rfinneru@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/27 11:57:57 by rfinneru      #+#    #+#                 */
/*   Updated: 2024/03/01 14:25:45 by rfinneru      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_isdigit(int c)
{
	return (c >= '0' && c <= '9');
}

int	check_av(int ac, char **av)
{
	int	i;
	int	j;

	i = 1;
	while (i < ac)
	{
		j = -1;
		while (av[i][++j])
		{
			if (!ft_isdigit((av[i][j])) || j > 17)
				return (0);
		}
		i++;
	}
	return (1);
}

int	set_and_check_args(int ac, char **av, t_data *data)
{
	if (!check_av(ac, av))
		return (printf("Invalid input\n"), 0);
	data->num_of_philophers = ft_atol(av[1]);
	data->time_to_die = ft_atol(av[2]);
	data->time_to_eat = ft_atol(av[3]);
	data->time_to_sleep = ft_atol(av[4]);
	if (data->num_of_philophers < 1 || data->num_of_philophers > 200
		|| data->time_to_die < 60 || data->time_to_eat < 60
		|| data->time_to_sleep < 60)
		return (printf("Invalid input\n"), 0);
	if (ac == 6)
	{
		data->total_to_eat = ft_atol(av[5]);
		if (data->total_to_eat < 0)
			return (printf("Invalid input\n"), 0);
	}
	else
		data->total_to_eat = -1;
	data->all_threads_ready = false;
	data->terminate = false;
	return (1);
}

int	init_struct(int ac, char **av, t_data *data)
{
	int	i;

	if (!set_and_check_args(ac, av, data))
		return (0);
	data->philos = malloc(sizeof(t_philo) * data->num_of_philophers);
	if (!data->philos)
		return (0);
	data->forks = malloc(sizeof(t_mtx) * data->num_of_philophers);
	if (!data->forks)
		return (free(data->forks), 0);
	i = -1;
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
	long	i;

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
