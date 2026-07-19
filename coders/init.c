/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmorale <chmorale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/28 12:02:13 by chmorale          #+#    #+#             */
/*   Updated: 2026/07/19 00:00:00 by chmorale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codex.h"

static int	alloc(t_data *data)
{
	data->dongles = malloc(sizeof(t_dongle) * data->coder_num);
	if (!data->dongles)
		return (error(ALLOC_ERR_1, data));
	data->coders = malloc(sizeof(t_coder) * data->coder_num);
	if (!data->coders)
		return (error(ALLOC_ERR_2, data));
	return (0);
}

static int	init_dongles(t_data *data)
{
	int	i;

	i = -1;
	while (++i < data->coder_num)
	{
		pthread_mutex_init(&data->dongles[i].mutex, NULL);
		pthread_cond_init(&data->dongles[i].cond, NULL);
		data->dongles[i].locked = 0;
		data->dongles[i].released_at = 0;
		data->dongles[i].waiting_count = 0;
	}
	data->coders[0].l_dongle = &data->dongles[0];
	data->coders[0].r_dongle = &data->dongles[data->coder_num - 1];
	data->coders[0].l_idx = 0;
	data->coders[0].r_idx = data->coder_num - 1;
	i = 1;
	while (i < data->coder_num)
	{
		data->coders[i].l_dongle = &data->dongles[i];
		data->coders[i].r_dongle = &data->dongles[i - 1];
		data->coders[i].l_idx = i;
		data->coders[i].r_idx = i - 1;
		i++;
	}
	return (0);
}

static void	init_coders(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->coder_num)
	{
		data->coders[i].data = data;
		data->coders[i].id = i + 1;
		data->coders[i].time_to_burnout = data->init_time + data->burnout_time;
		if (strcmp(data->scheduler, "fifo") == 0)
			data->coders[i].priority = data->coders[i].id;
		else
			data->coders[i].priority = data->coders[i].time_to_burnout;
		data->coders[i].compile_cont = 0;
		data->coders[i].status = 0;
		data->coders[i].arrival_ticket = 0;
		pthread_mutex_init(&data->coders[i].lock, NULL);
		heap_push(data->heap, &data->coders[i]);
		i++;
	}
}

static t_heap	*heap_create(int num_coders)
{
	t_heap	*heap;

	heap = malloc(sizeof(t_heap));
	if (!heap)
		return (NULL);
	heap->array = malloc(sizeof(t_coder *) * num_coders);
	if (!heap->array)
	{
		free(heap);
		return (NULL);
	}
	heap->size = 0;
	heap->capacity = num_coders;
	return (heap);
}

int	init(t_data *data)
{
	data->heap = heap_create(data->coder_num);
	if (!data->heap)
		return (1);
	data->init_time = get_time();
	pthread_mutex_init(&data->write, NULL);
	pthread_mutex_init(&data->lock, NULL);
	pthread_mutex_init(&data->ticket_lock, NULL);
	data->next_ticket = 0;
	if (alloc(data))
		return (1);
	if (init_dongles(data))
		return (1);
	init_coders(data);
	data->finished = 0;
	return (0);
}
