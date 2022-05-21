/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohan <sohan@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/21 17:34:14 by sohan             #+#    #+#             */
/*   Updated: 2022/05/21 17:34:16 by sohan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <sys/time.h>
#include <stdio.h>

t_ull_int	get_ms_time(void)
{
	struct timeval	t;
	t_ull_int		ms_time;

	gettimeofday(&t, NULL);
	ms_time = t.tv_sec * 1000 + t.tv_usec / 1000;
	return (ms_time);
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
				(void *)life_of_philosoper, &philo[index]);
		if (ret != 0)
		{	
			printf("thread create failed.\n");
			return (-1);
		}
		++index;
	}
	return (0);
}

int	join_threads(t_philo *philo, t_arg *arg)
{
	int	index;
	int	num;
	int	ret;

	index = 0;
	num = philo[0].params.number_of_philosopher;
	ret = pthread_join(arg->end_monitor, NULL);
	if (ret != 0)
		return (-1);
	while (index < num)
	{
		ret = pthread_join(philo[index].philosopher, NULL);
		if (ret != 0)
		{
			printf("thread join failed\n");
			return (-1);
		}
		++index;
	}
	return (0);
}

void	destroy_mutexes(pthread_mutex_t *chopstick, \
						pthread_mutex_t *common, int num)
{
	int	i;

	i = 0;
	while (i < num)
	{
		pthread_mutex_destroy(&chopstick[i]);
		++i;
	}
	pthread_mutex_destroy(common);
}
