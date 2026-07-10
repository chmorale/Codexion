/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmorale <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/28 09:30:36 by chmorale          #+#    #+#             */
/*   Updated: 2026/06/28 09:31:43 by chmorale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codex.h"
#include <string.h>

int thread_init(t_data *data)
{
    return(0);
}

int valid_data(t_data *data, char *argv[], int argc)	
{
	int i;
	i = 1;
	while (i < argc)
	{
		if (ft_atoi(argv[i]) <= 1 && i == 1)
		{
			printf("Error: Number of coders must be more than 0.\n");
			return (1);
		}
		data->coder_num = ft_atoi(argv[1]);
		if (ft_atoi(argv[i]) <= 0 && i == 2)
		{
			printf("Error: Time to burnout must be more than 0.\n");
			return (1);
		}
		data->burnout_time = ft_atoi(argv[2]);
		if (ft_atoi(argv[i]) <= 0 && i == 3)
		{
			printf("Error: Time_to_compile must be more than 0.\n");
			return (1);
		}
		data->compile_time = ft_atoi(argv[3]);
		if (ft_atoi(argv[i]) <= 0 && i == 4)
		{
			printf("Error: Time_to_debug must be more than 0.\n");
			return (1);
		}
		data->debug_time = ft_atoi(argv[4]);
		if (ft_atoi(argv[i]) <= 0 && i == 5)
		{
			printf("Error: Time_to_refactor must be more than 0.\n");
			return (1);
		}
		data->refactor_time = ft_atoi(argv[5]);
		if (ft_atoi(argv[i]) <= 0 && i == 6)
		{
			printf("Error: number_of_compiles_required must be more than 0.\n");
			return (1);
		}
		data->compiles_required = ft_atoi(argv[6]);
		if (ft_atoi(argv[i]) <= 0 && i == 7)
		{
			printf("Error: dongle_cooldown must be more than 0.\n");
			return (1);
		}
		data->dongle_cooldown = ft_atoi(argv[7]);
		if (!(strcmp(argv[i],"fifo") && strcmp(argv[i],"edf") && i == 8))
		{
			printf("Error: scheduler must be a  valid value.\n");
			return (1);
		}
		data->scheduler = argv[8];
		i++;
	}
	return(0);
}


int	main(int argc, char **argv)
{
	t_data	data;
	
	if (argc != 9)
	{
		printf("Error: Number incorrect of arguments.\n");
		return (1);
	}
	if (input_checker(argv))
		return (1);
	else if (strcmp(argv[8], "fifo") == 0 || strcmp(argv[8], "edf") == 0)
	    data.scheduler = argv[8];
	else
	    return (1);
	if (init(&data, argv, argc))
		return (1);
	if (valid_data(&data, argv, argc))
	    return (1);
	if (data.coder_num == 1)
		return (case_one(&data));
	if (thread_init(&data))
		return (1);
	ft_exit(&data);
	return (0);
}
	
	
	
	

