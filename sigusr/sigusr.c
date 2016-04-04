#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

void sig_func(int sig_id, siginfo_t* info, void* context) {
	size_t id = info->si_pid;
	switch(sig_id) {
		case SIGUSR1:
			write(1,"SIGUSR1 from ", 13);
			my_write_num(id);
			write(1,"\n", 1);
			exit(0);
		case SIGUSR2:
			write(1,"SIGUSR2 from ", 13);
			my_write_num(id);
			write(1,"\n", 1);
			exit(0);
		default:
			write(1,"Strange error\n", 14);
			exit(1);
	}			
}

void my_write_num(size_t i) {
	while(i > 0) {
		int t = ((int)'0') + i % 10;
		write(1, &t, 1);
		i /= 10;
	}
}



int main(int argc, char* argv[]) {
	struct sigaction setup;
	sigset_t mask;
	sigemptyset(&mask);
	sigaddset (&mask, SIGUSR1);
	sigaddset (&mask, SIGUSR2);
	setup.sa_sigaction = &sig_func;
	setup.sa_mask = mask;
	setup.sa_flags = SA_SIGINFO;

	if (sigaction(SIGUSR1, &setup, NULL) || sigaction(SIGUSR2, &setup, NULL)) {
        perror("Cannot set sighandlers"); 
    }
	sleep(10);
	write(1,"No signals were caught\n", 23);
	return 0;
}
