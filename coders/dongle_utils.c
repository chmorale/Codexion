/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmorale <chmorale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 11:59:12 by chmorale          #+#    #+#             */
/*   Updated: 2026/07/18 11:59:28 by chmorale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codex.h"

void	select_dongles(t_coder *coder, t_dongle_pair *pair)
{
	if (coder->id % 2 == 0)
	{
		pair->first = coder->r_dongle;
		pair->second = coder->l_dongle;
		pair->first_idx = coder->r_idx;
		pair->second_idx = coder->l_idx;
	}
	else
	{
		pair->first = coder->l_dongle;
		pair->second = coder->r_dongle;
		pair->first_idx = coder->l_idx;
		pair->second_idx = coder->r_idx;
	}
}

void	do_compile(t_data *data, t_coder *coder, t_dongle_pair *pair)
{
	coder->status = 1;
	compile(data, coder);
	coder->compile_cont += 1;
	coder->time_to_burnout = get_time() + data->burnout_time;
	drop_dongle(pair->first, pair->first_idx, data);
	drop_dongle(pair->second, pair->second_idx, data);
	if (strcmp(data->scheduler, "edf") == 0
		&& coder->compile_cont < data->compiles_required)
	{
		coder->priority = coder->time_to_burnout;
		pthread_mutex_lock(&data->lock);
		heap_push(data->heap, coder);
		pthread_mutex_unlock(&data->lock);
	}
	coder->status = 2;
}
