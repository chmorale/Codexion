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

static void	check_dongle_access(t_data *data, t_coder *coder)
{
	if (strcmp(data->scheduler, "edf") == 0)
	{
		coder->status = 0;
		if (is_most_urgent(data, coder))
			take_dongles_and_compile(coder);
		else
			take_dongles_and_compile(coder);
	}
}

static void	*routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	while (1)
	{
		check_dongle_access(coder->data, coder);
		cooldown(coder->data, coder);
		debug(coder->data, coder);
		refactor(coder->data, coder);
	}
	return (NULL);
}

int	thread_init(t_data *data)
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
	i = 0;
	while (i < data->coder_num)
	{
		pthread_join(data->coders[i].thread_id, NULL);
		i++;
	}
	return (0);
}
