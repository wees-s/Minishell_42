/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmpc                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bedantas <bedantas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 15:50:39 by wedos-sa          #+#    #+#             */
/*   Updated: 2025/10/24 18:39:03 by bedantas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strcmp(const char *string1, const char *string2)
{
	size_t	i;

	i = 0;
	while (string1[i] || string2[i])
	{
		if ((unsigned char)string1[i] != (unsigned char)string2[i])
			return ((unsigned char)string1[i] - (unsigned char)string2[i]);
		i++;
	}
	return (0);
}
