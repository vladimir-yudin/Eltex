#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h> 
#include <linux/uaccess.h>        /* определение функции copy_to_user */
#include "constants.h"

int init_module(void);
void cleanup_module(void);
static int device_open(struct inode *, struct file *);
static int device_release(struct inode *, struct file *);
static ssize_t device_read(struct file *, char __user*, size_t , loff_t *);
static ssize_t device_write(struct file *, const char __user*, size_t , loff_t *);

MODULE_DESCRIPTION("Example module illustrating the use of chardev.");
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yudin V.");

static int Major;               /* Старший номер устройства нашего драйвера */
static int Device_Open = 0;     /* Устройство открыто? */
static char msg[BUF_LEN];       /* Здесь будет собираться текст сообщения */
static struct class *cls; 

static struct file_operations fops = {
    .read = device_read,
    .write = device_write,
    .open = device_open,
    .release = device_release
};

static int device_open(struct inode *inode, struct file *file){
    if (Device_Open)
        return -EBUSY;
    Device_Open++;
    static int counter = 0;
    printk(KERN_INFO "File opened for %d times\n", counter++);
    try_module_get(THIS_MODULE);

    return SUCCESS;
}
static int device_release(struct inode *inode, struct file *file){
    Device_Open--;
    module_put(THIS_MODULE);

    return SUCCESS;
}
static ssize_t device_read(struct file *filp, char *buffer, size_t length, loff_t *offset){
    if (copy_to_user(buffer, msg, length) == BUF_LEN) {
        return -EFAULT;
    }
    return length;
}
static ssize_t device_write(struct file *filp, const char *buffer, size_t length, loff_t *offset){
    if (copy_from_user(msg, buffer, length)) {
        return -EFAULT;
    }
    return length;
}

int init_module(void){
    Major = register_chrdev(0, DEVICE_NAME, &fops);

    if (Major < 0) {    
        printk(KERN_INFO "Registering the character device failed with %d\n", Major);
        return Major;
    }
    cls = class_create(DEVICE_NAME); 
    device_create(cls, NULL, MKDEV(Major, 0), NULL, DEVICE_NAME); 
    pr_info("Device created on /dev/%s\n", DEVICE_NAME); 
    return SUCCESS;
}

void cleanup_module(void){
    unregister_chrdev(Major, DEVICE_NAME);
    device_destroy(cls, MKDEV(Major, 0)); 
    class_destroy(cls); 
}