typedef struct __motor_control_thread_struct {
	// "persistent" data for this "lthread" would go here
	int	data;
        int index;
} motor_control_thread_struct;

int motor_control_thread(motor_control_thread_struct *,int,int,unsigned char*);