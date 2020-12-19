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

	while(1){

		int status, count = 0;
		int amper = 0; // to check if background
		signal(SIGCHLD, childsignal);

	//	read()

		// check is Background
		if(1)	//has & )
			amper = 1;
		else
			amper = 0;


		/*launch background process*/
		if((pid1 = fork()) == 0){
/*
			if(redirect input? has >)
				fd = create(newfile)
			if(redirect output? has <)
				fd = create(newfile)
			if(pipe? has | ){
				pipe();
				if((pid2 = fork()) == 0){
					stdout...
				}
				else{
					stdin...
				}
				execlp();
			}
*/
		}

		// wait if foreground process
//		if(amper == 0)
//			wait();

		/*launch background process*/	

		
		while(fg == 1){
			pause();
			printf("Wakened in pause()! %d\n", ++count);
		}

		printf("bye\n");
	}
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
