/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohan <sohan@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/06 16:04:48 by sohan             #+#    #+#             */
/*   Updated: 2022/05/13 18:34:53 by sohan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

t_ull_int	get_ms_time(void)
{
	struct timeval	t;
	t_ull_int		ms_time;

	gettimeofday(&t, NULL);
	ms_time = t.tv_sec * 1000 + t.tv_usec / 1000;
	return (ms_time);
}

void	eat(void *arg)
{
	t_ull_int	time;
	t_philo		*philo;

	philo = arg;
	time = philo->time_of_last_meal;
	while ((int)time - (int)philo->time_of_last_meal < \
			philo->params.time_to_eat)
	{
		time = get_ms_time();
		usleep(600);
	}
	philo->time_of_last_meal_end = time;
}

void	sleepp(void *arg)
{
	t_ull_int	cur_time;
	t_philo		*philo;

	philo = arg;
	cur_time = philo->time_of_last_meal_end;
	while ((int)cur_time - (int)philo->time_of_last_meal_end < \
			philo->params.time_to_sleep)
	{
		cur_time = get_ms_time();
		usleep(600);
	}
	philo->time_of_sleep_end = cur_time;
}

int	is_philo_full(t_philo *philo)
{
	int	index;
	int	num;
	int	ret;

	index = 0;
	ret = 0;
	num = philo[0].params.number_of_philosopher;
	pthread_mutex_lock(philo[0].params.common);
	while (index < num)
	{
		ret += philo[index].eat_flag;
		++index;
	}
	if (ret == num)
	{	
		*philo[0].params.flag_end = 0;
		pthread_mutex_unlock(philo[0].params.common);
		return (1);
	}
	pthread_mutex_unlock(philo[0].params.common);
	return (0);
}

int	is_philo_dead(t_philo *philo)
{
	int			index;
	int			num;
	t_ull_int	time;

	index = 0;
	num = philo[0].params.number_of_philosopher;
	while (index < num)
	{
		pthread_mutex_lock(philo[index].params.common);
		time = get_ms_time();
		if ((int)(time - philo[index].time_of_last_meal) >= \
							philo[index].params.time_to_die)
		{
			printf("\033[0;31m[%lld] philosopher %d died\n\033[0m", \
						time - philo[index].start_time, index + 1);
			*philo[index].params.flag_end = 0;
			pthread_mutex_unlock(philo[index].params.common);
			pthread_mutex_unlock(philo[index].chopstick.left);
			return (1);
		}
		pthread_mutex_unlock(philo[index].params.common);
		++index;
	}
	return (0);
}

void	*end_monitoring(void *arg)
{
	t_philo		*philo;
	int			index;
	int			ret_mutex;
	int			num;

	index = 0;
	ret_mutex = 0;
	philo = arg;
	num = philo[index].params.number_of_philosopher;
	while (1)
	{
		if (is_philo_dead(philo) || is_philo_full(philo))
			break ;
		usleep(600);
	}
	return ((void *)0);
}

void	print_philo_state(t_philo *philo, const char *msg)
{
	int	ret_mutex;
	
	ret_mutex = pthread_mutex_lock(philo->params.common);
	if (ret_mutex == 0 && *philo->params.flag_end)
		printf("[%llu] philosopher %d %s\n", get_ms_time() - philo->start_time, \
				philo->philo_num, msg);
	pthread_mutex_unlock(philo->params.common);
}

int	is_end(t_philo *philo)
{
	pthread_mutex_lock(philo->params.common);
	if (*philo->params.flag_end == 0)
	{	
		pthread_mutex_unlock(philo->params.common);
		return (1);
	}
	pthread_mutex_unlock(philo->params.common);
	return (0);
}

void	eating(t_philo *philo)
{
	print_philo_state(philo, "is eating");
	pthread_mutex_lock(philo->params.common);
	philo->time_of_last_meal = get_ms_time();
	pthread_mutex_unlock(philo->params.common);
	eat(philo);
	pthread_mutex_lock(philo->params.common);
	++philo->eat;
	if (philo->eat == philo->params.number_of_times_philo_must_eat)
		philo->eat_flag = 1;
	pthread_mutex_unlock(philo->params.common);
}

void	*life_of_philosoper(void *arg)
{
	t_philo	*philo;

	philo = arg;
	if (philo->philo_num % 2)
		usleep(600);
	while (1)
	{
		if (is_end(philo))
			break;
		pthread_mutex_lock(philo->chopstick.left);
		print_philo_state(philo, "has taken a fork");
		pthread_mutex_lock(philo->chopstick.right);
		print_philo_state(philo, "has taken a fork");
		eating(philo);
		pthread_mutex_unlock(philo->chopstick.left);
		pthread_mutex_unlock(philo->chopstick.right);
		print_philo_state(philo, "is sleeping");
		sleepp(arg);
		print_philo_state(philo, "is thinking");
		usleep(600);
	}
	return (NULL);
}

int	get_philo_params(t_param *p, char **argv)
{
	p->number_of_philosopher = atoi_safe(argv[1]);
	p->time_to_die = atoi_safe(argv[2]);
	p->time_to_eat = atoi_safe(argv[3]);
	p->time_to_sleep = atoi_safe(argv[4]);
	if (argv[5])
	{
		p->number_of_times_philo_must_eat = atoi_safe(argv[5]);
		if (p->number_of_times_philo_must_eat < 1)
			return (-1);
	}
	if (p->number_of_philosopher < 1 || p->time_to_die < 1 ||\
			p->time_to_eat < 1 || p->time_to_sleep < 1)
		return (-1);
	return (0);
}

void	init_mutex(pthread_mutex_t *chopstick, t_philo *philo)
{
	int	i;
	int	num;

	i = 0;
	num = philo[0].params.number_of_philosopher;
	while (i < num)
	{
		pthread_mutex_init(&chopstick[i], NULL);
		++i;
	}
	i = 0;
	while (i < num)
	{
		philo[i].chopstick.left = &chopstick[i % num];
		philo[i].chopstick.right = &chopstick[(i + 1)  % num];
		++i;
	}
}

void	init_philo(t_philo *philo, t_param *p)
{
	int	index;

	index = 0;
	while(index < p->number_of_philosopher)
	{
		philo[index].philo_num = index + 1;
		philo[index].params = *p;
		philo[index].eat = 0;
		philo[index].eat_flag = 0;
		++index;
	}
}

void	init_time(t_philo *philo)
{
	int		index;
	t_ull_int	t;
	int		i;

	index = 0;
	i = philo[0].params.number_of_philosopher;
	t = get_ms_time();
	while (index < i)
	{
		philo[index].start_time = t;
		philo[index].time_of_last_meal = t;
		philo[index].time_of_last_meal_end = t;
		philo[index].time_of_sleep_end = t;
		++index;
	}
}

int	create_threads(t_philo *philo, t_arg *arg)
{
	int	index;
	int	num;
	int	ret;

	index = 0;
	num = philo[0].params.number_of_philosopher;
	ret = pthread_create(&arg->end_monitor, NULL, end_monitoring, philo);
	if (ret != 0)
		return (-1);
	while (index < num)
	{
		ret = pthread_create(&philo[index].philosopher, NULL, \
				life_of_philosoper, &philo[index]);
		if (ret != 0)
			return (-1);
		++index;
	}
	return (0);
}

void	join_threads(t_philo *philo, t_arg *arg)
{
	int	index;
	int	num;

	index = 0;
	num = philo[0].params.number_of_philosopher;
	pthread_join(arg->end_monitor, NULL);
	while (index < num)
	{
		pthread_join(philo[index].philosopher, NULL);
		++index;
	}
}

int	allocate_memories(t_philo **philo, pthread_mutex_t **chopstick, int num)
{
	*philo = malloc(num * sizeof(t_philo));
	if (!*philo)
		return (-1);
	*chopstick = malloc(num * sizeof(pthread_mutex_t));
	if (!*chopstick)
		return (-1);
	return (0);
}

int	main(int argc, char **argv)
{
	t_param			p;
	t_arg			arg;
	pthread_mutex_t	*chopstick;
	t_philo			*philo;

	if (argc != 5 && argc != 6)
		return (-1);
	if (get_philo_params(&p, argv) == -1)
		return (-1);
	philo = NULL;
	chopstick = NULL;
	if (allocate_memories(&philo, &chopstick, p.number_of_philosopher) == -1)
		return (-1);
	pthread_mutex_init(&arg.common, NULL);
	p.common = &arg.common;
	p.flag_end = &arg.end;
	init_philo(philo, &p);
	init_time(philo);
	init_mutex(chopstick, philo);
	arg.end = 1;
	if (create_threads(philo, &arg) == -1)
		return (-1);
	join_threads(philo, &arg);
	return (0);
}
