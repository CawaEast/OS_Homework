#include <unistd.h>

int main(int argc, char* argv[]) {
	int BUFF_SIZE = 1024;
	char* buff;
	ssize_t status = 1;
	while (status > 0) {
		status = read(0, buff, BUFF_SIZE);
		if (status > 0) { 
			write(1, buff, status);
		}
	}
	return 0;
}
