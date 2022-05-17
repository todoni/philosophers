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
	//gettimeofday(&cur, NULL);
	//time = cur.tv_sec * 1000 + cur.tv_usec / 1000;
	time = philo->time_of_last_meal;
	while ((int)time - (int)philo->time_of_last_meal < philo->params.time_to_eat)
	{
		gettimeofday(&cur, NULL);
		time = cur.tv_sec * 1000 + cur.tv_usec / 1000;
	}
	philo->time_of_last_meal_end = time;
}

void	sleepp(void *arg)
{
	t_time			cur;
	unsigned long long int	cur_time;
	t_philo			*philo;

	philo = arg;
	//gettimeofday(&cur, NULL);
	//cur_time = cur.tv_sec * 1000 + cur.tv_usec / 1000;
	cur_time = philo->time_of_last_meal_end;
	while ((int)cur_time - (int)philo->time_of_last_meal_end < philo->params.time_to_sleep)
	{
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
	int	num;

	index = 0;
	philo = arg;
	num = philo[index].params.number_of_philosopher;
	while (1)
	{
		ret_mutex = pthread_mutex_lock(philo[index].params.common);
		gettimeofday(&t, NULL);
		time = t.tv_sec * 1000 + t.tv_usec / 1000;
		if ((int)(time - philo[index].time_of_last_meal) >= philo[index].params.time_to_die)
		{
			printf("\033[0;31m[%lld] philosopher %d died[%d]\n\033[0m", time - philo[index].start_time, index + 1, ret_mutex);
			break ;
		}
		pthread_mutex_unlock(philo[index].params.common);
		++index;
		if (index == num)
			index = 0;
	}
	return ((void *)1);
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
		++index;
	}
}

void	init_time(t_philo *philo)
{
	int		index;
	t_time	time;
	unsigned long long int	t;
	int		i;

	index = 0;
	i = philo[0].params.number_of_philosopher;
	gettimeofday(&time, NULL);
	t = time.tv_sec * 1000 + time.tv_usec / 1000;
	while (index < i)
	{
		philo[index].start_time = t;
		philo[index].time_of_last_meal = t;
		philo[index].time_of_last_meal_end = t;
		philo[index].time_of_sleep_end = t;
		++index;
	}
}

void	*life_of_philosoper_odd(void *arg)
{
	t_time	cur;
	t_philo	*philo;
	unsigned long long int	time;

	philo = arg;
	while (1)
	{
		pthread_mutex_lock(philo->chopstick.left);
		pthread_mutex_lock(philo->params.common);
		gettimeofday(&cur, NULL);
		time = cur.tv_sec * 1000 + cur.tv_usec / 1000;
		printf("[%llu] philosopher %d has taken a left fork\n", time - philo->start_time, philo->philo_num);
		pthread_mutex_unlock(philo->params.common);
		pthread_mutex_lock(philo->chopstick.right);
		pthread_mutex_lock(philo->params.common);
		gettimeofday(&cur, NULL);
		time = cur.tv_sec * 1000 + cur.tv_usec / 1000;
		printf("[%llu] philosopher %d has taken a right fork\n", time - philo->start_time, philo->philo_num);
		printf("[%llu] philosopher %d is eating\n", time - philo->start_time, philo->philo_num);
		philo->time_of_last_meal = time;
		pthread_mutex_unlock(philo->params.common);
		eat(arg);
		pthread_mutex_unlock(philo->chopstick.right);
		pthread_mutex_unlock(philo->chopstick.left);
		pthread_mutex_lock(philo->params.common);
		gettimeofday(&cur, NULL);
		time = cur.tv_sec * 1000 + cur.tv_usec / 1000;
		//printf("[%llu] philosopher %d is sleeping\n", philo->time_of_last_meal_end - philo->start_time, philo->philo_num);
		printf("[%llu] philosopher %d is sleeping\n", time - philo->start_time, philo->philo_num);
		pthread_mutex_unlock(philo->params.common);
		sleepp(arg);
		pthread_mutex_lock(philo->params.common);
		gettimeofday(&cur, NULL);
		time = cur.tv_sec * 1000 + cur.tv_usec / 1000;
		//printf("[%llu] philosopher %d is thinking\n", philo->time_of_sleep_end - philo->start_time, philo->philo_num);
		printf("[%llu] philosopher %d is thinking\n", time - philo->start_time, philo->philo_num);
		pthread_mutex_unlock(philo->params.common);
	}
	return (NULL);
}

void	*life_of_philosoper_even(void *arg)
{
	t_time	cur;
	t_philo	*philo;
	unsigned long long int	time;

	philo = arg;
	while (1)
	{
		pthread_mutex_lock(philo->chopstick.right);
		pthread_mutex_lock(philo->params.common);
		gettimeofday(&cur, NULL);
		time = cur.tv_sec * 1000 + cur.tv_usec / 1000;
		printf("[%llu] philosopher %d has taken a right fork\n", time - philo->start_time, philo->philo_num);
		pthread_mutex_unlock(philo->params.common);
		pthread_mutex_lock(philo->chopstick.left);
		pthread_mutex_lock(philo->params.common);
		gettimeofday(&cur, NULL);
		time = cur.tv_sec * 1000 + cur.tv_usec / 1000;
		printf("[%llu] philosopher %d has taken a left fork\n", time - philo->start_time, philo->philo_num);
		printf("[%llu] philosopher %d is eating\n", time - philo->start_time, philo->philo_num);
		philo->time_of_last_meal = time;
		pthread_mutex_unlock(philo->params.common);
		eat(arg);
		pthread_mutex_unlock(philo->chopstick.right);
		pthread_mutex_unlock(philo->chopstick.left);
		pthread_mutex_lock(philo->params.common);
		gettimeofday(&cur, NULL);
		time = cur.tv_sec * 1000 + cur.tv_usec / 1000;
		//printf("[%llu] philosopher %d is sleeping\n", philo->time_of_last_meal_end - philo->start_time, philo->philo_num);
		printf("[%llu] philosopher %d is sleeping\n", time - philo->start_time, philo->philo_num);
		pthread_mutex_unlock(philo->params.common);
		sleepp(arg);
		pthread_mutex_lock(philo->params.common);
		gettimeofday(&cur, NULL);
		time = cur.tv_sec * 1000 + cur.tv_usec / 1000;
		//printf("[%llu] philosopher %d is thinking\n", philo->time_of_sleep_end - philo->start_time, philo->philo_num);
		printf("[%llu] philosopher %d is thinking\n", time - philo->start_time, philo->philo_num);
		pthread_mutex_unlock(philo->params.common);
	}
	return (NULL);
}

int	main(int argc, char **argv)
{
	t_param			p;
	pthread_mutex_t	*chopstick;
	t_philo			*philo;
	int				index = 0;
	pthread_t		death_monitor;
	pthread_mutex_t	common;

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
	pthread_mutex_init(&common, NULL);
	p.common = &common;
	init_philo(philo, &p);
	init_mutex(chopstick, philo);
	init_time(philo);
	pthread_create(&death_monitor, NULL, death_monitoring, philo);
	while (index < p.number_of_philosopher)
	{
		if (index % 2)
			pthread_create(&philo[index].philosopher, NULL, life_of_philosoper_odd, &philo[index]);
		else
			pthread_create(&philo[index].philosopher, NULL, life_of_philosoper_even, &philo[index]);
		++index;
		//pthread_detach(philo[index].philosopher);
		//usleep(1000);
	}
	pthread_join(death_monitor, NULL);
	return (0);
}
