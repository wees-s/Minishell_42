/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bedantas <bedantas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 16:31:36 by bedantas          #+#    #+#             */
/*   Updated: 2025/11/26 16:31:37 by bedantas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REDIRECT_H
# define REDIRECT_H

typedef struct s_fd
{
	int	i;
	int	tmp;
	int	quote[2];
	int	*fd;
}	t_fd;

typedef struct s_here
{
	char	**to_free;
	char	*result;
	char	*eof;
	char	*temp1;
	char	*str;
	char	*tmp1;
	char	*tmp2;
}	t_here;

#endif
