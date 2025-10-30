#include "../../utils/minishell.h"

int	have_equal(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '=')
			return (1);
		i++;
	}
	return (0);
}

int	valid_arg_value(char *str)
{
	int		i;
	int		count;
	char	x;
	int		len;

	i = 0;
	count = 0;
	len = ((int)ft_strlen(str) - 1);
	while (str[i] != '=')
		i++;
	i++;
	x = str[i];
	if (str[i] == '\"' || str[i] == '\''
		|| str[len] == '\"' || str[len] == '\'')
	{
		if (str[i] != str[len])
			return (0);
		while (str[i])
		{
			if (str[i] == x)
				count++;
			i++;
		}
		if (count % 2 != 0)
			return (0);
	}
	return (1);
}

int	valid_arg_name(char *str)
{
	int	i;

	i = 0;
	if (!have_equal(str))
		return (0);
	else
	{
		if (!ft_isalpha(str[0]))
			return (0);
		while (str[i] != '=')
		{
			if (ft_isalnum(str[i]))
				i++;
			else
				return (0);
		}
	}
	return (1);
}

int	valid_arg(char **split_line)
{
	int	i;

	i = 1;
	while (split_line[i])
	{
		if (valid_arg_name(split_line[i]) && valid_arg_value(split_line[i]))
			i++;
		else
			return (0);
	}
	return (1);
}

void	put_export(char *line, t_env *new_env)
{
	int		i;
	char	**split_line;

	i = 1;
	split_line = split_with_quotes(line);
	(void)new_env;
	if (valid_arg(split_line))
	{
		while (split_line[i])
		{
			check_to_put(split_line[i], &new_env);
			i++;
		}
		free_array(split_line);
	}
	else
	{
		ft_putstr_fd("ARG invalid\n", 2);
		free_array(split_line);
	}
}
