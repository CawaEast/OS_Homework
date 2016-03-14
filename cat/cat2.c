#include <unistd.h>

int main(int argc, char* argv[]) {
	int BUFF_SIZE = 1024;
	char buff[BUFF_SIZE];
	int status = 1;
	while (status > 0) {
		status = read(0, buff, BUFF_SIZE);
		if (status > 0) { 
			int j = write(1, buff, status);
		}
	}
	return 0;
}
