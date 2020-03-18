#include <iostream>
 
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
 
#include <sys/stat.h>
#include <sys/types.h>
 
using namespace std;
 
#define SERVER_FIFO_PATH "./SERVER2CLIENT"
#define CLIENT_FIFO_PATH "./CLIENT2SERVER"
int main()
{
//创建server2client的管道
    if (mkfifo(SERVER_FIFO_PATH, 0777) < 0 && errno != EEXIST)
    {
        cout<<"SERVER1:create fifo failed..."<<endl;
        return -1;
    }
//创建父子无名管道
    int fd[2];
    if (pipe(fd) < 0)
    {
        cout<<"SERVER1:create pipe failed..."<<endl;
        return -1;
    }
    char buf[256];
    int i=0;
    int j=0;
//在子进程中读取父进程发来的消息并输出
    if (fork() == 0)
    {
while(j<2){
        close(fd[1]);
        
        read(fd[0], buf, sizeof(buf));
        cout<<"SERVER1CHILD:receive message from pipe:"<<buf<<endl;
        j++;
        }
        exit(0);

    }
//sleep(3);


    
    while(i<2)
    {
        i++;
//从client_fifo中读取内容
        int readfd = open(CLIENT_FIFO_PATH, O_RDONLY);
        cout<<"SERVER1 open serverfifo success..."<<endl;
        read(readfd, buf, sizeof(buf));
        cout<<"SERVER1PARENT:receive message from fifo:"<<buf<<endl;
        close(readfd);
//读到后写给serverChild进程
        write(fd[1], buf, strlen(buf) + 1);
        close(fd[0]);
//往server_fifo中写入东西
        int writefd = open(SERVER_FIFO_PATH, O_WRONLY);
        cout<<"SERVER open clientfifo success..."<<endl;
        char *temp = "SERVER to CLIENT";
        write(writefd, temp, strlen(temp) + 1);
        close(writefd);

    }


    return 0;
}
