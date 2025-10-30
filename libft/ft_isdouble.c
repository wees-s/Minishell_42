/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isdouble.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wedos-sa <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 10:07:55 by wedos-sa          #+#    #+#             */
/*   Updated: 2025/10/16 10:21:48 by wedos-sa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_valid_number(const char *str)
{
	int	i;
	int	has_dot;
	int	has_digit;

	i = 0;
	has_dot = 0;
	has_digit = 0;
	while (str[i])
	{
		if (ft_isdigit(str[i]))
			has_digit = 1;
		else if (str[i] == '.')
		{
			if (has_dot)
				return (0);
			has_dot = 1;
			if (!ft_isdigit(str[i + 1]))
				return (0);
		}
		else
			return (0);
		i++;
	}
	return (has_digit);
}

int	ft_isdouble(const char *str)
{
	if (!str || !*str)
		return (0);
	if (*str == '-' || *str == '+')
		str++;
	if (!*str)
		return (0);
	return (is_valid_number(str));
}
