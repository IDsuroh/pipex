/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: suroh <suroh@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 21:42:05 by suroh             #+#    #+#             */
/*   Updated: 2024/12/03 16:47:06 by suroh            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static void	init_pipex(t_pipex *pipex, char **av)
{
	pipex->fileout = open(av[4], O_WRONLY | O_CREAT | O_TRUNC, 0664);
	if (access(av[1], F_OK) != 0)
	{
		perror(av[1]);
		pipex->filein = -1;
	}
	else if (access(av[1], R_OK) != 0)
	{
		ft_putstr_fd("Permission Denied\n", STDOUT_FILENO);
		exit(0);
	}
	else
		pipex->filein = open(av[1], O_RDONLY);
	if (pipe(pipex->fd) == -1)
		error_exit("Pipe creation failed", pipex);
}

static pid_t	handle_child_process(t_data *data, t_pipex *pipex)
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		error_exit("fork failed,", pipex);
	if (pid == 0)
		data->child_func(data->av, data->envp, data->fd, pipex);
	return (pid);
}

void	child_process_1(char **av, char **envp, int *fd, t_pipex *pipex)
{
	if ((*pipex).filein == -1)
		exit(0);
	dup2(fd[1], STDOUT_FILENO);
	dup2((*pipex).filein, STDIN_FILENO);
	close(fd[0]);
	close(fd[1]);
	close((*pipex).filein);
	close((*pipex).fileout);
	if (av[2][0] != '\0' && av[2])
		execute(av[2], envp, pipex);
	exit(1);
}

void	child_process_2(char **av, char **envp, int *fd, t_pipex *pipex)
{
	if ((*pipex).fileout == -1)
		error_exit(av[4], pipex);
	dup2(fd[0], STDIN_FILENO);
	dup2((*pipex).fileout, STDOUT_FILENO);
	close(fd[1]);
	close(fd[0]);
	close((*pipex).filein);
	close((*pipex).fileout);
	if (av[3][0] != '\0' && av[3])
		execute(av[3], envp, pipex);
	else
	{
		pipe_reader(STDIN_FILENO);
		close(STDIN_FILENO);
	}
	exit(1);
}

int	main(int ac, char **av, char **envp)
{
	t_pipex	pipex;
	t_data	data1;
	t_data	data2;
	int		status2;

	if (ac != 5)
		first_error_exit("\n\t\t\033[31mError: Wrong Format\n\n\e[0m");
	data1 = (t_data){av, envp, pipex.fd, pipex.filein, child_process_1};
	data2 = (t_data){av, envp, pipex.fd, pipex.fileout, child_process_2};
	init_pipex(&pipex, av);
	pipex.pid1 = handle_child_process(&data1, &pipex);
	pipex.pid2 = handle_child_process(&data2, &pipex);
	close_fds(&pipex);
	waitpid(pipex.pid1, NULL, 0);
	status2 = get_child_exit_status(pipex.pid2);
	if (pipex.fileout == -1)
		error_exit("Don't have permission to Write|Create|Truncate", &pipex);
	if (status2 != 0)
	{
		close_fds_files(&pipex);
		exit(status2);
	}
	close_fds_files(&pipex);
}

//valgrind --leak-check=full --show-reachable=yes --track-origins=yes
//--trace-children=yes  --track-fds=yes
//./pipex file1 "ls -l" "wc -l" file2 
//
// run; valgrind usr/bin/"The Command that makes reachable memories"
//
//
//
// 'ls' commands and 'grep' and 
// still run valgrind error saying there is reachable memory
// and possibly lost
// this is because
//
// Memory Management in grep
//
// The memory reported by Valgrind as "still reachable" or
// "possibly lost" is most likely caused by:
//
//      Memory allocated by libraries for regex or string processing.
//      Buffers or data structures that are designed to persist for the
//		lifetime of the process.
//
// These are not leaks in the usual sense, as the memory is still
// managed and will be freed automatically when the process exits.
// However, they are reported by Valgrind because the memory was not
// explicitly freed before termination.
//
// General Explanation
//
//     How Memory Is Allocated:
//         Commands allocate memory for their operations using libraries
//			or manual allocation.
//         Some allocations are for the program's entire lifetime and
//			are not explicitly freed.
//
//     How Memory Is Not Freed:
//         Memory for non-critical resources, such as locale data or
//			regex patterns, is often left unfreed.
//         Commands rely on the operating system to clean up after
//			termination.
//
//     Why It's Not a Problem:
//         The "remaining memory" does not grow indefinitely—it is
//			tied to specific operations.
//         The memory is reclaimed by the operating system
//			when the process ends.
//
//     Why Valgrind Reports It:
//         Valgrind checks for explicit free() calls
//			to ensure no memory is orphaned.
//         If the memory is allocated but not freed,
//			it flags it as "still reachable" or "possibly lost."
//
/* Why use Structs?
 * To store data easily and manipulate with more control (in my opinion).
 * more over, to reduce the function lines because norminette is a bitch.
 * data1 = (t_data){av, envp, pipex.fd, pipex.filein, child_process_1};
 * is called a compound literals.
 * It is not in the standard C, but is supported in the C99 standard.
 * and the bitch allows it.
 *
 * Why Use dup2()?
 *
 *   Simplicity in Command Execution:
 *       With dup2(), you can redirect the pipe or file directly to
 *       stdin or stdout.
 *       This allows subsequent commands (execve) to work seamlessly with the
 *       standard input/output streams.
 *   Avoids Manual Read/Write Loops:
 *       By setting up the redirections, you don't need explicit read()
 *       and write() loops in your code to transfer data between processes.
 *
 * The combination of these dup2() calls sets up the process so that:
 *
 * Reading: The process reads from the input file (av[1]) because
 * 	STDIN_FILENO is now connected to filein.
 * Writing: The process writes to the pipe (instead of stdout) because
 * 	STDOUT_FILENO is now connected to fd[1].
 * 0777: is a file permission mode (in octal format). This argument
 * 	is used when creating a new file if it doesn't exist.
 * 	The mode 0777 grants full read, write, and execute permissions
 * 	to the file for the owner, group, and others.
 * Note: In the context of opening an existing file, the third
 * 	argument (permissions) is usually ignored.
 * 	It is only used when creating a new file, where it specifies
 * 	the permissions for the newly created file.
 * 	
 * fd[0] = the read end is duplicated to the STDIN and then printed
 * out to the duplicated STDOUT which is from fileout. The file that
 * is opened and is able to write-only, create if not-exist, truncate
 * (O_TRUNC will erase any data in the file as soon as the file gets
 * opened).
 */
