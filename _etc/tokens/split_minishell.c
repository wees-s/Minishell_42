
#include "../utils/minishell.h"

char	**add_str_to_array(char **final_array, char *str)
{
	char	**new_array;
	int		count;
    int     i;

	count = 0;
	if (final_array)
	{
		while (final_array[count] != NULL)
			count++;
	}
	new_array = (char **)malloc(sizeof(char *) * (count + 2));
	if (!new_array)
		return (NULL); 
    i = 0;
	while (i < count)
	{
		new_array[i] = final_array[i];
        i++;
	}
	new_array[count] = str;
	new_array[count + 1] = NULL;
	if (final_array)
		free(final_array);
	return (new_array);
}

char	**split_minishell(char *line)
{
	int		i;
	char	*str;
	char    *current_str;
	char	**final_array;
	
	i = 0;
	current_str = NULL;
    final_array = NULL;
	while (line[i])
	{
		while (line[i] == ' ')
            i++;
		if (line[i] == '"' || line[i] == '\'')
			str = new_str_aspas(line, &i, line[i]);
		else if (line[i] == '<' || line[i] == '>' || line[i] == '|')
			str = new_str_red(line, &i);
		else
			str = new_str(line, &i);
		if (!str)
			return (NULL);
		final_array = add_str_to_array(final_array, str);
		free(str);
    }
    return (final_array);
}
