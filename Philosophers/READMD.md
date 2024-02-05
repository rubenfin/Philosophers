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






    #include "philosophers.h"

int	uneven_logic(t_general *general, t_philo *philosopher)
{
	if (philosopher->philo_num % 2 == 1 && !philosopher->full)
	{
		print_eating(general, philosopher);
		print_sleeping(general, philosopher);
	}
	else
	{
		print_thinking(general, philosopher);
		if (!philosopher->full)
			print_eating(general, philosopher);
		print_sleeping(general, philosopher);
	}
	return (1);
}

int	even_logic(t_general *general, t_philo *philosopher)
{
	if (philosopher->philo_num % 2 == 1 && !philosopher->full)
		print_eating(general, philosopher);
	else
		print_sleeping(general, philosopher);
	print_thinking(general, philosopher);
	if (philosopher->philo_num % 2 == 0 && !philosopher->full)
		print_eating(general, philosopher);
	else
		print_sleeping(general, philosopher);
	return (1);
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
	while (1)
	{
		if (general->num_of_philos == 1)
		{
			print_died(general, &philosopher);
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
		while (++i < general.num_of_philos)
			pthread_create(&threads[i], NULL, &philosopher, &general);
		general.threads_ready = true;
		i = -1;
		while (++i < general.num_of_philos)
			pthread_join(threads[i], NULL);
		destroy_mutex(&general);
		free(threads);
	}
	return (0);
}



#include "philosophers.h"

size_t	get_curr_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + ((time.tv_usec / 1000)));
}

unsigned int	get_time_passed(t_general *general)
{
	struct timeval	time2;
	long			time_passed;

	gettimeofday(&time2, NULL);
	time_passed = (((time2.tv_sec * 1000) - (general->time_started_sec * 1000))
			+ ((time2.tv_usec / 1000) - (general->time_started_usec / 1000)));
	return (time_passed);
}

int	get_sleep(size_t milliseconds)
{
	size_t	start;
	size_t	big;

	start = get_curr_time();
	big = milliseconds / 2;
	usleep(big * 1000);
	while ((get_curr_time() - start) < milliseconds)
		usleep(100);
	return (0);
}

long long	get_last_meal_time(t_general *general, t_philo philo)
{
	long long	last_meal;

	if (philo.msec_last_meal)
	{
		// printf("get_currtime: %zu & philo.msec_lastmeal: %lld\n",
		// get_curr_time(), philo.msec_last_meal);
		last_meal = get_curr_time() - philo.msec_last_meal;
	}
	else
	{
		// printf("timepassed:\n");
		last_meal = get_time_passed(general);
	}
	// printf("last meal fount %lld ago\n", last_meal);
	pthread_mutex_lock(&general->died_lock);
	if (general->time_to_die - last_meal < 0)
		general->terminate = true;
	if (philo.full)
		general->total_philos_full++;
	if (general->total_philos_full == general->num_of_philos)
		general->terminate = true;
	pthread_mutex_unlock(&general->died_lock);
	return (last_meal);
}

void	wait_for_threads(t_general *general)
{
	while (!general->threads_ready)
		;
}



#include "philosophers.h"

void	init_struct(int argc, char *argv[], t_general *general)
{
	struct timeval	time;
	unsigned int	i;

	general->num_of_philos = ft_atoi(argv[1]);
	general->time_to_die = ft_atoi(argv[2]);
	general->time_to_eat = ft_atoi(argv[3]);
	general->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		general->num_to_eat = ft_atoi(argv[5]);
	else
		general->num_to_eat = -1;
	gettimeofday(&time, NULL);
	general->time_started_sec = time.tv_sec;
	general->time_started_usec = time.tv_usec;
	general->terminate = false;
	general->threads_ready = false;
	i = 0;
	general->forks = malloc(sizeof(pthread_mutex_t) * general->num_of_philos);
	while (i < general->num_of_philos)
	{
		use_mutex(&general->forks[i], INIT);
		i++;
	}
	general->total_philos_full = 0;
}

void	init_philosopher(t_philo *philo, t_general *general, int i)
{
	philo->philo_num = i;
	philo->times_eaten = 0;
	philo->msec_last_meal = 0;
	philo->times_eaten = 0;
	philo->full = false;
	philo->r_fork = &general->forks[i - 1];
	if (philo->philo_num != general->num_of_philos)
		philo->l_fork = &general->forks[i];
	else
		philo->l_fork = &general->forks[0];
}



#include "philosophers.h"

