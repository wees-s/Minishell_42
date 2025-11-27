/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bedantas <bedantas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/06 14:02:38 by wedos-sa          #+#    #+#             */
/*   Updated: 2025/11/12 14:17:57 by bedantas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strstr(char **text, char *word)
{
	int	x;
	int	i;

	x = 0;
	while (text[x])
	{
		i = 0;
		if (text[x][i] == word[i])
		{
			while (word[i] != '\0' && text[x][i]
					!= '\0' && text[x][i] == word [i])
				i++;
			if (word[i] == '\0' && text[x][i - 1] == word[i - 1])
				return (&text[x][i + 1]);
		}
		x++;
	}
	return (0);
}
