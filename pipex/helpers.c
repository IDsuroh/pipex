/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helpers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suroh <suroh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/26 18:29:47 by suroh             #+#    #+#             */
/*   Updated: 2024/12/03 16:19:21 by suroh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

void	pipe_reader(int fd_in)
{
	char	*buffer;
	ssize_t	bytes_read;

	buffer = (char *)malloc(BUFFER_SIZE + 1);
	if (!buffer)
	{
		ft_putstr_fd("malloc failed", 2);
		return ;
	}
	bytes_read = read(fd_in, buffer, BUFFER_SIZE);
	while (bytes_read > 0)
	{
		buffer[bytes_read] = '\0';
		write(STDOUT_FILENO, buffer, bytes_read);
		bytes_read = read(fd_in, buffer, BUFFER_SIZE);
	}
	if (bytes_read == -1)
		ft_putstr_fd("read failed", 2);
	free(buffer);
}

void	free_split(char **split_array)
{
	int	i;

	if (!split_array)
		return ;
	i = 0;
	while (split_array[i])
	{
		free(split_array[i]);
		i++;
	}
	free(split_array);
}

void	close_fds_files(t_pipex *pipex)
{
	if (pipex->fd[0] >= 0)
	{
		close(pipex->fd[0]);
		pipex->fd[0] = -1;
	}
	if (pipex->fd[1] >= 0)
	{
		close(pipex->fd[1]);
		pipex->fd[1] = -1;
	}
	if (pipex->filein >= 0)
	{
		close(pipex->filein);
		pipex->filein = -1;
	}
	if (pipex->fileout >= 0)
	{
		close(pipex->fileout);
		pipex->fileout = -1;
	}
}

void	close_fds(t_pipex *pipex)
{
	if (pipex->fd[0] >= 0)
	{
		close(pipex->fd[0]);
		pipex->fd[0] = -1;
	}
	if (pipex->fd[1] >= 0)
	{
		close(pipex->fd[1]);
		pipex->fd[1] = -1;
	}
}
