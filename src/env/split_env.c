#include "../../utils/minishell.h"

char	*find_and_split(char *string)
{
	int		i;
	int		x;
	char	*str2;

	i = 0;
	x = 0;
	while (string[i])
	{
		if (string[i] == string[0])
			x++;
		i++;
	}
	str2 = malloc(sizeof(char) * (i - x + 1));
	i = 0;
	x = 0;
	while (string[i])
	{
		if (string[i] != string[0])
		{
			str2[x] = string[i];
			x++;
		}
		i++;
	}
	str2[x] = '\0';
	return (str2);
}

char	**split_env(char *string)
{
	char	**splited;
	char	**new_append;
	int		len;

	splited = malloc(sizeof(char *) * 3);
	new_append = ft_split(string, '=');
	if (!new_append[0])
		splited[0] = ft_strdup("");
	else
		splited[0] = ft_strdup(new_append[0]);
	if (!new_append[1])
		splited[1] = ft_strdup("");
	else
	{
		len = ft_strlen(new_append[1]);
		if (new_append[1][0] == '\'' || new_append[1][0] == '\"')
			splited[1] = find_and_split(new_append[1]);
		else
		{
			splited[1] = malloc(sizeof(char) * (len + 1));
			ft_strlcpy(splited[1], new_append[1], len + 1);
		}
	}
	splited[2] = NULL;
	free_array(new_append);
	return (splited);
}
