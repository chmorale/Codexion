/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmorale <chmorale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/18 11:53:13 by chmorale          #+#    #+#             */
/*   Updated: 2026/07/18 11:53:46 by chmorale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codex.h"

void	free_coders(t_data *data)
{
	int	i;

	if (!data->coders)
		return ;
	i = 0;
	while (i < data->coder_num)
	{
		pthread_mutex_destroy(&data->coders[i].lock);
		i++;
	}
	free(data->coders);
	data->coders = NULL;
}

void	free_dongles(t_data *data)
{
	int	i;

	if (!data->dongles)
		return ;
	i = 0;
	while (i < data->coder_num)
	{
		pthread_mutex_destroy(&data->dongles[i]);
		i++;
	}
	free(data->dongles);
	data->dongles = NULL;
}

void	free_misc(t_data *data)
{
	if (data->heap)
	{
		free(data->heap->array);
		free(data->heap);
		data->heap = NULL;
	}
	if (data->dongle_released_at)
	{
		free(data->dongle_released_at);
		data->dongle_released_at = NULL;
	}
}
