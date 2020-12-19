#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#define READ 0
#define WRITE 1
#define MAXLINE 100

void childsignal();
pid_t pid1, pid2, pid3;
int amper = 0; // to check if background


int main(void){

	while(1){

		char str[1024];
		char *command1, *command2;

		printf("[myshell]");
		fgets(str, sizeof(str), stdin);
		str[strlen(str)-1] = '\0';

		// register signal
		int status, count = 0;
		signal(SIGCHLD, childsignal);

	//	read()

		// check is Background
		if(1)	//has & )
			amper = 1;
		else
			amper = 0;


		/*launch process*/
		if((pid1 = fork()) == 0){
/*
			if(strchr(str,'>') != NULL){	// input redirection
				command1 = strtok(str, ">");
				command2 = strtok(NULL, ">");
				fd = open(command2, O_CREAT|O_TRUNC|O_WRONLY, 0600);
				dup2(fd, 1);
				close(fd);
			if(strchr(str,'<') != NULL){	// output redirection
				command1 = strtok(str, "<");
				command2 = strtok(NULL, "<");
				fd = open(command2, O_CREAT|O_TRUNC|O_WRONLY, 0600);
				dup2(fd, 1);
				close(fd);
	
				fd = create(newfile)
			if(strchr(str,'|') != NULL){	// use pipe
				int fd[2];

				command1 = strtok(str, "|");
				command2 = strtok(NULL, "|");
				pipe(fd);
				if((pid2 = fork()) == 0){
					close(fd[READ]);
					dup2(fd[WRITE], 1);
					close(fd[WRITE]);
					execlp(command1, command1, NULL);
					perror("pipe);
				}
				else{
					close(fd[WRITE]);
					dup2(fd[READ], 0);
					close(fd[READ]);
					execlp(command2, command2, NULL);
					perror("pipe);
				}
			}
			else{	// Not PIPE


				execlp(command1, command1, NULL);
			}
*/
		}

		// wait if foreground process
//		if(amper == 0)
//			wait();

		/*launch background process*/	

		
		while(amper == 1){
			pause();
			printf("Wakened in pause()! %d\n", ++count);
		}

		printf("bye\n");
	}
}

void childsignal(){

	int pid, status;
	pid = waitpid(-1, &status, 0);
		/*foreground process is terminated, set amper = 0*/
	if(pid == pid2){
		printf("amper is set to 0\n");
		amper = 0;
	}


	printf("called %d\n", pid);
}
