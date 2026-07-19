/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmorale <chmorale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/17 09:39:26 by chmorale          #+#    #+#             */
/*   Updated: 2025/12/17 09:44:44 by chmorale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codex.h"

int	ft_atoi(const char *nptr)
{
	int		i;
	int		signo;
	long	total;

	total = 0;
	i = 0;
	signo = 1;
	while ((nptr[i] > 8 && nptr[i] < 14) || (nptr[i] == 32))
		i++;
	if (nptr[i] == 45)
		signo = -1;
	if ((nptr[i] == 43) || (nptr[i] == 45))
		i++;
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		total = total * 10 + (nptr[i] - '0');
		i++;
	}
	total = (int)(total * signo);
	return (total);
}
