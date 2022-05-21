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

typedef unsigned long long int	t_ull_int;

typedef struct s_chopstick_mutex
{
	pthread_mutex_t	*left;
	pthread_mutex_t	*right;
}	t_chopstick;

typedef struct s_common
{
	int				end;
	pthread_mutex_t	common;
	pthread_t		end_monitor;
}	t_arg;

typedef struct s_param
{
	int				number_of_philosopher;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				number_of_times_philo_must_eat;
	int				*flag_end;
	pthread_mutex_t	*common;
}	t_param;

typedef struct s_philo
{
	pthread_t	philosopher;
	int			philo_num;
	t_ull_int	start_time;
	t_ull_int	time_of_last_meal;
	t_ull_int	time_of_last_meal_end;
	t_ull_int	time_of_sleep_end;
	t_chopstick	chopstick;
	t_param		params;
	int			eat;
	int			eat_flag;
}	t_philo;

int			atoi_safe(const char *str);
int			create_threads(t_philo *philo, t_arg *arg);
int			join_threads(t_philo *philo, t_arg *arg);
void		destroy_mutexes(pthread_mutex_t *chopstick, \
							pthread_mutex_t *common, int num);
t_ull_int	get_ms_time(void);
void		print_philo_state(t_philo *philo, const char *arg);
void		*end_monitoring(void *arg);
void		*life_of_philosoper(t_philo *philo);

#endif
