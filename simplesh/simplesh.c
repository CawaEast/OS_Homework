#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

size_t ARG_SIZE = 1000;
size_t ARGS_NUM = 100;
size_t PIPES_NUM = 100;

int childs[1000];
int childs_num = 0;

void interruption(int id, siginfo_t * sigtinfo, void* context) {
	int i = 0;
	for (i = 0; i < childs_num; i++) {
		kill(childs[i], id);
	}
}

void copy(char* from, int fbegin,  char* to, int tbegin, int size) {
	int i = 0;	
	for (i = 0; i < size; i++) {
		to[i + tbegin] = from[i + fbegin];
	}
}

void wnum(int i) {
	//char str[15];
	printf("%d\n", i);
	//write(1, str, 15);
}

pid_t execute(char* ans, char** args, int fdin, int fdout) {
	pid_t pid;
	if (pid = fork()) {
		if(fdin != 0) {
			close(fdin);
		}
		if(fdout != 1) {
			close(fdout);
		}
		return pid;
		
	} else {
		if(fdin != 0) {
			dup2(fdin, 0);
			close(fdin);
		}
		if(fdout != 1) {
			dup2(fdout, 1);
			close(fdout);
		}/*
		int i = 0;
		while (args[i] != NULL) {
			write(1, args[i], strlen(args[i]));
			write(1, " arg\n", 5);
			i++;
		}*/
		execvp(ans, args);
		perror("wows");
		return -1;
	}
}

pid_t parse_one_cmd(char* command, int size, int fdin, int fdout) {
	//char* str = (char*) malloc(size);
	//copy()
	//write(1, "exec comm:|", 11);
	//write(1, command, size - 1);
	//write(1, "|\n", 2);
	char* cmd[ARGS_NUM];
	int i = 0;
	int n_arg = 0;
	int pointer = 0;
	char** ans = (char**)malloc(ARGS_NUM);
	//memset(ans, 0, sizeof(char) * (ARGS_NUM));
	for(i = 0; i < size; i++) {
		if ((command[i] == ' ') || (i == size - 1)) {
			size_t comm_size = i - pointer;
			if (comm_size != 0) {
				//wnum(comm_size);
				ans[n_arg] = (char*)malloc(comm_size + 1);
				memset(ans[n_arg], 0, sizeof(char) * (comm_size +1));
				memcpy(ans[n_arg], command + pointer, comm_size);
				//copy(command, pointer, ans[n_arg], 0, comm_size);
				//write(1, "arg:|", 5);
				//write(1, ans[n_arg], comm_size);
				//write(1, "|\n", 2);
				n_arg++;
			} else {
			
			}
			pointer = i + 1;
		}
	}
	char* com = (char*) malloc(strlen(ans[0]) + 1);
	memset(com, 0, sizeof(char) * (strlen(ans[0]) + 1));
	memcpy(com, ans[0], strlen(ans[0]));
	//copy(ans[0], 0, com, 0, strlen(ans[0]));
	ans[n_arg] = NULL;
	pid_t pid = execute(com, ans, fdin, fdout);
	for(i = 0; i < n_arg; i++) {
		//write(1, "freeb\n", 6);
		free(ans[i]);
		//write(1, "freee\n", 6);
	}
	free(ans);
	free(com);
	return pid;
}

void run_com(char* string, int size) {
	int p = 0;
	int n_cmd = 0;
	int pfd[PIPES_NUM][2];
	int i;
	int status;
	for (i = 0; i < size; i++) {
		if ((string[i] == '|') || (i == size - 1)) {
			size_t comm_size = i - p + 1;
			int fdin = 0;
			int fdout = 1;
			if (i != size - 1) {
				pipe(pfd[n_cmd]);
				fdout = pfd[n_cmd][1];
			}
			if (n_cmd != 0) {
				fdin = pfd[n_cmd - 1][0];
			}
			pid_t pid = parse_one_cmd(string + p, comm_size, fdin, fdout);
			p = i + 1;
			n_cmd ++;
			if (pid != -1) {
				childs[childs_num++] = pid;
				waitpid(pid, &status,0);
			}
		}
	}
	write(1,"$\n", 2);
}

int main(int argc, char* argv[]) {
	struct sigaction s;
	s.sa_sigaction = &interruption;
	s.sa_flags = SA_SIGINFO;
	sigaction(SIGINT, &s, NULL);
	size_t buff_size = 1024;
	char buff[buff_size];
	int size_read = 1, buff_p = 0, comm_b = 0;
		write(1,"$\n", 2);
	while (size_read) {
		size_read = read(0, buff + buff_p, buff_size - buff_p);
		if (size_read > 0) {
			size_t i = 1;/*
			for(i = 0; i < size_read; i++) {
				write(1, "1", 1);
			}*/
			//write(1, "im here", 7);
			//write_num((int)size_read);
			for (i = buff_p; i < buff_p + size_read; i++) {
				//write(1, "im here", 7);
				if (buff[i] == '\n') {
					size_t comm_size = i - comm_b + 1;
					char command[comm_size];
					copy(buff, comm_b, command, 0, comm_size);
					run_com(command, comm_size);
					comm_b = i + 1;
				}
			}
			buff_p += size_read;
			if (buff_p == comm_b) {
					buff_p = 0;
				comm_b = 0;
			} else {
				copy(buff, comm_b, buff, 0, buff_p - comm_b);
				buff_p -= comm_b;
				comm_b = 0;
			}
		}	
	}
}
