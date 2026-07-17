/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_access.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmorale <chmorale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 14:39:34 by chmorale          #+#    #+#             */
/*   Updated: 2026/07/12 14:40:13 by chmorale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codex.h"

int	is_most_urgent(t_data *data, t_coder *coder)
{
	int		i;
	t_coder	*selected;

	i = 0;
	selected = NULL;
	while (i < data->coder_num)
	{
		if (data->coders[i].status == 0
			&& data->coders[i].compile_cont < data->compiles_required)
		{
			if (selected == NULL
				|| (data->coders[i].time_to_burnout
					< selected->time_to_burnout))
				selected = &data->coders[i];
		}
		i++;
	}
	return (selected == coder);
}

static int	take_dongle(t_coder *coder, pthread_mutex_t *dongle)
{
	while (pthread_mutex_trylock(dongle) != 0)
	{
		if (coder->data->finished)
			return (1);
		usleep(200);
	}
	return (0);
}

static void	drop_dongle(pthread_mutex_t *dongle)
{
	pthread_mutex_unlock(dongle);
}

void	print_status(t_coder *coder, char *text)
{
	int	time;

	pthread_mutex_lock(&coder->data->write);
	time = get_time() - coder->data->init_time;
	printf("%i %d %s\n", time, coder->id, text);
	pthread_mutex_unlock(&coder->data->write);
}

void	take_dongles_and_compile(t_data *data, t_coder *coder)
{
	pthread_mutex_t *first;
	pthread_mutex_t *second;
	
	if (coder->id % 2 == 0)
	{
		first = coder->r_dongle;
		second = coder->l_dongle;
	}
	else
	{
		first = coder->l_dongle;
		second = coder->r_dongle;
	}
	if (take_dongle(coder, first))
		return ;
	print_status(coder, "has taken a dongle");
	if (take_dongle(coder, second))
	{
		drop_dongle(first);
		return ;
	}
	print_status(coder, "has taken a dongle");
	coder->status = 1;
	compile(data, coder);
	coder->compile_cont += 1;
	coder->time_to_burnout = get_time() + data->burnout_time;
	drop_dongle(first);
	drop_dongle(second);
	coder->status = 2;
}
