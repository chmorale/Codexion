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
# define ALLOC_ERR_1 "Error: Dongles allocation failed.\n"
# define ALLOC_ERR_2 "Error: Coders allocation failed.\n"
# define ALLOC_ERR_3 "Error: Dongle timestamp allocation failed.\n"

# include <stdio.h>
# include <sys/types.h>
# include <stdint.h>
# include <stdlib.h>
# include <string.h>
# include <pthread.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_data	t_data;

typedef struct s_dongle_pair
{
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;
	int				first_idx;
	int				second_idx;
}	t_dongle_pair;

typedef struct s_coder
{
	int					id;
	pthread_t			thread_id;
	int					status;
	int					compile_cont;
	uint64_t			time_to_burnout;
	uint64_t			priority;
	pthread_mutex_t		*l_dongle;
	pthread_mutex_t		*r_dongle;
	int					l_idx;
	int					r_idx;
	pthread_mutex_t		lock;
	struct s_data		*data;
}	t_coder;

typedef struct s_heap
{
	t_coder	**array;
	int		size;
	int		capacity;
}	t_heap;

typedef struct s_data
{
	int				coder_num;
	u_int64_t		init_time;
	u_int64_t		burnout_time;
	u_int64_t		compile_time;
	u_int64_t		debug_time;
	u_int64_t		refactor_time;
	int				compiles_required;
	u_int64_t		dongle_cooldown;
	u_int64_t		*dongle_released_at;
	char			*scheduler;
	int				finished;
	t_coder			*coders;
	pthread_mutex_t	*dongles;
	pthread_mutex_t	lock;
	pthread_mutex_t	write;
	t_heap			*heap;
}	t_data;

void		clean_and_exit(t_data *data);
int			take_dongles_and_compile(t_data *data, t_coder *coder);
int			ft_atoi(const char *nptr);
int			init(t_data *data);
int			valid_data(t_data *data, char *argv[], int argc);
int			thread_init(t_data *data);
int			input_checker(char **argv);
int			error(char *msg, t_data *data);
void		free_all(t_data *data);
void		compile(t_data *data, t_coder *coder);
void		debug(t_data *data, t_coder *coder);
void		refactor(t_data *data, t_coder *coder);
long long	get_time(void);
void		print_status(t_coder *coder, char *text);
void		*routine(void *arg);
void		free_coders(t_data *data);
void		free_dongles(t_data *data);
void		free_misc(t_data *data);
void		select_dongles(t_coder *coder, t_dongle_pair *pair);
void		do_compile(t_data *data, t_coder *coder, t_dongle_pair *pair);
void		drop_dongle(pthread_mutex_t *dongle, int idx, t_data *data);
void		heap_push(t_heap *heap, t_coder *coder);
t_coder		*heap_pop(t_heap *heap);

#endif
