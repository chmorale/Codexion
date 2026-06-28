typedef struct s_data
{
	pthread_t		*tid;
	int				coders_num;
	int				compile_nb;
	int				burnout;
	int				finished;
	t_coders		*coders;
	u_int64_t		burnout_time;
	u_int64_t		compile_time;
	u_int64_t		debug_time;
	u_int64_t		refactor_time;
	u_int64_t		dongle_cooldown;
	str				scheduler;
	pthread_mutex_t	*forks;
	pthread_mutex_t	lock;
	pthread_mutex_t	write;
}	t_data;
