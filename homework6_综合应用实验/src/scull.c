#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/types.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>
#include "scull.h"
#define SCULL_CMD1 1
#define SCULL_CMD2 2
#define SCULL_CMD3 3
int scull_major = 242;
int scull_minor = 0;
 
module_param(scull_major, int, S_IRUGO);
module_param(scull_minor, int, S_IRUGO);
 
struct scull_dev *scull_device;
 
int scull_trim(struct scull_dev *dev)
{
    if (dev)
    {
        if (dev->data)
        {
            kfree(dev->data);
        }
        dev->data = NULL;
        dev->size = 0;
    }
    return 0;
}
 
int scull_open(struct inode *inode, struct file *filp)
{
    struct scull_dev *dev;
 
    dev = container_of(inode->i_cdev, struct scull_dev, cdev);
    filp->private_data = dev;
 
    if ((filp->f_flags & O_ACCMODE) == O_WRONLY)
    {
        if (down_interruptible(&dev->sem))
        {
            return -ERESTARTSYS;
        }
        scull_trim(dev);
        up(&dev->sem);
    }
 
    return 0;
}
 
int scull_release(struct inode *inode, struct file *filp)
{
    return 0;
}
 
ssize_t scull_read(struct file *filp, char __user *buf, size_t count, loff_t *f_pos)
{
    struct scull_dev *dev = filp->private_data;
    ssize_t retval = 0;
 
    if (down_interruptible(&dev->sem))
    {
        return -ERESTARTSYS;
    }
    if (*f_pos >= dev->size)
    {
        goto out;
    }
    if (*f_pos + count > dev->size)
    {
        count = dev->size - *f_pos;
    }
 
    if (!dev->data)
    {
        goto out;
    }
 
    if (raw_copy_to_user(buf, dev->data + *f_pos, count))
    {
        retval = -EFAULT;
        goto out;
    }
 
    *f_pos += count;
    retval = count;
 
    out:
        up(&dev->sem);
        return retval;
}
 
ssize_t scull_write(struct file *filp, const char __user *buf, size_t count, loff_t *f_pos)
{
    struct scull_dev *dev = filp->private_data;
    ssize_t retval = -ENOMEM;
 
    if (down_interruptible(&dev->sem))   
    {
        return -ERESTARTSYS;
    }
 
    if (!dev->data)   
    {
        dev->data = kmalloc(SCULL_BUFFER_SIZE, GFP_KERNEL);
        if (!dev->data)
        {
            goto out;
        }
        memset(dev->data, 0, SCULL_BUFFER_SIZE);
     }
 
    if (count > SCULL_BUFFER_SIZE - dev->size)
    {
        count = SCULL_BUFFER_SIZE - dev->size;
    }
 
    if (raw_copy_from_user(dev->data + dev->size, buf, count))
    {
        retval = -EFAULT;
        goto out;
    }
     
    dev->size += count;
    retval = count;
 
    out:
        up(&dev->sem);
        return retval;
}
 
loff_t scull_llseek(struct file *filp, loff_t off, int whence)
{
    struct scull_dev *dev = filp->private_data;
    loff_t newpos;
 
    switch(whence)
    {
        case 0:
            newpos = off;
            break;
        case 1:
            newpos = filp->f_pos + off;
            break;
        case 2:
            newpos = dev->size + off;
            break;
        default:
            return -EINVAL;
    }
    if (newpos < 0)
    {
        return -EINVAL;
    }
    filp->f_pos = newpos;
    return newpos;
}

int scull_ioctl(struct inode *inode, struct file *filp, unsigned int cmd, unsigned long arg)
{
if (cmd == SCULL_CMD1) {
printk("running SCULL_CMD1 \n");
return 0;
}
if (cmd == SCULL_CMD2) {
printk(" running SCULL_CMD2 \n");
return 0;
}
if (cmd == SCULL_CMD3) {
printk(" running SCULL_CMD3 \n");
return 0;
}
printk("cmd error! \n");
return -EFAULT;
}
 
struct file_operations scull_fops = {
    .owner = THIS_MODULE,
    .llseek = scull_llseek,
    .read = scull_read,
    .write = scull_write,
    .open = scull_open,
    .release = scull_release,   
};
 
void  scull_cleanup_module(void)
{
    dev_t devno = MKDEV(scull_major, scull_minor);
 
    if (scull_device)
    {
        scull_trim(scull_device);
        cdev_del(&scull_device->cdev);
        kfree(scull_device);   
    }
    unregister_chrdev_region(devno, 1);
}
 
static void scull_setup_cdev(struct scull_dev *dev)
{
    int err, devno = MKDEV(scull_major, scull_minor);
 
    cdev_init(&dev->cdev, &scull_fops);
    dev->cdev.owner = THIS_MODULE;
    dev->cdev.ops = &scull_fops;
    err = cdev_add(&dev->cdev, devno, 1);
 
    if (err)
    {
        printk(KERN_NOTICE "Error %d adding scull", err);
    }
}
 
static int __init scull_init_module(void)
{
    int result;
    dev_t dev = 0;
 
    if (scull_major)   
    {
        dev = MKDEV(scull_major, scull_minor);
        result = register_chrdev_region(dev, 1, "scull");
    }
    else
    {
        result = alloc_chrdev_region(&dev, scull_minor, 1, "scull");
        scull_major = MAJOR(dev);
    }
    if (result < 0)
    {
        printk(KERN_WARNING "scull: can't get major %d\n", scull_major);
        return result;
    }
 
    scull_device = kmalloc(sizeof(struct scull_dev), GFP_KERNEL);       
    if (!scull_device)
    {
        result = -ENOMEM;
        goto fail;
    }
    memset(scull_device, 0, sizeof(struct scull_dev));
 
    sema_init(&scull_device->sem, 1);
 
    scull_setup_cdev(scull_device);
 
    return 0;
 
    fail:
        scull_cleanup_module();
        return result;
}
 
module_init(scull_init_module);
module_exit(scull_cleanup_module);
 
MODULE_LICENSE("GPL");
