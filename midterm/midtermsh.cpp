#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <string>
#include <vector>
#include <iostream>

int find(char c, char* mas, int size) {
	int i = 0;
	//write(1, mas, size);
	for (i = 0; i < size; i++) {
		if (mas[i] == c) {
			return i;		
		}
	}
	return size;
}

void split_args(std::vector<std::string> *vec, std::string mas, int size, char c) {
	int i = 0;
	int	isCom = 0;
	std::string comm = "";
	for (i = 0; i < size; i++) {
		if ((mas[i] == c) && !isCom) {
			std::string tt = std::string(comm);
			vec->push_back(tt);
			//std::cout << comm << " ";
			comm = "";
		} else {
			if ('\n' != mas[i]) {
				comm += mas[i];
			}
		}
		if (mas[i] == '\'') {
			isCom ^= true;
		}
	}
	if (comm.length() > 0) {
		vec -> push_back(comm);
			//std::cout << comm << " ";
	}
	//std::cout << std::endl;
} 

void copy(char* from, int fbegin,  char* to, int tbegin, int size) {
	int i = 0;	
	for (i = 0; i < size; i++) {
		to[i + tbegin] = from[i + fbegin];
	}
}


void execute_func(int fd_in, int fd_out,const char* str, char* const* args) {
	int status = 0;
	int comm_stat;
	std::cout << "Im here" << std::endl;
	int id = fork();
	if (id == 0) {
		std::cout << "child" << std::endl;
		std::cout << "fd\'s " << fd_in << " " << fd_out << std::endl;
		dup2(fd_in, 0);
		dup2(fd_out, 1);
		//args[0] = "echo";
		//execvp("echo", args);
		execvp(str, args);
		perror("");
		return;
		//write(1,"Hello!!!",8);
	} else {
		std::cout << "wait" << std::endl;
		wait(&id);		
		std::cout << "wait_ended" << std::endl;
	}
}

void execute_one(std::string buff, int size, int fd_in, int fd_out) {
	std::vector<std::string> vec = std::vector<std::string>();
	//vec.push_back(vec[vec.size() - 1]);
	split_args(&vec, buff, size, ' '); 
	//std::cout << vec.size() << " ";
/*
	for (int i = 0; i < vec.size(); i++) {
		std::cout << vec[i] << " ";
	}
	std::cout << std::endl;*/
	execute_func(fd_in, fd_out, vec[0].c_str(), (char* const*) vec.data());
	vec.clear();
}

int main(int argc, char* argv[]) {
	int run = 1;
	
	while (run) {
		int size = 0;  
		int tmp_buff_size = 1;
		int buff_size = 10240;
		char tmp_buff[tmp_buff_size]; 
		char buff[buff_size];
		size = 0;
		write(1, "$\n", 2);
		int status = 1;
		while (status) {
			status = read(0, tmp_buff, tmp_buff_size);
			if (status > 0) {
				copy(tmp_buff, 0, buff, size, tmp_buff_size);
				size += status;
				if (buff[size - 1] == '\n') {
					status = 0;
					size--;
				}
			} else {
				run = 0;
				status = 0;
			}
		}
		if (run && size) {
			std::vector<std::string> vec = std::vector<std::string>();
			split_args(&vec, buff, size, '|');
			int fd_in;
			int fd_out;
			int fd_old[2];
			int fd_new[2];
			std::cout << vec.size() << " ";
			for (int i = 0; i < vec.size(); i++) {
			std::cout << "start analis " << vec[i] << std::endl;
				if (!i) {
					if (vec.size() > 1) {
						pipe(fd_new);
						fd_in = 0;
						fd_out = fd_new[1];
					} else {
						fd_in = 0;
						fd_out = 1;
					}
				} else {
					if (i == vec.size() - 1) {
/*						if (fd_in != 0) {
							close(fd_in);
						}
	*/					fd_in = fd_new[0];/*
						if (fd_out != 1) {
							close(fd_out);
						}*/
						fd_out = 1;
					} else {
						/*if (fd_in != 0) {
							close(fd_in);
						}*/
						fd_in = fd_new[0];
						pipe(fd_new);/*
						if (fd_out != 1) {
							close(fd_out);
						}*/
						fd_out = fd_new[1];
					}
				}
				execute_one(vec[i], vec[i].size(), fd_in , fd_out);
				vec.clear();
			}
			if (fd_out != 1) {
				close(fd_out);
			}
			if (fd_in != 0) {
				close(fd_in);
			}
		}
		std::cout<< "end_of_command" << std::endl;
	}
}

