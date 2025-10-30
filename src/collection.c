#include "../utils/minishell.h"

t_collect	*create_item(void *data)
{
	t_collect	*ptr;

	ptr = malloc(sizeof(t_collect));
	if (!ptr)
		return (NULL);
	if (ptr)
	{
		ptr->data = data;
		ptr->next = NULL;
	}
	return (ptr);
}

t_collect	*last_item(t_collect *begin_list)
{
	while (begin_list->next != NULL)
		begin_list = begin_list->next;
	return (begin_list);
}

void	*put_item(t_collect **begin_list, void *data)
{
	t_collect	*last;
	t_collect	*item;

	if (!begin_list)
		return (NULL);
	item = create_item(data);
	if (!item)
		return (NULL);
	if (!*begin_list)
	{
		*begin_list = item;
		return (NULL);
	}
	last = last_item(*begin_list);
	if (last)
		last->next = item;
	else
		*begin_list = item;
	return (data);
}

t_collect	*init_collect(void)
{
	t_collect	*ptr;

	ptr = malloc(sizeof(t_collect));
	if (!ptr)
		return (NULL);
	if (ptr)
	{
		ptr->data = NULL;
		ptr->next = NULL;
	}
	return (ptr);
}

void	close_collect(t_collect *init)
{
	t_collect	*temp;

	while (init)
	{
		temp = init->next;
		free(init);
		init = temp;
	}
}
