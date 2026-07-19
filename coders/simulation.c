/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmorale <chmorale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/28 09:30:36 by chmorale          #+#    #+#             */
/*   Updated: 2026/07/11 16:26:01 by chmorale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codex.h"

static int	try_claim_if_urgent(t_data *data, t_coder *coder)
{
	int		urgent;

	pthread_mutex_lock(&data->lock);
	urgent = 0;
	if (data->heap->size > 0 && data->heap->array[0] == coder)
	{
		heap_pop(data->heap);
		coder->status = 1;
		urgent = 1;
	}
	pthread_mutex_unlock(&data->lock);
	return (urgent);
}

static int	check_dongle_access(t_data *data, t_coder *coder)
{
	if (coder->compile_cont >= data->compiles_required)
		return (0);
	if (strcmp(data->scheduler, "edf") == 0)
	{
		if (try_claim_if_urgent(data, coder))
			return (take_dongles_and_compile(data, coder));
		usleep(100);
		return (0);
	}
	return (take_dongles_and_compile(data, coder));
}

void	*routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	while (!coder->data->finished)
	{
		if (!check_dongle_access(coder->data, coder))
		{
			usleep(1000);
			continue ;
		}
		if (coder->data->finished)
			break ;
		coder->status = 3;
		debug(coder->data, coder);
		coder->status = 4;
		refactor(coder->data, coder);
		coder->status = 0;
	}
	return (NULL);
}
