/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bedantas <bedantas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 16:32:04 by bedantas          #+#    #+#             */
/*   Updated: 2025/11/26 16:32:05 by bedantas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../utils/minishell.h"
#include "../redirect.h"

static void	free_all(char *str1, char *str2, char *str3)
{
	free(str1);
	free(str2);
	free(str3);
	str3 = NULL;
}

static void	expand_and_free(t_here	*h, t_env *begin_list)
{
	int		len;
	int		flag_quotes;
	char	*result_final;

	len = ft_strlen(h->eof);
	flag_quotes = 0;
	if (len >= 2 && ((h->eof[0] == '\'' && h->eof[len - 1] == '\'')
			|| (h->eof[0] == '\"' && h->eof[len - 1] == '\"')))
		flag_quotes = 1;
	if (flag_quotes == 0)
	{
		result_final = expand_arg(begin_list, h->result, 0);
		free(h->result);
		h->result = ft_strdup(result_final);
		free(result_final);
	}
	else
		(void)result_final;
	free(h->str);
	free_array(h->to_free);
	free(h->eof);
}

static t_here	init_heredoc(t_shell *sh, int i)
{
	t_here	here;

	here.to_free = ft_split(strstr(sh->s_pipe[i], "<<"), ' ');
	free_array(sh->s_pipe);
	here.result = ft_strdup("");
	here.eof = NULL;
	if (ft_strlen(here.to_free[0]) > 2)
		here.eof = ft_strdup(here.to_free[0] + 2);
	else
		here.eof = ft_strdup(here.to_free[1]);
	here.temp1 = remove_quotes_str(here.eof, 0, 0);
	here.str = NULL;
	return (here);
}

char	*heredoc(t_shell *sh, int i, int *fd_malloc)
{
	t_here	h;

	h = init_heredoc(sh, i);
	while (1)
	{
		h.str = readline("heredoc > ");
		if (h.str == NULL)
		{
			printf("Warning: Expecting delimiter (required '%s')\n", h.temp1);
			break ;
		}
		if (ft_strcmp(h.str, h.temp1) == 0)
			break ;
		h.tmp1 = ft_strjoin(h.result, h.str);
		h.tmp2 = ft_strjoin(h.tmp1, "\n");
		free(h.result);
		h.result = ft_strdup(h.tmp2);
		free_all(h.tmp1, h.tmp2, h.str);
	}
	dup2_close_in_out(sh->fd_in, sh->fd_out);
	free(fd_malloc);
	free(h.temp1);
	expand_and_free(&h, sh->env);
	free_list(&sh->env);
	return (h.result);
}
