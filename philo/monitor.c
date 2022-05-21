/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohan <sohan@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/21 17:34:03 by sohan             #+#    #+#             */
/*   Updated: 2022/05/21 17:34:06 by sohan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>
#include <unistd.h>

static int	is_philo_full(t_philo *philo)
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

static int	is_philo_dead(t_philo *philo)
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

	philo = arg;
	while (1)
	{
		if (is_philo_dead(philo) || is_philo_full(philo))
			break ;
		usleep(600);
	}
	return ((void *)0);
}
