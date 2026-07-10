#ifndef CODEX_H
# define CODEX_H

#include <stdio.h>
#include <sys/types.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct s_data
{
	int				coder_num;
	u_int64_t		burnout_time;
	u_int64_t		compile_time;
	u_int64_t		debug_time;
	u_int64_t		refactor_time;
    int				compiles_required;
	u_int64_t		dongle_cooldown;
	char 			* scheduler;
	pthread_t		*tid;
	int				compile_nb;
	int				burnout;
	int				finished;
	t_coder 		*coders;
	pthread_mutex_t	*dongles;
	pthread_mutex_t	lock;
	pthread_mutex_t	write;
}	t_data;

int ft_atoi(const char *str);
int     init(t_data *data, char **argv, int argc);

#endif
