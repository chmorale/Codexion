/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmorale <chmorale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/28 09:30:36 by chmorale          #+#    #+#             */
/*   Updated: 2026/06/28 09:31:43 by chmorale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codex.h"

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
	if (init(&data, argv))
		return (1);
	if (valid_data(&data, argv, argc))
		return (1);
	if (thread_init(&data))
		return (1);
	clean_and_exit(&data);
	return (0);
}
