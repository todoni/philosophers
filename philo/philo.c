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

unsigned long long	get_ms_time(void)
{
	struct timeval			t;
	unsigned long long int	ms_time;

	gettimeofday(&t, NULL);
	ms_time = t.tv_sec * 1000 + t.tv_usec / 1000;
	return (ms_time);
}

void	eat(void *arg)
{
	unsigned long long int	time;
	t_philo			*philo;

	philo = arg;
	time = philo->time_of_last_meal;
	while ((int)time - (int)philo->time_of_last_meal < philo->params.time_to_eat)
	{
		time = get_ms_time();
		usleep(600);
	}
	philo->time_of_last_meal_end = time;
}

void	sleepp(void *arg)
{
	unsigned long long int	cur_time;
	t_philo			*philo;

	philo = arg;
	cur_time = philo->time_of_last_meal_end;
	while ((int)cur_time - (int)philo->time_of_last_meal_end < philo->params.time_to_sleep)
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
	while (index < num)
	{
		ret += philo[index].eat_flag;
		++index;
	}
	if (ret == num)
	{
		//*philo[0].params.flag_end = 0;
		return (1);
	}
	return (0);
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
		gettimeofday(&t, NULL);
		time = t.tv_sec * 1000 + t.tv_usec / 1000;
		ret_mutex = pthread_mutex_lock(philo[index].params.common);
		if ((int)(time - philo[index].time_of_last_meal) >= philo[index].params.time_to_die)
		{
			printf("\033[0;31m[%lld] philosopher %d died[%d]\n\033[0m", \
					time - philo[index].start_time, index + 1, ret_mutex);

			pthread_mutex_unlock(philo[index].params.common);
			break ;
		}
		if (is_philo_full(philo))
		{	pthread_mutex_unlock(philo[index].params.common);
			//return ((void *)1);
			break ;
			}
		pthread_mutex_unlock(philo[index].params.common);
		++index;
		if (index == num)
			index = 0;
		usleep(600);
	}
	return ((void *)0);
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
	unsigned long long int	t;
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

void	print_philo_state(t_philo *philo, const char *msg)
{
	int	ret_mutex;
	
	ret_mutex = pthread_mutex_lock(philo->params.common);
	if (ret_mutex == 0 && *philo->params.flag_end)
	//{
		printf("[%llu] philosopher %d %s[%d]\n", get_ms_time() - philo->start_time, philo->philo_num, msg, ret_mutex);
	pthread_mutex_unlock(philo->params.common);
	//}
}

void	*life_of_philosoper(void *arg)
{
	t_philo	*philo;

	philo = arg;
	if (philo->philo_num % 2)
		usleep(600);
	while (1)
	{
		pthread_mutex_lock(philo->params.common);
		if (*philo->params.flag_end == 0)
		{	
			
			pthread_mutex_unlock(philo->params.common);
			break;
		}
		pthread_mutex_unlock(philo->params.common);
		pthread_mutex_lock(philo->chopstick.left);
		print_philo_state(philo, "has taken a fork");
		pthread_mutex_lock(philo->chopstick.right);
		print_philo_state(philo, "has taken a fork");
		print_philo_state(philo, "is eating");
		pthread_mutex_lock(philo->params.common);
		philo->time_of_last_meal = get_ms_time();
		pthread_mutex_unlock(philo->params.common);
		eat(arg);
		pthread_mutex_lock(philo->params.common);
		++philo->eat;
		if (philo->eat == philo->params.number_of_times_philo_must_eat)
			philo->eat_flag = 1;
		pthread_mutex_unlock(philo->params.common);
		pthread_mutex_unlock(philo->chopstick.left);
		pthread_mutex_unlock(philo->chopstick.right);
		print_philo_state(philo, "is sleeping");
		sleepp(arg);
		print_philo_state(philo, "is thinking");
		usleep(200);
	}
	printf("bye\n");
	return (NULL);
}

int	main(int argc, char **argv)
{
	t_param			p;
	pthread_mutex_t	*chopstick;
	t_philo			*philo;
	int				index = 0;
	int				end;
	pthread_t		death_monitor;
	pthread_mutex_t	common;

	if (argc != 5 && argc != 6)
		return (-1);
	p.number_of_philosopher = atoi_safe(argv[1]);
	p.time_to_die = atoi_safe(argv[2]);
	p.time_to_eat = atoi_safe(argv[3]);
	p.time_to_sleep = atoi_safe(argv[4]);
	if (argv[5])
	{
		p.number_of_times_philo_must_eat = atoi_safe(argv[5]);
		if (p.number_of_times_philo_must_eat < 1)
			return (-1);
	}
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
	p.flag_end = &end;
	init_philo(philo, &p);
	init_mutex(chopstick, philo);
	init_time(philo);
	pthread_create(&death_monitor, NULL, death_monitoring, philo);
	end = 1;
	while (index < p.number_of_philosopher)
	{
		pthread_create(&philo[index].philosopher, NULL, life_of_philosoper, &philo[index]);
		//pthread_detach(philo[index].philosopher);
		++index;
	}
	index = 0;
	/*while (index < p.number_of_philosopher)
	{
		pthread_join(philo[index].philosopher, NULL);
		++index;
	}*/
	pthread_join(death_monitor, NULL);
	pthread_mutex_lock(&common);
	end = 0;
	pthread_mutex_unlock(&common);
	printf("hihihihihi\n\n\n\n\n");
	while (index < p.number_of_philosopher)
	{
		pthread_join(philo[index].philosopher, NULL);
		++index;
	}
	return (0);
}
