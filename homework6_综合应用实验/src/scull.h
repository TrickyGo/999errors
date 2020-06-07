#ifndef _SCULL_H
#define _SCULL_H
 
#define SCULL_MAJOR 0
#define SCULL_BUFFER_SIZE PAGE_SIZE
 
struct scull_dev {
    char *data;
    unsigned long size;
    struct semaphore sem;
    struct cdev cdev;
};
 
#endif
