#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Yudin V.");
MODULE_DESCRIPTION("Module5 Lab1");

static int hello_init(void)
{
    printk(KERN_INFO "Hello world! How are you?\n");
    return 0;
}

static void hello_cleanup(void)
{
    printk(KERN_INFO "Cleaning up module.\n");
}

module_init(hello_init);
module_exit(hello_cleanup);