#include <unistd.h>
#include <errno.h>

int main(int argc, char* argv[]) {
	int BUFF_SIZE = 1024;
	char buff[BUFF_SIZE];
	int status_in = 1;
	int length = 1;
	int fdin = 0;
	if (argc == 2) {
		fdin = open(argv[1]);
		if (fdin < 0) {
			status_in = -1;
		} 
	}
	while (status_in > 0) {
		status_in = read(fdin, buff, BUFF_SIZE);
		if (status_in > 0) {
			length = status_in;
			while (length > 0) {
				int status_out = write(1, buff, length);
				if (status_out >= 0) {
					length -= status_out;
				} else { 				
					if (errno == EINTR) {
						status_out = 1;
					} else {
						status_in = -1;
						close(fdin);
						return 1;
					}
				}
			}
		}
		if (errno == EINTR) {
			status_in = 1;	
		} else {
			status_in = -1;
			close(fdin);
			return 1;
		}
	}
	return 0;
}
