/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmorale <chmorale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 18:24:04 by chmorale          #+#    #+#             */
/*   Updated: 2026/07/12 18:24:44 by chmorale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codex.h"

void	compile(t_data *data, t_coder *coder)
{
	if (data && coder)
	{
		print_status(coder, "coder is compiling");
		usleep(coder->data->compile_time * 1000);
	}
}

void	cooldown(t_data *data, t_coder *coder)
{
	if (data && coder)
	{
		usleep(coder->data->dongle_cooldown * 1000);
	}
}

void	debug(t_data *data, t_coder *coder)
{
	if (data && coder)
	{
		print_status(coder, "coder is debuging");
		usleep(coder->data->debug_time * 1000);
	}
}

void	refactor(t_data *data, t_coder *coder)
{
	if (data && coder)
	{
		print_status(coder, "coder is refactoring");
		usleep(coder->data->refactor_time * 1000);
	}
}
