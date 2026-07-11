/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmorale <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/28 09:30:36 by chmorale          #+#    #+#             */
/*   Updated: 2026/07/11 16:26:01 by chmorale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codex.h"


int simulation(t_data *data)
{
    return (thread_init(&data));
}

void check_dongle_access(t_data *data, t_code *coder)
{
    if (data->scheduler == 'edf')
        //check_time_to_burnout()
        int i;
        t_code *seleted;
        
        i = 0;
        seleccted = NULL;
        
        while (i < data->coder_num)
        {
            if (data-coders[i].waiting == 1)
            {
                if (selected == NULL ||
                data->coders[i].time_to_burnout < selected->time_to_burnout)
                {
                    selected = &data->coders[i]
                }
            }
            i++;
        }
        return (selected);
    coger_primer_dongle(coder);
    coger_segundo_dongle(coder);

    compile(coder);

    soltar_primer_dongle(coder);
    soltar_segundo_dongle(coder);
}

void *routine(void *arg)
{
    t_coder *coder;

    coder = (t_coder *)arg;
    while (1)
    {
        check_dongle_access(coder->data, coder);
        // compilar, dormir, actualizar time_to_burnout, etc.
    }
    return (NULL);
}

int thread_init(t_data *data)
{
    int i;
    
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

