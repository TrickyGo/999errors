#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h> 
#include <time.h> 

//#include <sys/types.h>  
#include <unistd.h>  
#include <fcntl.h>  
#include <linux/rtc.h>  
#include <sys/ioctl.h> 
//#include <stdio.h>  
//#include <stdlib.h>  
#include <sys/stat.h>

int main(int argc, char *argv[])
{
    int sockfd = 0, n = 0;
    char recvBuff[1024];
    struct sockaddr_in serv_addr; 
    time_t ticks;
    //read scull
    int fd;  
    //int i;  
    char data[256] = {0};  
    int retval;  
    int seek=0;
    
 
    
    if(argc != 3)
    {
        printf("\n Usage: %s <ip of server> \n",argv[0]);
        return 1;
    } 
    char *name = argv[2];
    memset(recvBuff, '0',sizeof(recvBuff));
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Error : Could not create socket \n");
        return 1;
    } 

    memset(&serv_addr, '0', sizeof(serv_addr)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(5000); 

    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
        return 1;
    } 

    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
       printf("\n Error : Connect Failed \n");
       return 1;
    } 
    //scull open
    fd = open(name, O_RDWR | O_CREAT);  
    if (fd == -1) {  
        perror("open error\n");  
        exit(-1);  
    }  
    printf("open %s successfully\n", name);  
    int ending = lseek(fd, 0, SEEK_END);  
    if (retval == -1) {  
        perror("lseek error\n");
        close(fd);
        exit(-1);  
    }  
    retval = lseek(fd, 0, SEEK_SET);  
    if (retval == -1) {  
        perror("lseek error\n");
        close(fd);
        exit(-1);  
    }  
   while(ending>seek){
    //scull read
    retval = read(fd, data, 10);  
    if (retval == -1) {  
        perror("read error\n");
        close(fd);
        exit(-1);  
    }  
    else printf("read successfully: %s\n", data);
    //scull next
    seek = lseek(fd, 10, SEEK_CUR);  
    if (retval == -1) {  
        perror("lseek error\n");
        close(fd);
        exit(-1);  
    }  
    else printf("lseek successfully\n");
    printf("connected with tricky's server!\n");

    printf("Writing to tricky's server\n");
    ticks = time(NULL);
    //snprintf(recvBuff, sizeof(recvBuff), "%.24s\r\n", ctime(&ticks));
    
    write(sockfd,data, strlen(data));
    printf("send from tricky's client: %s\n", data);

     sleep(1);

  /*  printf("tricky's server says:\n");
    while ( (n = read(sockfd, recvBuff, sizeof(recvBuff)-1)) > 0)
    {
        recvBuff[n] = 0;
        if(fputs(recvBuff, stdout) == EOF)
        {
            printf("\n Error : Fputs error\n");
        }
    } 

    if(n < 0)
    {
        printf("\n Read error \n");
  */ 
}
    return 0;


}
