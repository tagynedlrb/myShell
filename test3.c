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

typedef struct back_proc_list{
	pid_t pid;
	char *name;
}element;

void childsignal();
void delete_at(int);
void delete_proc(pid_t);
void view_back_proc();
pid_t pid1, for_pid;
element back_list[100];
int amper = 0; // to check if background
int fg = 0; // to check if foreground running
int proc_cnt = 0;


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
	printf("string after amper = %s", str);
			amper = 1;
		}
		else
			amper = 0;


		/*launch process*/
		if((pid1 = fork()) == 0){
			command1 = str;
//printf("command : %s\n", command1);

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

		/* if foreground*/
		if(amper == 0){
			fg = 1;
			for_pid = pid1;
		} 
		/* if background*/
		else{
			element proc;
			proc.name = command1;
			proc.pid = pid1;
			back_list[proc_cnt++] = proc;
		}

view_back_proc();
		// wait if foreground process
		while(fg == 1){
			pause();
			printf("Wakened in pause()! %d\n", ++count);
		}

	}
}

void childsignal(){

	int pid, status;
	pid = waitpid(-1, &status, 0);
		/*foreground process is terminated, set fg = 0*/
	if(pid == for_pid){
		printf("fg is set to 0\n");
		fg = 0;
	}
	else{
		delete_proc(pid);
	}


	printf("called %d\n", pid);
}

void delete_at(int index){
	for(int i = index; i<proc_cnt; i++)
		back_list[i] = back_list[i+1];
	--proc_cnt;
}

void delete_proc(pid_t pid){
	for(int i = 0; i<proc_cnt; i++){
		if(back_list[i].pid == pid){
			delete_at(i);
			break;
		}
	}
}
void view_back_proc(){
	printf("name       PID\n");
	for(int i = 0; i<proc_cnt; i++){
		printf("%10s %d\n", back_list[i].name, back_list[i].pid);
	}
}
