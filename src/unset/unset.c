#include "../../utils/minishell.h"

void	unset_env(t_env **env, char *cmd)
{
	t_env	*curr;
	t_env	*prev;
	char	*key;

	key = ft_strchr(cmd, ' ');
	if (!key || !*(++key))
	{
		printf("unset: not enough arguments\n");
		return ;
	}
	curr = *env;
	prev = NULL;
	while (curr)
	{
		if (ft_strncmp(curr->key, key, ft_strlen(key) + 1) == 0)
		{
			if (prev)
				prev->next = curr->next;
			else
				*env = curr->next;
			free(curr->key);
			free(curr->value);
			free(curr);
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
}
