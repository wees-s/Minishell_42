/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bedantas <bedantas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 16:33:31 by bedantas          #+#    #+#             */
/*   Updated: 2025/11/26 16:33:32 by bedantas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../utils/minishell.h"

void	print_env(t_env *ptr)
{
	while (ptr != NULL)
	{
		if (ptr->value == NULL)
			;
		else
			printf("%s=%s\n", ptr->key, ptr->value);
		ptr = ptr->next;
	}
}

char	**split_env(char *line)
{
	char	**split_line;
	int		equal;

	split_line = malloc(sizeof(char *) * 3);
	equal = ft_chars_until(line, '=');
	if (equal == -1)
	{
		split_line[0] = ft_strdup(line);
		split_line[1] = NULL;
	}
	else if (line[equal + 1] == '\0')
	{
		split_line[0] = ft_substr(line, 0, equal);
		split_line[1] = ft_strdup("");
	}
	else
	{
		split_line[0] = ft_substr(line, 0, equal);
		split_line[1] = ft_strdup(line + equal + 1);
	}
	split_line[2] = NULL;
	return (split_line);
}

static t_env	*create_env(char *line)
{
	t_env	*node;
	char	**split;

	split = split_env(line);
	if (!split)
		return (NULL);
	node = malloc(sizeof(t_env));
	if (!node)
		return (NULL);
	node->key = split[0];
	node->value = split[1];
	node->next = NULL;
	free(split);
	return (node);
}

void	put_env(t_env **list, char *line)
{
	t_env	*new;
	t_env	*tmp;

	if (!list)
		return ;
	new = create_env(line);
	if (!new)
		return ;
	if (!*list)
	{
		*list = new;
		return ;
	}
	tmp = *list;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

t_env	*clone_env(char **envp)
{
	t_env	*list;
	int		i;

	list = NULL;
	i = 0;
	while (envp && envp[i])
	{
		put_env(&list, envp[i]);
		i++;
	}
	return (list);
}
