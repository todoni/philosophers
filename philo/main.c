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
#include <stdlib.h>

static int	get_philo_params(t_param *p, char **argv)
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
	if (p->number_of_philosopher < 1 || p->time_to_die < 1 || \
			p->time_to_eat < 1 || p->time_to_sleep < 1 || \
			p->number_of_philosopher > 8192)
		return (-1);
	return (0);
}

static int	init_mutex(pthread_mutex_t *chopstick, t_philo *philo)
{
	int	i;
	int	num;
	int	ret;

	i = 0;
	num = philo[0].params.number_of_philosopher;
	while (i < num)
	{
		ret = pthread_mutex_init(&chopstick[i], NULL);
		if (ret != 0)
			return (-1);
		++i;
	}
	i = 0;
	while (i < num)
	{
		philo[i].chopstick.left = &chopstick[i % num];
		philo[i].chopstick.right = &chopstick[(i + 1) % num];
		++i;
	}
	return (0);
}

static void	init_philo(t_philo *philo, t_param *p)
{
	int			index;
	t_ull_int	t;

	index = 0;
	t = get_ms_time();
	while (index < p->number_of_philosopher)
	{
		philo[index].philo_num = index + 1;
		philo[index].params = *p;
		philo[index].eat = 0;
		philo[index].eat_flag = 0;
		philo[index].start_time = t;
		philo[index].time_of_last_meal = t;
		philo[index].time_of_last_meal_end = t;
		philo[index].time_of_sleep_end = t;
		++index;
	}
}

static int	allocate_memories(t_philo **philo, \
					pthread_mutex_t **chopstick, int num)
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

	philo = NULL;
	chopstick = NULL;
	if (argc != 5 && argc != 6)
		return (-1);
	if (get_philo_params(&p, argv) == -1 || \
		allocate_memories(&philo, &chopstick, p.number_of_philosopher) == -1)
		return (-1);
	if (pthread_mutex_init(&arg.common, NULL) != 0)
		return (-1);
	p.common = &arg.common;
	arg.end = 1;
	p.flag_end = &arg.end;
	init_philo(philo, &p);
	if (init_mutex(chopstick, philo) == -1 || \
		create_threads(philo, &arg) == -1 || join_threads(philo, &arg) == -1)
		return (-1);
	destroy_mutexes(chopstick, &arg.common, p.number_of_philosopher);
	return (0);
}
