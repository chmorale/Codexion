/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmorale <chmorale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/28 10:59:20 by chmorale          #+#    #+#             */
/*   Updated: 2026/07/19 00:00:00 by chmorale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codex.h"

long long	get_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL) == -1)
		return (0);
	return ((tv.tv_sec * 1000LL) + (tv.tv_usec / 1000LL));
}

u_int64_t	next_ticket(t_data *data)
{
	u_int64_t	ticket;

	pthread_mutex_lock(&data->ticket_lock);
	ticket = data->next_ticket;
	data->next_ticket += 1;
	pthread_mutex_unlock(&data->ticket_lock);
	return (ticket);
}

int	error(char *msg, t_data *data)
{
	printf("%s", msg);
	if (data)
	{
		free_all(data);
	}
	return (1);
}

void	free_all(t_data *data)
{
	if (!data)
		return ;
	free_coders(data);
	free_dongles(data);
	free_misc(data);
	pthread_mutex_destroy(&data->write);
	pthread_mutex_destroy(&data->lock);
	pthread_mutex_destroy(&data->ticket_lock);
}

void	clean_and_exit(t_data *data)
{
	if (data)
	{
		free_all(data);
		printf("FIN");
	}
}
