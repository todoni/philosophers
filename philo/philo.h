/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sohan <sohan@student.42seoul.kr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/06 16:04:34 by sohan             #+#    #+#             */
/*   Updated: 2022/05/06 16:04:44 by sohan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <sys/time.h>

typedef struct timeval	t_time;

typedef struct s_fork_mutex
{
	pthread_mutex_t	right_fork;
	pthread_mutex_t	left_fork;
}	t_fork;

typedef struct s_philo
{
	pthread_t	philosopher;
	int			num_philosopher;
	int			start_time;
	int			time_of_last_meal;
	t_fork		philo_fork;
}	t_philo;

typedef struct s_node
{
	struct s_node	*left;
	t_philo			philo;
	struct s_node	*right;
}	t_node;

typedef struct s_param
{
	int	number_of_philosopher;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
}	t_param;

int	atoi_safe(const char *str);

#endif
