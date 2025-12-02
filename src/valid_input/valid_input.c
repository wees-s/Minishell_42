/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   valid_input_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bedantas <bedantas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 16:31:12 by bedantas          #+#    #+#             */
/*   Updated: 2025/11/26 16:31:13 by bedantas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../utils/minishell.h"

static int	valid_quotes(char *line)
{
	int	i;
	int	quote1;
	int	quote2;

	i = 0;
	quote1 = 0;
	quote2 = 0;
	while (line[i])
	{
		if (line[i] == '\'' && !quote1)
			quote2 = !quote2;
		else if (line[i] == '\"' && !quote2)
			quote1 = !quote1;
		i++;
	}
	if (quote1 || quote2)
		return (0);
	return (1);
}

static int	valid_space(char *line)
{
	int	i;

	i = 0;
	while (line[i] == ' ')
		i++;
	if (line[i] == '\0')
		return (1);
	return (0);
}

int	valid_input(char *line, t_shell *sh)
{
	t_valid	v;

	v.quote1 = 0;
	v.quote2 = 0;
	v.last_pipe = 0;
	v.found_char = 0;
	if (valid_space(line) == 1)
		return (0);
	else if (!valid_pipe(line, &v, 0)
		|| !valid_red(line, '>') || !valid_red(line, '<'))
	{
		printf("Syntax error\n");
		sh->last_exit_status = 2;
		return (0);
	}
	else if (!valid_quotes(line))
	{
		printf("unexpected EOF while looking for matching `\"\'\n");
		sh->last_exit_status = 2;
		return (0);
	}
	return (1);
}
