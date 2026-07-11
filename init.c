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

#include "codex.h"


int	alloc(t_data *data)
{
	data->tid = malloc(sizeof(pthread_t) * data->coder_num);
	if (!data->tid)
		return (error(ALLOC_ERR_1, data));
	data->dongles = malloc(sizeof(pthread_mutex_t) * data->coder_num);
	if (!data->dongles)
		return (error(ALLOC_ERR_2, data));
	data->coders = malloc(sizeof(t_coder) * data->coder_num);
	if (!data->coders)
		return (error(ALLOC_ERR_3, data));
	return (0);
}

int	init_dongles(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->coder_num)
		pthread_mutex_init(&data->dongles[i], NULL);
	i = 0;
	data->coders[0].l_dongle = &data->dongles[0];
	data->coders[0].r_dongle = &data->dongles[data->coder_num - 1];
	i = 1;
	while (i < data->coder_num)
	{
		data->coders[i].l_dongle = &data->dongles[i];
		data->coders[i].r_dongle = &data->dongles[i - 1];
		i++;
	}
	return (0);
}

void	init_coders(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->coder_num)
	{
		data->coders[i].data = data;
		data->coders[i].id = i + 1;
		data->coders[i].time_to_burnout = data->burnout_time;
		data->coders[i].compile_cont = 0;
		data->coders[i].compiling = 0;
		data->coders[i].status = 0;
		pthread_mutex_init(&data->coders[i].lock, NULL);
		i++;
	}
}

int	init_data(t_data *data, char **argv)
{
	data->coder_num = (int) ft_atoi(argv[1]);
	data->burnout_time = (u_int64_t) ft_atoi(argv[2]);
	data->compile_time = (u_int64_t) ft_atoi(argv[3]);
	data->debug_time = (u_int64_t) ft_atoi(argv[4]);
	data->refactor_time = (u_int64_t) ft_atoi(argv[5]);
	data->compiles_required = (int) ft_atoi(argv[6]);
    data->dongle_cooldown = (u_int64_t) ft_atoi(argv[7]);
  	data->scheduler = (char*) (argv[8]);
	// if (data->coder_num <= 0 || data->coder_num > 200 || data->burnout_time < 0
	//	|| data->compile_time < 0 || data->refactor_time < 0)
	if  (data->coder_num <= 0 || data->coder_num > 200)
		return (error(ERR_IN_2, NULL));
	data->burnout = 0;
	data->finished = 0;
	pthread_mutex_init(&data->write, NULL);
	pthread_mutex_init(&data->lock, NULL);
	return (0);
}

int	init(t_data *data, char **argv)
{
	if (init_data(data, argv))
		return (1);
	if (alloc(data))
		return (1);
	if (init_dongles(data))
		return (1);
	init_coders(data);
	return (0);
}
