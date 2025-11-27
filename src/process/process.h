/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bedantas <bedantas@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/26 16:32:19 by bedantas          #+#    #+#             */
/*   Updated: 2025/11/26 16:32:20 by bedantas         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROCESS_H
# define PROCESS_H

typedef struct s_pipes
{
	int		prev_fd;
	int		fd[2];
	pid_t	pid;
	char	*cmd;
	char	**tokens_cmd;
	pid_t	last_pid;
}	t_pipes;

#endif
