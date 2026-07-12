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
		if (data->coders[i].status == 1
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

static void	take_dongle(pthread_mutex_t *dongle)
{
	pthread_mutex_lock(dongle);
}

static void	drop_dongle(pthread_mutex_t *dongle)
{
	pthread_mutex_unlock(dongle);
}

static void	print_status(t_coder *coder, char *text)
{
	pthread_mutex_lock(&coder->data->write);
	printf("%lld %d %s\n", get_time(), coder->id, text);
	pthread_mutex_unlock(&coder->data->write);
}

void	take_dongles_and_compile(t_coder *coder)
{
	take_dongle(coder->l_dongle);
	print_status(coder, "has taken a dongle");
	take_dongle(coder->r_dongle);
	print_status(coder, "has taken a dongle");
	coder->status = 0;
	print_status(coder, "is compiling");
	//compile(coder);
	drop_dongle(coder->r_dongle);
	print_status(coder, "has dropped a dongle");
	drop_dongle(coder->l_dongle);
	print_status(coder, "has dropped a dongle");
}
