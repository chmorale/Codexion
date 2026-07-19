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

static int	take_dongle(t_coder *coder, pthread_mutex_t *dongle, int idx)
{
	while (1)
	{
		if (coder->data->finished)
			return (1);
		if (pthread_mutex_trylock(dongle) == 0)
		{
			if ((u_int64_t)get_time() - coder->data->dongle_released_at[idx]
				>= coder->data->dongle_cooldown)
				return (0);
			pthread_mutex_unlock(dongle);
		}
		usleep(200);
	}
}

void	drop_dongle(pthread_mutex_t *dongle, int idx, t_data *data)
{
	data->dongle_released_at[idx] = get_time();
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

int	take_dongles_and_compile(t_data *data, t_coder *coder)
{
	t_dongle_pair	pair;

	select_dongles(coder, &pair);
	if (take_dongle(coder, pair.first, pair.first_idx))
		return (0);
	print_status(coder, "has taken a dongle");
	if (take_dongle(coder, pair.second, pair.second_idx))
	{
		drop_dongle(pair.first, pair.first_idx, data);
		return (0);
	}
	print_status(coder, "has taken a dongle");
	do_compile(data, coder, &pair);
	return (1);
}
