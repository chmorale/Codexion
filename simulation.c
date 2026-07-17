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
	int	urgent;

	pthread_mutex_lock(&data->lock);
	urgent = is_most_urgent(data, coder);
	if (urgent)
		coder->status = 1;   // se "reserva" DENTRO del lock, antes de soltar
	pthread_mutex_unlock(&data->lock);
	return (urgent);
}

static void	check_dongle_access(t_data *data, t_coder *coder)
{
	if (coder->compile_cont >= data->compiles_required)
		return ;
	if (strcmp(data->scheduler, "edf") == 0)
	{
		//pthread_mutex_lock(&cola_mutex);
		while (try_claim_if_urgent(data, coder))
		{
			//pthread_mutex_unlock(&cola_mutex);
			take_dongles_and_compile(data, coder);
		}
		usleep(100);
		//pthread_mutex_lock(&cola_mutex);
	}
	else
		take_dongles_and_compile(data, coder);
}

static void	*routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	while (!coder->data->finished)
	{
		check_dongle_access(coder->data, coder);
		if (coder->data->finished)
			break ;
		if (coder->compile_cont >= coder->data->compiles_required)
		{
			usleep(1000);
			continue;
		}
		cooldown(coder->data, coder);
		coder->status = 3;
		debug(coder->data, coder);
		coder->status = 4;
		refactor(coder->data, coder);
		coder->status = 0;
	}
	return (NULL);
}

static int	all_finished(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->coder_num)
	{
		if (data->coders[i].compile_cont < data->compiles_required)
			return (0);
		i++;
	}
	return (1);
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
	// PASO 2: EL VIGILANTE (El main se queda aquí controlando el burnout)
	while (!data->finished)
	{
		i = 0;
		while (i < data->coder_num)
		{
			if (data->coders[i].compile_cont < data->compiles_required
				&& (uint64_t)get_time() > data->coders[i].time_to_burnout)
			{
				print_status(&data->coders[i], "has burnout");
				data->finished = 1; // Activamos el interruptor de apagado global
				break ;
			}
			i++;
		}
		if (all_finished(data))
			data->finished = 1;
		usleep(1000); // Descanso de 1ms para no saturar tu CPU
	}
	i = 0;
	while (i < data->coder_num)
	{
		pthread_join(data->coders[i].thread_id, NULL);
		i++;
	}
	return (0);
}
