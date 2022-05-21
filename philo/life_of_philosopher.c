/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   life_of_philosopher.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohan <sohan@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/21 17:33:52 by sohan             #+#    #+#             */
/*   Updated: 2022/05/21 17:33:54 by sohan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>

static void	eat(t_philo *philo)
{
	t_ull_int	cur_time;

	cur_time = philo->time_of_last_meal;
	while ((int)cur_time - (int)philo->time_of_last_meal < \
			philo->params.time_to_eat)
	{
		cur_time = get_ms_time();
		usleep(600);
	}
	philo->time_of_last_meal_end = cur_time;
}

static void	sleepp(t_philo *philo)
{
	t_ull_int	cur_time;

	print_philo_state(philo, "is sleeping");
	cur_time = philo->time_of_last_meal_end;
	while ((int)cur_time - (int)philo->time_of_last_meal_end < \
			philo->params.time_to_sleep)
	{
		cur_time = get_ms_time();
		usleep(600);
	}
	philo->time_of_sleep_end = cur_time;
}

static int	is_end(t_philo *philo)
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

static void	eating(t_philo *philo)
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

void	*life_of_philosoper(t_philo *philo)
{
	if (philo->philo_num % 2)
		usleep(600);
	while (1)
	{
		if (is_end(philo))
			break ;
		pthread_mutex_lock(philo->chopstick.left);
		print_philo_state(philo, "has taken a fork");
		if (philo->params.number_of_philosopher == 1)
		{
			pthread_mutex_unlock(philo->chopstick.left);
			break ;
		}
		pthread_mutex_lock(philo->chopstick.right);
		print_philo_state(philo, "has taken a fork");
		eating(philo);
		pthread_mutex_unlock(philo->chopstick.left);
		pthread_mutex_unlock(philo->chopstick.right);
		sleepp(philo);
		print_philo_state(philo, "is thinking");
		usleep(600);
	}
	return (NULL);
}
