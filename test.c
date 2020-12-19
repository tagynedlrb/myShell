#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

void childsignal();
pid_t pid1, pid2, pid3;
int fg = 0;

int main(void){

	int status, count = 0;
	signal(SIGCHLD, childsignal);

	/*launch background process*/
	if((pid1 = fork()) == 0){
		sleep(3);
		printf("1st child is terminated\n");
		exit(1);
	}

	/*launch foreground process*/	
	if((pid2 = fork()) == 0){
		sleep(6);
		printf("2nd child is terminated\n");
		exit(2);
	}
	fg = 1;

	/*launch background process*/	
	if((pid3 = fork()) == 0){
		sleep(9);
		printf("3rd child is terminated\n");
		exit(3);
	}

	printf("%d, %d, %d\n", pid1, pid2, pid3);
	
	while(fg == 1){
		pause();
		printf("Wakened in pause()! %d\n", ++count);
	}

	printf("bye\n");
}

void childsignal(){

	int pid, status;
	pid = waitpid(-1, &status, 0);
		/*foreground process is terminated, set fg = 0*/
	if(pid == pid2){
		printf("fg is set to 0\n");
		fg = 0;
	}


	printf("called %d\n", pid);
}
