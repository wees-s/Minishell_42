/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strndup.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bedantas <bedantas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/15 15:50:39 by wedos-sa          #+#    #+#             */
/*   Updated: 2025/10/28 10:55:09 by bedantas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strndup(const char *s1, size_t n)
{
	char	*s2;

	s2 = malloc((n + 1) * sizeof(char));
	if (!s2)
		return (NULL);
	ft_strlcpy(s2, s1, n);
	return (s2);
}
