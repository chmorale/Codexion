/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmorale <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/28 10:59:20 by chmorale          #+#    #+#             */
/*   Updated: 2026/06/28 11:00:36 by chmorale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	input_checker(char **argv)
{
	int	i;
	int	j;

	i = 1;
	while (argv[i] && i < 8)
	{
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] == ' ')
			{
				j++;
				continue ;
			}
			if ((argv[i][j] < 48 || argv[i][j] > 57))
				return (error(ERR_IN_1, NULL));
			j++;
		}
		i++;
	}
	return (0);
}


