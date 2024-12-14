/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suroh <suroh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 22:34:54 by suroh             #+#    #+#             */
/*   Updated: 2024/12/04 10:30:09 by suroh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

int	get_child_exit_status(pid_t pid)
{
	int	status;

	status = 0;
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

char	*find_in_path(char **full_path, char *command)
{
	char	*bin_path;
	char	*path_tracker;
	int		i;

	i = -1;
	while (full_path[++i])
	{
		path_tracker = ft_strjoin(full_path[i], "/");
		bin_path = ft_strjoin(path_tracker, command);
		free(path_tracker);
		if (access(bin_path, F_OK) == 0)
			return (bin_path);
		free(bin_path);
	}
	return (NULL);
}

char	**get_full_path(char **envp)
{
	int	i;

	i = 0;
	while (envp[i] && !ft_strnstr(envp[i], "PATH=", 5))
		i++;
	if (!envp[i] || !envp[i][5])
		return (NULL);
	return (ft_split(envp[i] + 5, ':'));
}

char	*find_loc(char *command, char **envp)
{
	char	**full_path;
	char	*bin_path;

	full_path = get_full_path(envp);
	if (!full_path)
		return (NULL);
	bin_path = find_in_path(full_path, command);
	free_split(full_path);
	return (bin_path);
}

void	execute(char *av, char **envp, t_pipex *pipex)
{
	char	**command;
	char	*location;

	if (!av || !*av)
	{
		ft_putstr_fd("Empty command ignored\n", STDERR_FILENO);
		exit(0);
	}
	command = ft_split(av, ' ');
	if (!command)
		error_exit("Command Parsing Failed", pipex);
	location = find_loc(command[0], envp);
	if (!location)
	{
		free_split(command);
		error_127(av, pipex);
	}
	if (execve(location, command, envp) == -1)
	{
		free(location);
		free_split(command);
		error_exit("Can't Execute", pipex);
	}
}

/* ft_strnstr() will find if "PATH" is present at the beginning of a
 * 	string by searching each envp[i].
 * Then ft_split() will skip the "PATH=" part and splits the string with
 * 	':'.
 * 
 * for example:
 * envp[0] = "PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin";
 *	full_path[0] = /usr/local/sbin
 *	path_tracker = /usr/local/sbin/
 *	bin_path = /usr/local/sbin/"command or av[2]".
 *
 *	In this case, the function access(bin_path, F_OK) would return
 *	-1. Which means that in that location or path,
 *	the command file is not found.
 *	if the first while loop is passed, it will eventually return
 *	0.
 *	*/
/* On error, execve() will return a -1. So if successfully ran, it will
 * execute command in the location.
 *
 * The flags of the command are handled properly because the execve()
 * function has the whole parameter of command.
 * 	*/
