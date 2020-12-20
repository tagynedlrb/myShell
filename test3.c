#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
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
		int fd;

		// for exec
		char *args[64];
		char **next = args;


		printf("[myshell]");
		fgets(str, sizeof(str), stdin);
		str[strlen(str)-1] = '\0';

		// register signal
		int status, count = 0;
		signal(SIGCHLD, childsignal);

		// check is Background
		if(strchr(str,'&') != NULL){
			*(strchr(str, '&')) = '\0';
			amper = 1;
		}
		else
			amper = 0;


		/*launch process*/
		if((pid1 = fork()) == 0){
			command1 = str;
printf("command : %s\n", command1);

			/* Input redirection*/
			if(strchr(str,'<') != NULL){
				command1 = strtok(str, "< ");
				command2 = strtok(NULL, "< ");
				fd = open(command2, O_CREAT|O_TRUNC|O_WRONLY, 0600);
				dup2(fd, 0);
				close(fd);
			}

			/* Output redirection*/
			if(strchr(str,'>') != NULL){
				command1 = strtok(str, "> ");
				command2 = strtok(NULL, "> ");
				fd = open(command2, O_CREAT|O_TRUNC|O_WRONLY, 0600);
				dup2(fd, 1);
				close(fd);
			}

			/* Use pipe*/
			if(strchr(str,'|') != NULL){
				int fd[2];

				command1 = strtok(str, "|");
				command2 = strtok(NULL, "|");
				pipe(fd);
				if(fork() == 0){
					close(fd[READ]);
					dup2(fd[WRITE], 1);
					close(fd[WRITE]);

					/* executing routine*/
					char *temp = strtok(command1, " \n");
					while(temp != NULL){
						*next++ = temp;
						//printf("%s\n", temp);
						temp = strtok(NULL, " \n");
					}
					*next = NULL;

					for(next = args; *next != 0; next++)
						//puts(*next);
					execvp(args[0], args);
					perror("pipe");
				}
				else{
					close(fd[WRITE]);
					dup2(fd[READ], 0);
					close(fd[READ]);

					/* executing routine*/
					char *temp = strtok(command2, " \n");
					while(temp != NULL){
						*next++ = temp;
						//printf("%s\n", temp);
						temp = strtok(NULL, " \n");
					}
					*next = NULL;

					for(next = args; *next != 0; next++)
						//puts(*next);
					execvp(args[0], args);
					perror("pipe");
				}
			} // end pipe

			/* executing routine*/
			char *temp = strtok(command1, " \n");
			while(temp != NULL){
				*next++ = temp;
				//printf("%s\n", temp);
				temp = strtok(NULL, " \n");
			}
			*next = NULL;

			for(next = args; *next != 0; next++)
				//puts(*next);
			execvp(args[0], args);
		}

		// wait if foreground process
		if(amper == 0)
			wait(&status);

		while(amper == 1){
			pause();
			printf("Wakened in pause()! %d\n", ++count);
		}

//		printf("bye\n");
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
