/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_join_all.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bedantas <bedantas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/24 17:38:03 by wedos-sa          #+#    #+#             */
/*   Updated: 2025/11/12 14:12:35 by bedantas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_join_all(char **string, unsigned int start)
{
	char	*result;
	char	*temp;

	temp = NULL;
	result = ft_strdup("");
	if (!result)
		return (NULL);
	while (string[start] != NULL)
	{
		temp = ft_strjoin(result, string[start]);
		free(result);
		result = temp;
		if (string[start + 1])
		{
			temp = ft_strjoin(result, " ");
			free(result);
			result = temp;
		}
		start++;
	}
	return (result);
}
