/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmorale <chmorale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 09:56:14 by chmorale          #+#    #+#             */
/*   Updated: 2026/07/19 00:00:00 by chmorale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codex.h"

static int	create_all(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->coder_num)
	{
		if (pthread_create(&data->coders[i].thread_id, NULL,
				routine, &data->coders[i]) != 0)
			return (1);
		i++;
	}
	return (0);
}

static int	scan_coders(t_data *data)
{
	int	i;
	int	all_done;

	i = 0;
	all_done = 1;
	while (i < data->coder_num)
	{
		if (data->coders[i].compile_cont < data->compiles_required)
		{
			all_done = 0;
			if ((uint64_t)get_time() > data->coders[i].time_to_burnout)
			{
				print_status(&data->coders[i], "burned out");
				return (1);
			}
		}
		i++;
	}
	return (all_done);
}

static void	join_all(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->coder_num)
	{
		pthread_join(data->coders[i].thread_id, NULL);
		i++;
	}
}

static void	*monitor_routine(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	while (!data->finished)
	{
		if (scan_coders(data))
		{
			data->finished = 1;
			wake_all_dongles(data);
		}
		usleep(1000);
	}
	return (NULL);
}

int	thread_init(t_data *data)
{
	pthread_t	monitor_tid;

	if (create_all(data))
		return (1);
	if (pthread_create(&monitor_tid, NULL, monitor_routine, data) != 0)
		return (1);
	pthread_join(monitor_tid, NULL);
	join_all(data);
	return (0);
}
