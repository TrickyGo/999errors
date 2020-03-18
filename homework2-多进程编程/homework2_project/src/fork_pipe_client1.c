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
//创建client2server的管道
    if (mkfifo(CLIENT_FIFO_PATH, 0777) < 0 && errno != EEXIST)
    {
        cout<<"CLIENT1:create fifo failed..."<<endl;
        return -1;
    }
//创建父子无名管道
    int fd[2];
    if (pipe(fd) < 0)
    {
        cout<<"CLIENT1:create pipe failed..."<<endl;
        return -1;
    }
    char buf[256];
//在子进程中读取父进程发来的消息并输出
    if (fork() == 0)
    {
        close(fd[1]);
        
        read(fd[0], buf, sizeof(buf));
        cout<<"CLIENT1CHILD:receive message from pipe:"<<buf<<endl;
        
        exit(0);

    }
//sleep(3);

//往client_fifo中写入东西
    int writefd = open(CLIENT_FIFO_PATH, O_WRONLY);
    cout<<"CLENT1 open clientfifo success..."<<endl;
    char *temp = "CLENT1 to SERVER";
    write(writefd, temp, strlen(temp) + 1);
    close(writefd);
//从server_fifo中读取内容
    int readfd = open(SERVER_FIFO_PATH, O_RDONLY);
    cout<<"CLENT1 open serverfifo success..."<<endl;
    read(readfd, buf, sizeof(buf));
    cout<<"CLIENT1PARENT:receive message from fifo:"<<buf<<endl;
    close(readfd);
//读到后写给clientChild进程
    write(fd[1], buf, strlen(buf) + 1);
    close(fd[0]);
    return 0;
}
