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

#include <codexion.h>


int	main(int argc, char **argv)
{
	t_data	data;
	
	if (argc != 8)
	{
		printf("Error: Number incorrect of arguments.\n");
		return (1);
	}
	if (input_checker(argv))
		return (1);
	if (argv[1][8] != "fifo" and argv[1][8] != "edf")
	    return (1);
	else
	    data -> scheduler = argv[8];
	if (init(&data, argv, argc))
		return (1);
	if (data.philo_num == 1)
		return (case_one(&data));
	if (thread_init(&data))
		return (1);
	ft_exit(&data);
	return (0);
}
	
	
	
	
	
	number_of_coders = 0;
	time_to_burnout = 0;
	time_to_compile = 0;
	time_to_debug = 0;
	time_to_refactor = 0;
	number_of_compiles_required = 0;
	dongle_cooldown = 0;
	scheduler = "";
	i = 1;
	while (i < argc)
	{
		if (argv[i] <= 1 and i == 1)
		{
			printf("Error: Number of coders must be more than 1.\n");
			return (1);
		}
		number_of_coders = argv[1];
		if (argv[i] <= 0 and i == 2)
		{
			printf("Error: Time to burnout must be more than 0.\n");
			return (1);
		}
		time_to_burnout = argv[2];
		if (argv[i] <= 0 and i == 3)
		{
			printf("Error: Time_to_compile must be more than 0.\n");
			return (1);
		}
		time_to_compile = argv[3];
		if (argv[i] <= 0 and i == 4)
		{
			printf("Error: Time_to_debug must be more than 0.\n");
			return (1);
		}
		time_to_debug = argv[4];
		if (argv[i] <= 0 and i == 5)
		{
			printf("Error: Time_to_refactor must be more than 0.\n");
			return (1);
		}
		time_to_refactor = argv[5];
		if (argv[i] <= 0 and i == 6)
		{
			printf("Error: number_of_compiles_required must be more than 0.\n");
			return (1);
		}
		number_of_compiles_required = argv[6];
		if (argv[i] <= 0 and i == 7)
		{
			printf("Error: dongle_cooldown must be more than 0.\n");
			return (1);
		}
		dongle_cooldown = argv[7];
		if (argv[i] != "fifo" and argv[i] != "edf" and i == 8)
		{
			printf("Error: scheduler must be a  valid value.\n");
			return (1);
		}
		scheduler = argv[8];
		i++;
	}
}
