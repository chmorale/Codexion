/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmorale <chmorale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/28 12:02:13 by chmorale          #+#    #+#             */
/*   Updated: 2026/06/28 12:03:22 by chmorale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../lib/philo.h"

int	alloc(t_data *data)
{
	data->tid = malloc(sizeof(pthread_t) * data->coder_num);
	if (!data->tid)
		return (error(ALLOC_ERR_1, data));
	data->forks = malloc(sizeof(pthread_mutex_t) * data->coder_num);
	if (!data->forks)
		return (error(ALLOC_ERR_2, data));
	data->coders = malloc(sizeof(t_coder) * data->coder_num);
	if (!data->coders)
		return (error(ALLOC_ERR_3, data));
	return (0);
}

int	init_forks(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->philo_num)
		pthread_mutex_init(&data->forks[i], NULL);
	i = 0;
	data->coders[0].l_fork = &data->forks[0];
	data->coders[0].r_fork = &data->forks[data->coder_num - 1];
	i = 1;
	while (i < data->philo_num)
	{
		data->coders[i].l_fork = &data->forks[i];
		data->coders[i].r_fork = &data->forks[i - 1];
		i++;
	}
	return (0);
}

void	init_coders(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->philo_num)
	{
		data->coders[i].data = data;
		data->coders[i].id = i + 1;
		data->coders[i].time_to_die = data->death_time;
		data->coders[i].eat_cont = 0;
		data->coders[i].eating = 0;
		data->coders[i].status = 0;
		pthread_mutex_init(&data->coders[i].lock, NULL);
		i++;
	}
}

int	init_data(t_data *data, char **argv, int argc)
{
	data->coder_num = (int) ft_atoi(argv[1]);
	data->death_time = (u_int64_t) ft_atoi(argv[2]);
	data->compile_time = (u_int64_t) ft_atoi(argv[3]);
	data->refactor_time = (u_int64_t) ft_atoi(argv[4]);
	if (argc == 6)
		data->compile_nb = (int) ft_atoi(argv[5]);
	else
		data->compile_nb = -1;
	if (data->coder_num <= 0 || data->coder_num > 200 || data->death_time < 0
		|| data->compile_time < 0 || data->refactor_time < 0)
		return (error(ERR_IN_2, NULL));
	data->dead = 0;
	data->finished = 0;
	pthread_mutex_init(&data->write, NULL);
	pthread_mutex_init(&data->lock, NULL);
	return (0);
}

int	init(t_data *data, char **argv, int argc)
{
	if (init_data(data, argv, argc))
		return (1);
	if (alloc(data))
		return (1);
	if (init_forks(data))
		return (1);
	init_coders(data);
	return (0);
}
