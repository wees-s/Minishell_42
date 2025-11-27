/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bedantas <bedantas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 16:32:46 by bedantas          #+#    #+#             */
/*   Updated: 2025/11/26 16:32:47 by bedantas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../utils/minishell.h"

static int	count_quotes(char *line)
{
	int	i;
	int	quote1;
	int	quote2;
	int	count;

	i = 0;
	quote1 = 0;
	quote2 = 0;
	count = 0;
	while (line[i])
	{
		if (line[i] == '\'' && !quote1)
		{
			quote2 = !quote2;
			count++;
		}
		else if (line[i] == '\"' && !quote2)
		{
			quote1 = !quote1;
			count++;
		}
		i++;
	}
	return (i - count);
}

char	*remove_quotes_str(char *line, int quote1, int quote2)
{
	int		i;
	int		j;
	char	*str;

	i = 0;
	j = 0;
	str = malloc(sizeof(char) * (count_quotes(line) + 1));
	while (line[i])
	{
		if (line[i] == '\'' && !quote1)
			quote2 = !quote2;
		else if (line[i] == '\"' && !quote2)
			quote1 = !quote1;
		if ((line[i] == '\'' && !quote1) || (line[i] == '\"' && !quote2))
			;
		else
		{
			str[j] = line[i];
			j++;
		}
		i++;
	}
	str[j] = '\0';
	return (str);
}

char	**tokens(char *line)
{
	char	**split_tokens;
	char	*temp;

	split_tokens = split_with_quotes(line);
	temp = remove_quotes_str(split_tokens[0], 0, 0);
	if (ft_countchar(split_tokens[0], ' ') != 0 || temp == NULL)
	{
		printf("Command not found\n");
		if (temp)
			free(temp);
		free_array(split_tokens);
		return (NULL);
	}
	free(split_tokens[0]);
	split_tokens[0] = temp;
	return (split_tokens);
}
