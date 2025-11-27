/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bedantas <bedantas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 16:33:49 by bedantas          #+#    #+#             */
/*   Updated: 2025/11/26 16:33:50 by bedantas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../utils/minishell.h"

static void	swap_key_value(t_env *exp, t_env *cmp)
{
	char	*temp;

	temp = exp->key;
	exp->key = cmp->key;
	cmp->key = temp;
	temp = exp->value;
	exp->value = cmp->value;
	cmp->value = temp;
}

static void	sort_list(t_env **env)
{
	t_env	*cmp;
	t_env	*exp;

	if (!env)
		return ;
	exp = *env;
	while (exp)
	{
		cmp = exp->next;
		while (cmp)
		{
			if (ft_strcmp(exp->key, cmp->key) > 0)
				swap_key_value(exp, cmp);
			cmp = cmp->next;
		}
		exp = exp->next;
	}
}

static t_env	*copy_list(t_env *new_env)
{
	t_env	*exp;
	t_env	*cur;
	t_env	*node;
	t_env	*last;

	exp = NULL;
	cur = new_env;
	last = NULL;
	while (cur)
	{
		node = malloc(sizeof(t_env));
		node->key = ft_strdup(cur->key);
		if (cur->value == NULL)
			node->value = NULL;
		else
			node->value = ft_strdup(cur->value);
		node->next = NULL;
		if (!exp)
			exp = node;
		else
			last->next = node;
		last = node;
		cur = cur->next;
	}
	return (exp);
}

static void	creat_print_export(t_env *new_env)
{
	t_env	*exp;
	t_env	*tmp;

	exp = copy_list(new_env);
	if (!exp)
		return ;
	sort_list(&exp);
	tmp = exp;
	while (tmp)
	{
		if (tmp->value == NULL)
			printf("declare -x %s\n", tmp->key);
		else
			printf("declare -x %s=\"%s\"\n", tmp->key, tmp->value);
		tmp = tmp->next;
	}
	free_list(&exp);
}

void	export_arg(char **line_tokens, t_shell *sh)
{
	int	i;

	i = 1;
	if (!line_tokens[1])
	{
		sh->last_exit_status = 0;
		creat_print_export(sh->env);
	}
	else if (valid_arg(line_tokens, sh) == 1)
	{
		while (line_tokens[i])
		{
			check_to_put(line_tokens[i], &sh->env);
			i++;
		}
		sh->last_exit_status = 0;
	}
}