/*
 * In this project we are asked to code a program which will immitate
 * the behavior of this shell command: ❯ < file1 cmd1 | cmd2 > file2
 *	
 *	The idea of the program is that we take an infile and two commands,
 *	and pass the infile as the standard input to the first command, and then
 *	pipe the output of the first command to the second command, and finally
 *	save it all to a second file. 
 *
 * Symbol Meaning:
 *	
 *	<: Used to denote that we will be passing the next argument as
 *		the standard input (stdin)
 *
 * 	file1: Path to the file we want to open as the standard input.
 * 		It must exist and should be opened read-only
 * 
 * 	cmd1: First command. It will receive the stdin and run a command with it,
 * 		if applicable
 * 
 * 	|: Transforms the standard output (stdout) of the first command (cmd1) into
 * 		the standard input (stdin) for the next command (cmd2)
 * 
 * 	cmd2: Receives the standard output of the first command (cmd1) as stdin and
 * 		runs a command with it, if applicable
 *
 * 	>: Redirects whatever is on the standard output (stdout) into a file,
 * 		creating it if it does not exist
 * 
 * 	file2: Path to an output file which may or may not exist. If it exists,
 * 		it will be truncated (emptied) and should be opened write-only
 *
 */
/*
 * Clarification on How envp Works at Program Start:
 *
 * When a program is executed (e.g., from a shell or as a system
 * process), the operating system prepares the environment variables
 * and passes them to the program.
 * The envp array is populated automatically by the operating system
 * with all the environment variables that were present at the time
 * the program was started.
 *
 * Examples:
 *
 *   PATH=/usr/bin:/bin:/usr/local/bin
 *   PATH: Lists directories the system searches for executable files.
 *   HOME: Represents the current user’s home directory.
 *   USER: Stores the name of the current user.
 *   PWD: Contains the current working directory.
 *
 * the envp parameter is automatically recognized.
 * the envp pointer array holds all the environment variables that
 * are set in the system at the time of the program's execution.
 *
 * envp[0] = "PATH=/usr/bin:/bin";
 * envp[1] = "USER=exampleuser";
 * envp[2] = "HOME=/home/exampleuser";
 * envp[3] = "SHELL=/bin/bash";
 * envp[4] = "LANG=en_US.UTF-8";
 * envp[5] = "TERM=xterm-256color";
 * // More environment variables could be here...
 * envp[N] = NULL; // Marks the end of the array
 * 
 * Most modern operating systems (e.g., Linux, macOS) will populate
 * envp with some basic environment variables like PATH, USER, and
 * HOME by default when a program starts.
 *
 * Execution with Context: By passing envp to the execve() call,
 * 	your program ensures that cmd1 and cmd2 have access to all
 * 	the environment variables they need to function correctly.
 * 	Without envp, the new program might not have the correct paths
 * 	or environment settings, leading to issues like
 * 	"command not found" or unexpected behavior.
 * Consistency: Using envp maintains consistency between how your
 * 	pipex program runs the commands and how the shell would execute
 * 	them.
 *
 * What Does waitpid(pid, NULL, 0) Do?
 *
 *	In the code waitpid(pid, NULL, 0), you're telling the parent process to:
 *
 *   Wait for the child with the process ID pid to terminate.
 *   Do nothing with the exit status of the child
 *   	(since the second argument is NULL).
 *   Block the parent until the child process finishes
 *   	(since options is set to 0).
 *	
 * pipex.pid1 is the PID of the first child process created by
 * handle_child_process(&data1). &status1 is the address of an integer
 * variable where waitpid will store the child's exit status.
 * The third parameter (0) means that waitpid will block until the specified
 * child process terminates.
 *
 *	If we don't use waitpid or wait, the child may enter into zombie state.
 *	which can cause errors in commands like;
 * 			./pipex infile "sleep 3" | "ls" outfile
 */
/* If you don't pass envp when using execve():
 *
 *	The command being executed might fail if it relies on any
 *	environment variable, such as PATH.
 *	The new process may not run in the intended environment,
 *	potentially causing errors or unexpected behavior.
 *
 * The function get_child_exit_status checks if the pid successfully ran
 * the commands in the parameter.
 * waitpid(pid, &status, 0);
 * if (WIFEXITED(status))
 *	return (WEXITSTATUS(status));
 * return (1);
 * WIFEXITED–Query status to see if a child process ended normally.
 * This macro queries the child termination status provided by the wait and
 * waitpid functions, and determines whether the child process ended normally.
 * 
 * The condition if (status2 != 0) checks if the second child process.
 * and exits program with exitfunction directly
 */
