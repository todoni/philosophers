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

//void	pickup_chopstick();
//void	putdown_chopstick();
void	eat();
//void	sleep();
//void	think();

void	eat(void *arg)
{
	t_time			cur;
	unsigned long long int	time;
	t_philo			*philo;

	philo = arg;
	pthread_mutex_lock(philo->common);
	gettimeofday(&cur, NULL);
	time = cur.tv_sec * 1000 + cur.tv_usec / 1000;
	pthread_mutex_unlock(philo->common);
	while ((int)time - (int)philo->time_of_last_meal < philo->params.time_to_eat)
	{
		usleep(1);
		gettimeofday(&cur, NULL);
		time = cur.tv_sec * 1000 + cur.tv_usec / 1000;
	}
	philo->time_of_last_meal_end = time;
}

void	sleepp(void *arg)
{
	t_time			cur;
	//unsigned long long int	time;
	unsigned long long int	cur_time;
	t_philo			*philo;

	philo = arg;
	pthread_mutex_lock(philo->common);
	gettimeofday(&cur, NULL);
	cur_time = cur.tv_sec * 1000 + cur.tv_usec / 1000;
	pthread_mutex_unlock(philo->common);
	//cur_time = time;
	while ((int)cur_time - (int)philo->time_of_last_meal_end < philo->params.time_to_sleep)
	{
		usleep(1);
		gettimeofday(&cur, NULL);
		cur_time = cur.tv_sec * 1000 + cur.tv_usec / 1000;
	}
	philo->time_of_sleep_end = cur_time;
}

void	*death_monitoring(void *arg)
{
	t_philo			*philo;
	int				index;
	struct timeval	t;
	unsigned long long int	time;
	int	ret_mutex = 0;
	pthread_mutex_t	temp;

	index = 0;
	philo = arg;
	pthread_mutex_init(&temp, NULL);
	while (1)
	{
		gettimeofday(&t, NULL);
		time = t.tv_sec * 1000 + t.tv_usec / 1000;
		ret_mutex = pthread_mutex_lock(philo[index].common);
		if ((int)(time - philo[index].time_of_last_meal) >= philo[index].params.time_to_die || philo->death == 1)
		{
			//gettimeofday(&t, NULL);
			pthread_mutex_lock(philo[index].print);
			printf("\033[0;31m[%lld] philosopher %d died[%d]\n\033[0m", time - philo->start_time, index + 1, ret_mutex);
			//usleep(100);
			break ;
		}
		pthread_mutex_unlock(philo[index].common);
		//pthread_mutex_unlock(philo[index].common);
		++index;
		if (index == philo->params.number_of_philosopher)
			index = 0;
		//usleep(10);
	}
	return ((void *)1);
}

void	init_mutex(pthread_mutex_t *chopstick, t_philo *philo)
{
	int	i;
	int	num;

	i = 0;
	num = philo->params.number_of_philosopher;
	while (i < philo->params.number_of_philosopher)
	{
		pthread_mutex_init(&chopstick[i], NULL);
		++i;
	}
	i = 0;
	while (i < philo->params.number_of_philosopher)
	{
		philo[i].chopstick.left = &chopstick[i % num];
		philo[i].chopstick.right = &chopstick[(i + 1)  % num];
		++i;
	}
}

int	main(int argc, char **argv)
{
	t_param			p;
	pthread_mutex_t	*chopstick;
	t_philo			*philo;
	//int				index;
	pthread_t		death_monitor;
	void			*death = NULL;

	if (argc != 5 && argc != 6)
		return (-1);
	p.number_of_philosopher = atoi_safe(argv[1]);
	//index = p.number_of_philosopher;
	p.time_to_die = atoi_safe(argv[2]);
	p.time_to_eat = atoi_safe(argv[3]);
	p.time_to_sleep = atoi_safe(argv[4]);
	if (argv[5])
		p.number_of_times_philo_must_eat = atoi_safe(argv[5]);
	if (p.number_of_philosopher < 1 || p.time_to_die < 1 ||\
			p.time_to_eat < 1 || p.time_to_sleep < 1)
		return (-1);
	philo = malloc(p.number_of_philosopher * sizeof(t_philo));
	if (!philo)
		return (-1);
	chopstick = malloc(p.number_of_philosopher * sizeof(pthread_mutex_t));
	if (!chopstick)
		return (-1);
	return (0);
}
