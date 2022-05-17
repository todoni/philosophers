/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohan <sohan@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/06 16:04:34 by sohan             #+#    #+#             */
/*   Updated: 2022/05/13 18:22:59 by sohan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <sys/time.h>

typedef struct timeval	t_time;

typedef struct s_chopstick_mutex
{
	pthread_mutex_t	*left;
	pthread_mutex_t	*right;
}	t_chopstick;

typedef struct s_param
{
	int	number_of_philosopher;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	number_of_times_philo_must_eat;
	pthread_mutex_t				*common;
	pthread_mutex_t				*print;
}	t_param;

typedef struct s_philo
{
	pthread_t				philosopher;
	int						philo_num;
	unsigned long long int	start_time;
	unsigned long long int	time_of_last_meal;
	unsigned long long int	time_of_last_meal_end;
	unsigned long long int	time_of_sleep_end;
	t_chopstick				chopstick;
	t_param					params;
}	t_philo;

int	atoi_safe(const char *str);

#endif
