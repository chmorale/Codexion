/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_queue.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmorale <chmorale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/19 00:00:00 by chmorale          #+#    #+#             */
/*   Updated: 2026/07/19 00:00:00 by chmorale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codex.h"

static int	should_go_first(t_coder *a, t_coder *b, t_data *data)
{
	if (strcmp(data->scheduler, "edf") == 0)
		return (a->time_to_burnout < b->time_to_burnout);
	return (a->arrival_ticket < b->arrival_ticket);
}

int	is_my_turn(t_dongle *dongle, t_coder *coder, t_data *data)
{
	int	i;

	if (dongle->locked)
		return (0);
	if ((u_int64_t)get_time() - dongle->released_at < data->dongle_cooldown)
		return (0);
	i = 0;
	while (i < dongle->waiting_count)
	{
		if (dongle->waiters[i] != coder
			&& should_go_first(dongle->waiters[i], coder, data))
			return (0);
		i++;
	}
	return (1);
}

void	enqueue(t_dongle *dongle, t_coder *coder)
{
	dongle->waiters[dongle->waiting_count] = coder;
	dongle->waiting_count += 1;
}

void	dequeue(t_dongle *dongle, t_coder *coder)
{
	int	i;

	i = 0;
	while (dongle->waiters[i] != coder)
		i++;
	while (i < dongle->waiting_count - 1)
	{
		dongle->waiters[i] = dongle->waiters[i + 1];
		i++;
	}
	dongle->waiting_count -= 1;
}

void	wake_all_dongles(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->coder_num)
	{
		pthread_mutex_lock(&data->dongles[i].mutex);
		pthread_cond_broadcast(&data->dongles[i].cond);
		pthread_mutex_unlock(&data->dongles[i].mutex);
		i++;
	}
}