void	*print_fork(t_general *general, t_philo *philosopher)
{
	pthread_mutex_lock(&general->died_lock);
	if (general->terminate)
	{
		pthread_mutex_unlock(&general->died_lock);
		return (NULL);
	}
	pthread_mutex_unlock(&general->died_lock);
	pthread_mutex_lock(&general->print_lock);
	printf("%u ", get_time_passed(general));
	printf("%u has taken a fork\n", philosopher->philo_num);
	pthread_mutex_unlock(&general->print_lock);
	return (NULL);
}

void	*print_eating(t_general *general, t_philo *philosopher)
{
	if (philosopher->philo_num % 2 == 1)
	{
		pthread_mutex_lock(philosopher->l_fork);
		printf("at ms%u philo : %d, has locked l_fork %p\n",
			get_time_passed(general), philosopher->philo_num,
			philosopher->l_fork);pthread_mutex_lock(&general->died_lock);
		print_fork(general, philosopher);
		pthread_mutex_lock(philosopher->r_fork);
		printf("at ms%u philo : %d, has locked r_fork %p\n",
			get_time_passed(general), philosopher->philo_num,
			philosopher->r_fork);
		print_fork(general, philosopher);
	}
	else
	{
		pthread_mutex_lock(philosopher->r_fork);
		printf("at ms%u philo : %d, has locked r_fork %p\n",
			get_time_passed(general), philosopher->philo_num,
			philosopher->r_fork);
		print_fork(general, philosopher);
		pthread_mutex_lock(philosopher->l_fork);
		printf("at ms%u philo : %d, has locked l_fork %p\n",
			get_time_passed(general), philosopher->philo_num,
			philosopher->l_fork);
		print_fork(general, philosopher);
	}
	pthread_mutex_lock(&general->init_mutex);
	philosopher->times_eaten++;
	if (philosopher->times_eaten == general->num_to_eat)
	{
		printf("\n\n%u is full, times eaten = %u\n\n",
		philosopher->philo_num, philosopher->times_eaten);
		philosopher->full = true;
	}
	pthread_mutex_unlock(&general->init_mutex);
	pthread_mutex_lock(&general->died_lock);
	if (general->terminate)
	{
		pthread_mutex_unlock(philosopher->r_fork);
		pthread_mutex_unlock(philosopher->l_fork);
		pthread_mutex_unlock(&general->died_lock);
		return (NULL);
	}
	pthread_mutex_unlock(&general->died_lock);
	pthread_mutex_lock(&general->print_lock);
	printf("%u ", get_time_passed(general));
	printf("%u is eating\n", philosopher->philo_num);
	pthread_mutex_unlock(&general->print_lock);
	philosopher->msec_last_meal = get_curr_time();
	get_sleep(general->time_to_eat);
	pthread_mutex_lock(&general->init_mutex);
	pthread_mutex_unlock(&general->init_mutex);
	pthread_mutex_unlock(philosopher->r_fork);
	printf("at ms%u philo : %d, has unlocked r_fork %p\n",
		get_time_passed(general), philosopher->philo_num, philosopher->r_fork);
	pthread_mutex_unlock(philosopher->l_fork);
	printf("at ms%u philo : %d, has unlocked l_fork %p\n",
		get_time_passed(general), philosopher->philo_num, philosopher->l_fork);
	return (NULL);
}

void	*print_sleeping(t_general *general, t_philo *philosopher)
{
	pthread_mutex_lock(&general->print_lock);
	printf("%u ", get_time_passed(general));
	printf("%u is sleeping\n", philosopher->philo_num);
	pthread_mutex_unlock(&general->print_lock);
	get_sleep(general->time_to_sleep);
	get_last_meal_time(general, *philosopher);
	return (NULL);
}

void	*print_thinking(t_general *general, t_philo *philosopher)
{
	pthread_mutex_lock(&general->died_lock);
	if (general->terminate)
	{
		pthread_mutex_unlock(&general->died_lock);
		return (NULL);
	}
	pthread_mutex_unlock(&general->died_lock);
	pthread_mutex_lock(&general->print_lock);
	printf("%u ", get_time_passed(general));
	printf("%u is thinking\n", philosopher->philo_num);
	pthread_mutex_unlock(&general->print_lock);
	return (NULL);
}

void	*print_died(t_general *general, t_philo *philosopher)
{
	static int	i;

	i++;
	if (i == 1)
	{
		pthread_mutex_lock(&general->print_lock);
		printf("%u ", get_time_passed(general));
		printf("%u died\n", philosopher->philo_num);
		pthread_mutex_unlock(&general->print_lock);
		return (NULL);
	}
	return (NULL);
}
