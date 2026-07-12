/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chmorale <chmorale@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/06/28 09:30:36 by chmorale          #+#    #+#             */
/*   Updated: 2026/07/11 16:26:01 by chmorale         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEX_H
# define CODEX_H
# define ERR_IN_1 "Error: Invalid character in arguments.\n"
# define ALLOC_ERR_1 "Error: Pointers allocation failed.\n"
# define ALLOC_ERR_2 "Error: Dongles allocation failed.\n"
# define ALLOC_ERR_3 "Error: Coders allocation failed.\n"  
# define ERR_IN_2    "Error: Invalid argument values.\n"

# include <stdio.h>
# include <sys/types.h>
# include <stdint.h>
# include <stdlib.h>
# include <string.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_data	t_data;

typedef struct s_coder
{
	int					id;
	pthread_t			thread_id;
	int					status;
	int					compiling;
	int					compile_cont;
	uint64_t			time_to_burnout;
	pthread_mutex_t		*l_dongle;
	pthread_mutex_t		*r_dongle;
	pthread_mutex_t		lock;
	struct s_data		*data;
}	t_coder;

typedef struct s_data
{
	int				coder_num;
	u_int64_t		burnout_time;
	u_int64_t		compile_time;
	u_int64_t		debug_time;
	u_int64_t		refactor_time;
	int				compiles_required;
	u_int64_t		dongle_cooldown;
	char			*scheduler;
	pthread_t		*tid;
	int				compile_nb;
	int				burnout;
	int				finished;
	t_coder			*coders;
	pthread_mutex_t	*dongles;
	pthread_mutex_t	lock;
	pthread_mutex_t	write;
}	t_data;

void		clean_and_exit(t_data *data);
void		take_dongles_and_compile(t_coder *coder);
int			ft_atoi(const char *str);
int			init(t_data *data, char **argv);
int			valid_data(t_data *data, char *argv[], int argc);
int			thread_init(t_data *data);
int			input_checker(char **argv);
int			is_most_urgent(t_data *data, t_coder *coder);
int			error(char *msg, t_data *data);
void		compile(t_data *data, t_coder *coder);
void		cooldown(t_data *data, t_coder *coder);
void		debug(t_data *data, t_coder *coder);
void		refactor(t_data *data, t_coder *coder);
long long	get_time(void);

#endif
