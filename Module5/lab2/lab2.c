#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/sched.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

#define PROC_NAME "lab2"
static const int SIZE = 10;
 
static int len, temp;
static char *msg;
 
static ssize_t read_proc(struct file *filp, char *buf, size_t count, loff_t *offp ) {
    if(count > temp) {
        count = temp;
    }
    temp = temp - count;
    if (copy_to_user(buf, msg, count)){
        return -1;
    }
    if(count == 0)
        temp = len;
    return count;
}
 
static ssize_t write_proc(struct file *filp, const char *buf, size_t count, loff_t *offp) {
    if (copy_from_user(msg, buf, count)){
        return -1;
    }
    len = count;
    temp = len;
    return count;
}
 
static const struct proc_ops proc_fops = {
    proc_read: read_proc,
    proc_write: write_proc,
};
 
static void create_new_proc_entry(void) { //use of void for no arguments is compulsory now
    proc_create(PROC_NAME, 0, NULL, &proc_fops);
    msg = kmalloc(SIZE * sizeof(char), GFP_KERNEL);
}

static int proc_init (void) {
    create_new_proc_entry();
    return 0;
}
 
static void proc_cleanup(void) {
    remove_proc_entry(PROC_NAME, NULL);
    kfree(msg);
}
 
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yudin V.");
module_init(proc_init);
module_exit(proc_cleanup);