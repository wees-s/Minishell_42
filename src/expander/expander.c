#include "../../utils/minishell.h"
/*
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
}*/

#include "../../utils/minishell.h"

static char	*get_key(char *str)
{
	int		i;
	char	*key;

	i = 0;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	key = malloc(sizeof(char) * (i + 1));
	if (!key)
		return (NULL);
	ft_strlcpy(key, str, i + 1);
	return (key);
}

static void	join_and_free(char **result, char *add)
{
	char	*tmp;

	tmp = *result;
	*result = ft_strjoin(*result, add);
	free(tmp);
}

char	*expand_arg(t_env *begin_list, char *str)
{
	int		i;
	int		in_single;
	int		in_double;
	char	*result;

	i = 0;
	in_single = 0;
	in_double = 0;
	result = ft_strdup("");
	while (str[i])
	{
		printf("VEZES QUE EU ENTREI NO LAÇO = %d\n", i);
		if (str[i] == '\'' && !in_double)
		{
			in_single = !in_single;	
			printf("VALOR DO IN_SINGLE = %d\n", in_double);
		}
		else if (str[i] == '"' && !in_single)
		{
			in_double = !in_double;
			printf("VALOR DO IN_DOUBLE = %d\n", in_double);
		}
		else if (str[i] == '$' && !in_single && str[i + 1])
		{
			char *key = get_key(&str[i + 1]);
			char *value = expanded(begin_list, key);
			if (!value)
				value = "";
			join_and_free(&result, value);
			i += ft_strlen(key);
			free(key);
		}
		else
		{
			char *literal = ft_substr(str, i, 1);
			join_and_free(&result, literal);
			free(literal);
		}
		i++;
	}
	return (result);
}

char	*expanded(t_env *begin_list, char *key)
{
	while (begin_list)
	{
		if (ft_strncmp(key, begin_list->key, ft_strlen(begin_list->key) + 1) == 0)
			return (begin_list->value);
		begin_list = begin_list->next;
	}
	return (NULL);
}
