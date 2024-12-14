/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suroh <suroh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/03 01:01:35 by suroh             #+#    #+#             */
/*   Updated: 2024/12/03 16:19:35 by suroh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	error_127(const char *msg, t_pipex *pipex)
{
	if (pipex)
		close_fds_files(pipex);
	write(STDERR_FILENO, "Cannot find such command: ", 26);
	ft_putstr_fd(msg, STDERR_FILENO);
	write(STDERR_FILENO, "\n", 1);
	exit(127);
}

void	error_exit(const char *msg, t_pipex *pipex)
{
	if (pipex)
		close_fds_files(pipex);
	ft_putstr_fd(msg, STDERR_FILENO);
	write(STDERR_FILENO, "\n", 1);
	exit(1);
}

void	first_error_exit(const char *msg)
{
	ft_putstr_fd(msg, STDERR_FILENO);
	write(STDERR_FILENO, "\n", 1);
	exit(1);
}

/*
 * simple error msg
 * 	*/
