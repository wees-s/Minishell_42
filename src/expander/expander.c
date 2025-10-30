#include "../../utils/minishell.h"

char	*expand_arg(t_env *begin_list, char *string)
{
	int		i;
	int		j;
	char	*result;
	char	*temp;

	temp = NULL;
	result = NULL;
	j = 1;
	i = 0;
	while (string[i] != '$' && string[i] != '\0')
		i++;
	if (string[i] != '$')
		return (NULL);
	if (i == 0)
	{
		while (string[j] != '\0')
			j++;
		temp = malloc(sizeof(char) * 1); //malloc
		temp[0] = '\0';
		result = ft_strjoin(temp, expanded(begin_list, string + 1)); //malloc
	}
	if (i > 0)
	{
		temp = malloc(sizeof(char) * i + 1); //malloc
		ft_strlcpy(temp, string, i + 1);
		result = ft_strjoin(temp, expanded(begin_list, string + i + 1)); //malloc
	}
	if (temp != NULL)
		free(temp);
	return (result);
}

char	*expanded(t_env *begin_list, char *key)
{
	while (begin_list)
	{
		if (ft_strncmp(key, begin_list->key, ft_strlen(key) + 1) == 0)
			return (begin_list->value);
		begin_list = begin_list->next;
	}
	return (NULL);
}
