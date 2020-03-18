#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>
#include<signal.h>

int data = 1;

void parent_count(int sig){
    printf("-----[the parent process]-----------%d\n",data);
    data += 2;
    kill(getpid() + 1 , SIGUSR2);
}

void child_count(int sig){
    printf("-----[the forked child process]-----%d\n",data);      
    data += 2;
    kill(getppid() , SIGUSR1);
}

int main()
{
    
    pid_t pid;
    //do fork
    pid = fork();

    if(pid<0)
        printf("error in fork!\n");

    
    else if(pid == 0){  
        //child process created by fork()
        if (signal(SIGUSR2 , child_count) != SIG_ERR){
            printf("-----[the forked child process]-----ID : %d\n",getpid());
            data += 1;
            kill(getppid(), SIGUSR1);
        }
    }
 
    else{    
        if (signal(SIGUSR1 , parent_count) != SIG_ERR){
		    printf("-----[the parent process]-----ID : %d\n",getpid());
	    }
    }
    while(data <= 100);
    return 0;
}