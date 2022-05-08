/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohan <sohan@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/06 16:04:48 by sohan             #+#    #+#             */
/*   Updated: 2022/05/06 16:04:52 by sohan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

//void	pickup_chopstick();
//void	putdown_chopstick();
//void	eat();
//void	sleep();
//void	think();

pthread_mutex_t	mu = PTHREAD_MUTEX_INITIALIZER;

void	*life_of_philosopher(void *arg)
{
	t_time	cur;
	t_philo	*philo;
	unsigned long long int	time;
	unsigned long long int	cur_time;
	int	ret_mutex;

	philo = arg;
	while (1)
	{
		ret_mutex = pthread_mutex_lock(philo->chopstick.left);
		gettimeofday(&cur, NULL);
		time = cur.tv_sec * 1000 + cur.tv_usec / 1000;
		printf("[%llu] philosopher %d has taken a left fork\n", time - philo->start_time, philo->philo_num);
		ret_mutex = pthread_mutex_lock(philo->chopstick.right);
		gettimeofday(&cur, NULL);
		time = cur.tv_sec * 1000 + cur.tv_usec / 1000;
		printf("[%llu] philosopher %d has taken a right fork\n", time - philo->start_time, philo->philo_num);
		gettimeofday(&cur, NULL);
		time = cur.tv_sec * 1000 + cur.tv_usec / 1000;
		printf("[%llu] philosopher %d is eating\n", time - philo->start_time, philo->philo_num);
		pthread_mutex_lock(&mu);
		philo->time_of_last_meal = time;
		pthread_mutex_unlock(&mu);
		usleep(philo->params.time_to_eat * 1000);
		++philo->eat;
		pthread_mutex_unlock(philo->chopstick.right);
		pthread_mutex_unlock(philo->chopstick.left);
		gettimeofday(&cur, NULL);
		time = cur.tv_sec * 1000 + cur.tv_usec / 1000;
		cur_time = time;
		printf("[%llu] philosopher %d is sleeping\n", time - philo->start_time, philo->philo_num);
		usleep(philo->params.time_to_sleep * 1000);
		gettimeofday(&cur, NULL);
		time = cur.tv_sec * 1000 + cur.tv_usec / 1000;
		printf("[%llu] philosopher %d is thinking\n", time - philo->start_time, philo->philo_num);
	}
	return (NULL);
}

void	*life_of_philosopher_alone(void *arg)
{
	t_time	cur;
	t_philo	*philo;
	unsigned long long int	time;
	int	ret_mutex;

	philo = arg;
	while (1)
	{
		//eat();
		ret_mutex = pthread_mutex_lock(philo->chopstick.right);
		gettimeofday(&cur, NULL);
		time = cur.tv_sec * 1000 + cur.tv_usec / 1000;
		printf("[%llu] philosopher %d has taken a right fork\n", time - philo->start_time, philo->philo_num);
		ret_mutex = pthread_mutex_lock(philo->chopstick.left);
		gettimeofday(&cur, NULL);
		time = cur.tv_sec * 1000 + cur.tv_usec / 1000;
		printf("[%llu] philosopher %d has taken a left fork\n", time - philo->start_time, philo->philo_num);
		gettimeofday(&cur, NULL);
		time = cur.tv_sec * 1000 + cur.tv_usec / 1000;
		printf("[%llu] philosopher %d is eating\n", time - philo->start_time, philo->philo_num);
		pthread_mutex_lock(&mu);
		philo->time_of_last_meal = time;
		pthread_mutex_unlock(&mu);
		usleep(philo->params.time_to_eat * 1000);
		++philo->eat;
		pthread_mutex_unlock(philo->chopstick.left);
		pthread_mutex_unlock(philo->chopstick.right);
		gettimeofday(&cur, NULL);
		time = cur.tv_sec * 1000 + cur.tv_usec / 1000;
		printf("[%llu] philosopher %d is sleeping\n", time - philo->start_time, philo->philo_num);
		usleep(philo->params.time_to_sleep * 1000);
		gettimeofday(&cur, NULL);
		time = cur.tv_sec * 1000 + cur.tv_usec / 1000;
		printf("[%llu] philosopher %d is thinking\n", time - philo->start_time, philo->philo_num);
		//think();
	}
	return (NULL);
}

void	*death_monitoring(void *arg)
{
	t_philo			*philo;
	int				index;
	struct timeval	t;
	unsigned long long int	time;

	index = 0;
	philo = arg;
	while (1)
	{
		gettimeofday(&t, NULL);
		time = t.tv_sec * 1000 + t.tv_usec / 1000;
		pthread_mutex_lock(&mu);
		if ((int)(time - philo[index].time_of_last_meal) >= philo[index].params.time_to_die)
		{
			gettimeofday(&t, NULL);
			printf("[%lld] philosopher %d died\n", (t.tv_sec * 1000) + (t.tv_usec / 1000) - philo->start_time, index + 1);
			break ;
		}
		pthread_mutex_unlock(&mu);
		++index;
		if (index == philo->params.number_of_philosopher)
			index = 0;
	}
	return ((void *)1);
}

void	init_time(t_philo *philo, int size)
{
	struct timeval t;
	int	i;
	
	i = 0;
	gettimeofday(&t, NULL);
	while (i < size)
	{	
		philo[i].start_time = (t.tv_sec * 1000) + (t.tv_usec / 1000);
		philo[i].time_of_last_meal = philo[i].start_time;
		philo[i].eat = 0;
		philo[i].philo_num = i + 1;
		++i;
	}
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

void	init_philo(t_philo *philo, t_param *param)
{
	int	i;
	int	num;

	i = 0;
	num = param->number_of_philosopher;
	while (i < num)
	{
		philo[i].params = *param;
		++i;
	}
}

int	main(int argc, char **argv)
{
	t_param			p;
	pthread_mutex_t	*chopstick;
	t_philo			*philo;
	int				index;
	pthread_t		death_monitor;
	void			*death = NULL;

	if (argc != 5 && argc != 6)
		return (-1);
	p.number_of_philosopher = atoi_safe(argv[1]);
	index = p.number_of_philosopher;
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
	init_philo(philo, &p);
	init_mutex(chopstick, philo);
	init_time(philo, p.number_of_philosopher);
	pthread_create(&death_monitor, NULL, death_monitoring, philo);
	--p.number_of_philosopher;
	while (p.number_of_philosopher > -1)
	{	
		if  (p.number_of_philosopher % 2)
			pthread_create(&philo[p.number_of_philosopher].philosopher, NULL, life_of_philosopher, &philo[p.number_of_philosopher]);
		else
			pthread_create(&philo[p.number_of_philosopher].philosopher, NULL, life_of_philosopher_alone, &philo[p.number_of_philosopher]);
		--p.number_of_philosopher;
	}
	//while (death != (void *)1){;};
	pthread_join(death_monitor, (void **)&death);
	//while (death != (void *)1){;};
	//for (int i = 0; i < index; i++)
	//	printf("philosopher %d has eat %d times.\n", philo[i].philo_num, philo[i].eat);
	//while (1){;};
	return (0);
}
