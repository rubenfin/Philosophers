/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   ft_atoi.c                                          :+:    :+:            */
/*                                                     +:+                    */
/*   By: rfinneru <rfinneru@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2024/01/19 10:55:03 by rfinneru      #+#    #+#                 */
/*   Updated: 2024/01/19 10:55:18 by rfinneru      ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

static int	sign_count(char num)
{
	int	sign;

	sign = 1;
	if (num == '-')
	{
		sign = -1;
	}
	return (sign);
}

int	ft_atoi(const char *nptr)
{
	int		value;
	int		sign;
	char	*num;
	int		i;

	i = 0;
	value = 0;
	sign = 1;
	num = (char *)nptr;
	while (num[i] == ' ' || (num[i] >= 9 && num[i] <= 13))
	{
		i++;
	}
	if (num[i] == '-' || num[i] == '+')
	{
		sign = sign_count(num[i]);
		i++;
	}
	while (num[i] >= '0' && num[i] <= '9')
	{
		value = value * 10 + (num[i] - '0');
		i++;
	}
	return (value * sign);
}
