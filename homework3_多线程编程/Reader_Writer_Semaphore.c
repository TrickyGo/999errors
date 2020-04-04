#include<stdio.h>
#include<pthread.h>
#include<sched.h>
#include<semaphore.h>

sem_t mutex, writeblock;
int data = 0, rcount = 0;
FILE *fp = NULL;
char buff[255];
char sign[10];
void *reader(void *arg)
{
   
    int f;
    f = ((int)arg);
    sem_wait(&mutex);
    rcount = rcount + 1;
    if(rcount==1)//the first reader
        sem_wait(&writeblock);
    sem_post(&mutex);
    fp = fopen("./test.txt", "r");
    fgets(buff, 255, (FILE*)fp);
    fclose(fp);
    printf("reader_%d read data :%s\n",f,buff);
    sleep(2);

    sem_wait(&mutex);
    rcount = rcount - 1;
    if(rcount==0)//the last reader
        sem_post(&writeblock);
    sem_post(&mutex);
}

void *writer(void *arg)
{

    int f;
    f = ((int) arg);
    sem_wait(&writeblock);
    data++;
    sprintf(sign,"Writer%d",data);
    fp = fopen("./test.txt", "a");
    fputs( sign, (FILE*)fp );
    fclose(fp);
    printf("writer_%d write data:%d\n",f,data);
    sleep(1);
    sem_post(&writeblock);
}


int main()
{   
    //create low priority thread attribute
    pthread_attr_t attr_low;
    int ret;
    int prio_low = 1;
    struct sched_param param_low;

    ret = pthread_attr_init (&attr_low);
    param_low.sched_priority = prio_low;
    ret=pthread_attr_setschedpolicy (&attr_low, SCHED_FIFO);
    pthread_attr_setschedparam (&attr_low, &param_low);

    //create high priority thread attribute
    pthread_attr_t attr_high;
    int high_prio = 90;
    struct sched_param param_high;

    ret = pthread_attr_init (&attr_high);
    param_high.sched_priority = high_prio;
    ret=pthread_attr_setschedpolicy (&attr_high, SCHED_FIFO);
    pthread_attr_setschedparam (&attr_high, &param_high);

    //Task1: Reader First
    fp=fopen("./test.txt","w+");
    fputs("TASK1", (FILE*)fp );
    fclose(fp);
    printf("------Task1: Reader First!-------\n");

    int i; 
    pthread_t rtid[11],wtid[11];
    sem_init(&mutex,0,1);
    sem_init(&writeblock,0,1);
    for(i=0;i<=10;i++)
    {
      pthread_create(&rtid[i],&attr_high,reader,(void *)i); 
      pthread_create(&wtid[i],&attr_low,writer,(void *)i);
  
      //if(i%2 == 0){ 
        //sleep(5);
      //}

    }
    for(i=0;i<=10;i++)
    {
      pthread_join(wtid[i],NULL);
      pthread_join(rtid[i],NULL);
    }
    fp=fopen("./test.txt","w+");
    fputs("TASK2", (FILE*)fp );
    fclose(fp);
    //Task2: Writer First

    data = 0;

    printf("\n------Task2: Writer First!-------\n");

    fp = fopen("./test.txt", "r");
    fgets(buff, 255, (FILE*)fp);
    fclose(fp);
    printf("dierci:%s\n",buff);
    
    sem_init(&mutex,0,1);
    sem_init(&writeblock,0,1);
    for(i=0;i<=10;i++)
    {
      
      pthread_create(&rtid[i],&attr_low,reader,(void *)i); 
      pthread_create(&wtid[i],&attr_high,writer,(void *)i);

    }
    for(i=0;i<=10;i++)
    {
      pthread_join(wtid[i],NULL);
      pthread_join(rtid[i],NULL);
    }
    fp=fopen("./test.txt","w+");
    fclose(fp);
    return 0;
}
