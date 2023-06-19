// The first process sends an integer to the second process
// which adds 5 to the integer and then sends it to a third process
// which adds 7 to it and sends it back to the first process
// The result is then displayed on the screen

#include <stdio.h>
#include <unistd.h>
#include <wait.h>

int main(int argc, char *argv[]) {
	int fd[2];

	if (pipe(fd) == -1) {
    printf("Could not create pipe\n");
    return 1;
  }

	int x = 3;
	if (write(fd[1], &x, sizeof(int)) == -1) {
		printf("Parent could not write to pipe\n");
    return 1;
	};

	int pid1 = fork();

	if (pid1 == -1) {
    printf("Could not fork first child\n");
    return 1;
  }

	if (pid1 == 0) {
		if (read(fd[0], &x, sizeof(int)) == -1) {
			printf("First child could not read from pipe\n");
      return 1;
		};
		x += 5;

		int pid2 = fork();

		if (pid2 == -1) {
			printf("Could not fork second child");
      return 1;
    }

		if (pid2 == 0) {
			if (read(fd[0], &x, sizeof(int)) == -1) {
				printf("Second child could not read from pipe\n");
				return 1;
			}
			close(fd[0]);

			x += 7;

			if (write(fd[1], &x, sizeof(int)) == -1) {
				printf("Second child could not write to pipe\n");
        return 1;
      };
			close(fd[1]);

			return 0;
		}
		
		close(fd[0]);

		if (write(fd[1], &x, sizeof(int)) == -1) {
			printf("First child could not write to pipe\n");
      return 1;
    };
		close(fd[1]);
		wait(NULL);

		return 0;
	}

	close(fd[1]);
	wait(NULL);

	if (read(fd[0], &x, sizeof(int)) == -1) {
		printf("Parent could not read from pipe\n");
    return 1;
	}
	close(fd[0]);

	printf("Total: %d\n", x);

	return 0;
}