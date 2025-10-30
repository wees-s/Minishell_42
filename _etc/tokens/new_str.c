
#include "../utils/minishell.h"

char	*malloc_str(char *line, int start, int count)
{
	char	*str;
	int		x;

	x = 0;
	str = malloc((count + 1) * sizeof(char));
	if (!str)
		return (NULL);
	while (x < count)
	{
		str[x] = line[start + x];
		x++;
	}
	str[count] = '\0';
	return (str);
}

char	*new_str_aspas(char *line, int *i, char x)
{
	int		count;
	int		start_i;
	char	*new_str;

	start_i = *i;
	count = 0;
	(*i)++;
	while (line[*i] != x && line[*i] != '\0')
	{
		count++;
		(*i)++;
	}
	if (line[(*i) - 1] != x && line[*i] == '\0')
		return (NULL);
	(*i)++;
	new_str = malloc_str(line, start_i + 1, count);
	if (!new_str)
		return (NULL);
	return (new_str);
}

char	*new_str_red(char *line, int *i)
{
    int     count;
    char    *new_str;
    int     start_i;

	start_i = *i;
    count = 1;
    if (line[*i] == '<' && line[*i + 1] == '<')
        count = 2;
    else if (line[*i] == '>' && line[*i + 1] == '>')
        count = 2;
    new_str = malloc_str(line, start_i, count);
    *i += count;
    return (new_str);
}

char	*new_str(char *line, int *i)
{
	int		count;
	int     start_i;
	char	*new_str;

	start_i = *i;
	count = 0;
	while (line[*i] != '>' && line[*i] != '<' && line[*i] != '|'
			&& line[*i] != '"' && line[*i] != '\'' && line[*i] != '\0')
	{
		count++;
		(*i)++;
	}
	new_str = malloc_str(line, start_i, count);
	if (!new_str)
		return (0);
	return (new_str);
}
