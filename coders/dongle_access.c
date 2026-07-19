/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_access.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmorale <chmorale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 14:39:34 by chmorale          #+#    #+#             */
/*   Updated: 2026/07/19 00:00:00 by chmorale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codex.h"

static void	build_deadline(struct timespec *ts)
{
	struct timeval	now;
	long			ms;

	gettimeofday(&now, NULL);
	ms = (now.tv_usec / 1000) + 1;
	ts->tv_sec = now.tv_sec + (ms / 1000);
	ts->tv_nsec = (ms % 1000) * 1000000L;
}

static int	take_dongle(t_coder *coder, t_dongle *dongle, t_data *data)
{
	struct timespec	ts;

	pthread_mutex_lock(&dongle->mutex);
	enqueue(dongle, coder);
	while (!is_my_turn(dongle, coder, data) && !data->finished)
	{
		build_deadline(&ts);
		pthread_cond_timedwait(&dongle->cond, &dongle->mutex, &ts);
	}
	dequeue(dongle, coder);
	if (data->finished)
	{
		pthread_cond_broadcast(&dongle->cond);
		pthread_mutex_unlock(&dongle->mutex);
		return (1);
	}
	dongle->locked = 1;
	pthread_mutex_unlock(&dongle->mutex);
	return (0);
}

void	drop_dongle(t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->locked = 0;
	dongle->released_at = get_time();
	pthread_cond_broadcast(&dongle->cond);
	pthread_mutex_unlock(&dongle->mutex);
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

	coder->arrival_ticket = next_ticket(data);
	select_dongles(coder, &pair);
	if (take_dongle(coder, pair.first, data))
		return (0);
	print_status(coder, "has taken a dongle");
	if (take_dongle(coder, pair.second, data))
	{
		drop_dongle(pair.first);
		return (0);
	}
	print_status(coder, "has taken a dongle");
	do_compile(data, coder, &pair);
	return (1);
}
