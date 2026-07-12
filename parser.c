/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmorale <chmorale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/07/12 14:37:48 by chmorale          #+#    #+#             */
/*   Updated: 2026/07/12 14:38:42 by chmorale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codex.h"

static void	asign_data(t_data *data, char *argv[])
{
	data->coder_num = ft_atoi(argv[1]);
	data->burnout_time = ft_atoi(argv[2]);
	data->compile_time = ft_atoi(argv[3]);
	data->debug_time = ft_atoi(argv[4]);
	data->refactor_time = ft_atoi(argv[5]);
	data->compiles_required = ft_atoi(argv[6]);
	data->dongle_cooldown = ft_atoi(argv[7]);
	data->scheduler = argv[8];
}

int	valid_data(t_data *data, char *argv[], int argc)
{
	int	i;

	i = 1;
	while (i < argc)
	{
		if (ft_atoi(argv[i]) <= 1 && i == 1)
			return (printf("Error: Number of coders less than 0.\n"), 1);
		if (ft_atoi(argv[i]) <= 0 && i == 2)
			return (printf("Error: Time to burnout less than 0.\n"), 1);
		if (ft_atoi(argv[i]) <= 0 && i == 3)
			return (printf("Error: Time_to_compile less than 0.\n"), 1);
		if (ft_atoi(argv[i]) <= 0 && i == 4)
			return (printf("Error: Time_to_debug less than 0.\n"), 1);
		if (ft_atoi(argv[i]) <= 0 && i == 5)
			return (printf("Error: Time_to_refactor less than 0.\n"), 1);
		if (ft_atoi(argv[i]) <= 0 && i == 6)
			return (printf("Error: compiles_required less than 0.\n"), 1);
		if (ft_atoi(argv[i]) <= 0 && i == 7)
			return (printf("Error: dongle_cooldown less than 0.\n"), 1);
		if ((strcmp(argv[i], "fifo") && strcmp(argv[i], "edf") && i == 8))
			return (printf("Error: scheduler must be a  valid value.\n"), 1);
		i++;
	}
	asign_data(data, argv);
	return (0);
}
