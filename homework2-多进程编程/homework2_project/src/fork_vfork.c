#include<sys/types.h>
#include<unistd.h>
#include<stdio.h>
 
int main()
{
    int data = 1;
    pid_t pid;
    //do fork
    pid = fork();

    if(pid<0)
        printf("error in fork!\n");

    
    else if(pid == 0){
        //child process created by fork()
        printf("-----[the forked child process]-----ID : %d\n",getpid());
        data += 1;
        printf("-----[the forked child process]-----data : %d\n\n",data);
        exit(0);
    }
 
    else{
        printf("-----[the parent process]-----ID : %d\n",getpid());
        printf("-----[the parent process]-----data : %d\n",data);
        wait(0);
        printf("-----[the parent process]-----after [the forked child process], data : %d\n\n",data);

        //do vfork
        pid = vfork();

        if(pid<0)
        printf("error in fork!\n");

        
        else if(pid == 0){
            //child process created by vfork()      
            printf("-----[the vforked child process]-----ID : %d\n",getpid());
            printf("-----[the vforked child process]-----sleeping...zzzzz\n 5 seconds later\n");
            sleep(5);
            data += 1;
            printf("-----[the vforked child process]-----data : %d\n\n",data);
            exit(0);
        }
        else if(pid > 0)
        {
            printf("-----[the parent process]-----ID is %d\n",getpid());
            printf("-----[the parent process]-----data is %d\n",data);
            printf("-----[the parent process]-----after [the vforked child process], data : %d\n\n",data);
        }
    }
    return 0;
    
 
}