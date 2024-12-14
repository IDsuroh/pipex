/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suroh <suroh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 14:02:40 by suroh             #+#    #+#             */
/*   Updated: 2024/12/03 14:24:18 by suroh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <errno.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <fcntl.h>
# include "libft/libft.h"
# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif

//Struct to hold pipex_values
typedef struct s_pipex
{
	int		fd[2];
	int		filein;
	int		fileout;
	pid_t	pid1;
	pid_t	pid2;
}	t_pipex;

//Struct to hold Arguments
typedef struct s_child_process_data
{
	char	**av;
	char	**envp;
	int		*fd;
	int		file;
	void	(*child_func)(char **, char **, int*, t_pipex*);
}	t_data;

//error handlers {errors.c}
void	first_error_exit(const char *msg);
void	error_exit(const char *msg, t_pipex *pipex);
void	error_127(const char *msg, t_pipex *pipex);

//utility functions {utils.c}
int		get_child_exit_status(pid_t pid);
char	*find_in_path(char **full_path, char *command);
char	**get_full_path(char **envp);
char	*find_loc(char *command, char **envp);
void	execute(char *av, char **envp, t_pipex *pipex);

//support function {helpers.c}
void	pipe_reader(int fd_in);
void	free_split(char **split_array);
void	close_fds_files(t_pipex *pipex);
void	close_fds(t_pipex *piepx);

//main functions {pipex.c}
void	child_process_1(char **av, char **envp, int *fd, t_pipex *pipex);
void	child_process_2(char **av, char **envp, int *fd, t_pipex *pipex);
int		main(int ac, char **av, char **envp);

#endif
