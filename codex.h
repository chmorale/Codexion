typedef struct s_data
{
	int				coders_num;
	u_int64_t		burnout_time;
	u_int64_t		compile_time;
	u_int64_t		debug_time;
	u_int64_t		refactor_time;
    int				compiles_required;
	u_int64_t		dongle_cooldown;
	char			scheduler;
}	t_data

!	pthread_t		*tid;
!	int				compile_nb;
!	int				burnout;
!	int				finished;
!	t_coders		*coders;
!	pthread_mutex_t	*dongle;
!	pthread_mutex_t	lock;
!	pthread_mutex_t	write;
!}	t_data;

