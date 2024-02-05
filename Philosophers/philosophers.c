/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   philosophers.c                                     :+:    :+:            */
/*                                                     +:+                    */
/*   By: rfinneru <rfinneru@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/17 12:16:35 by rfinneru      #+#    #+#                 */
/*   Updated: 2024/01/25 15:31:25 by rfinneru      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	uneven_logic(t_general *general, t_philo *philosopher)
{
	static int	flag;

	get_last_meal_time(general, *philosopher);
	pthread_mutex_lock(&general->init_mutex);
	pthread_mutex_lock(&general->died_lock);
	if (general->terminate)
	{
		if (!general->num_to_eat
			|| general->num_to_eat != philosopher->times_eaten)
			print_died(general, philosopher);
		else if (flag == 0)
		{
			flag++;
			printf("philos are full\n");
		}
		pthread_mutex_unlock(&general->died_lock);
		pthread_mutex_unlock(&general->init_mutex);
		return (0);
	}
	pthread_mutex_unlock(&general->died_lock);
	pthread_mutex_unlock(&general->init_mutex);
	if (philosopher->philo_num % 2 == 1 && !philosopher->full)
	{
		get_last_meal_time(general, *philosopher);
		print_eating(general, philosopher);
		print_sleeping(general, philosopher);
	}
	else
	{
		get_last_meal_time(general, *philosopher);
		print_thinking(general, philosopher);
		if (!philosopher->full)
			print_eating(general, philosopher);
		print_sleeping(general, philosopher);
	}
	get_last_meal_time(general, *philosopher);
	pthread_mutex_lock(&general->init_mutex);
	pthread_mutex_lock(&general->died_lock);
	if (general->terminate)
	{
		if (!general->num_to_eat
			|| general->num_to_eat != philosopher->times_eaten)
			print_died(general, philosopher);
		else if (flag == 0)
		{
			flag++;
			printf("philos are full\n");
		}
		pthread_mutex_unlock(&general->died_lock);
		pthread_mutex_unlock(&general->init_mutex);
		return (0);
	}
	pthread_mutex_unlock(&general->died_lock);
	pthread_mutex_unlock(&general->init_mutex);
	return (1);
}

int	even_logic(t_general *general, t_philo *philosopher)
{
	static int	flag;

	if (philosopher->philo_num % 2 == 1 && !philosopher->full)
		print_eating(general, philosopher);
	else
		print_sleeping(general, philosopher);
	get_last_meal_time(general, *philosopher);
	pthread_mutex_lock(&general->init_mutex);
	pthread_mutex_lock(&general->died_lock);
	if (general->terminate)
	{
		if (!general->num_to_eat
			|| general->num_to_eat != philosopher->times_eaten)
			print_died(general, philosopher);
		else if (flag == 0)
		{
			flag++;
			printf("philos are full\n");
		}
		pthread_mutex_unlock(&general->died_lock);
		pthread_mutex_unlock(&general->init_mutex);
		return (0);
	}
	pthread_mutex_unlock(&general->died_lock);
	pthread_mutex_unlock(&general->init_mutex);
	print_thinking(general, philosopher);
	if (philosopher->philo_num % 2 == 0 && !philosopher->full)
		print_eating(general, philosopher);
	else
		print_sleeping(general, philosopher);
	get_last_meal_time(general, *philosopher);
	pthread_mutex_lock(&general->init_mutex);
	pthread_mutex_lock(&general->died_lock);
	if (general->terminate)
	{
		if (!general->num_to_eat
			|| general->num_to_eat != philosopher->times_eaten)
			print_died(general, philosopher);
		else if (flag == 0)
		{
			flag++;
			printf("philos are full\n");
		}
		pthread_mutex_unlock(&general->died_lock);
		pthread_mutex_unlock(&general->init_mutex);
		return (0);
	}
	pthread_mutex_unlock(&general->died_lock);
	pthread_mutex_unlock(&general->init_mutex);
	return (1);
}

void	check_for_deaths(t_general *general)
{
	while (!general->terminate)
	
	
}

void	*philosopher(void *arg)
{
	t_general	*general;
	t_philo		philosopher;
	static int	i = 1;

	general = (t_general *)arg;
	pthread_mutex_lock(&general->init_mutex);
	init_philosopher(&philosopher, general, i++);
	pthread_mutex_unlock(&general->init_mutex);
	wait_for_threads(general);
	if (philosopher->philo == -1)
	{
		check_for_deaths(&general);
		
	}
	while (1)
	{
		if (general->num_of_philos == 1)
		{
			printf("edge case");
			return (NULL);
		}
		if (general->num_of_philos % 2 == 0)
		{
			if (!even_logic(general, &philosopher))
				return (NULL);
		}
		if (general->num_of_philos % 2 == 1)
		{
			if (!uneven_logic(general, &philosopher))
				return (NULL);
		}
	}
	return (NULL);
}

// void	use_thread(pthread_t *threads, t_general opcode)
// {
// }

void	use_mutex(pthread_mutex_t *mutex, t_opcode opcode)
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

void	init_mutex(t_general *general)
{
	use_mutex(&general->print_lock, INIT);
	use_mutex(&general->check_log, INIT);
	use_mutex(&general->init_mutex, INIT);
	use_mutex(&general->died_lock, INIT);
}

void	destroy_mutex(t_general *general)
{
	unsigned int	i;

	i = -1;
	while (++i < general->num_of_philos)
		use_mutex(&general->forks[i], DESTROY);
	use_mutex(&general->print_lock, DESTROY);
	use_mutex(&general->check_log, DESTROY);
	use_mutex(&general->init_mutex, DESTROY);
	use_mutex(&general->died_lock, DESTROY);
}

int	main(int ac, char *av[])
{
	t_general		general;
	pthread_t		*threads;
	unsigned int	i;

	if (ac == 5 || ac == 6)
	{
		init_struct(ac, av, &general);
		init_mutex(&general);
		threads = (pthread_t *)malloc(sizeof(pthread_t)
				* general.num_of_philos);
		i = -1;
		while (++i < general.num_of_philos + 1)
			pthread_create(&threads[i], NULL, &philosopher, &general);
		usleep(100000);
		general.threads_ready = true;
		i = -1;
		while (++i < general.num_of_philos)
			pthread_join(threads[i], NULL);
		destroy_mutex(&general);
		free(threads);
	}
	return (0);
}
