#include <linux/module.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/miscdevice.h>
#include <linux/kernel.h>
#include <linux/major.h>
#include <linux/stat.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/kmod.h>
#include <linux/platform_device.h>

#define DEV_MINOR 0X02


int my_open (struct inode *node, struct file *filp)
{
    printk("-----misc my_open-----\n");
	return 0;
}

int my_release (struct inode *node, struct file *filp)
{
    printk("-----misc my_release-----\n");
	return 0;
}

struct file_operations myfops = {
	.owner		= THIS_MODULE,
    .open       = my_open,
    .release    = my_release,
};

struct miscdevice mymisc_dev = {
    .minor            	  	= DEV_MINOR,
    .name             	  	= "my_misc_dev",
    .fops    				= &myfops,
};

static int __init  my_driver_init(void)
{
	misc_register(&mymisc_dev);
	printk("-----misc_register-----\n");
    return 0;
}

static void __exit  my_driver_exit(void)
{
	misc_deregister(&mymisc_dev);
	printk("-----misc_deregister-----\n");
}


module_init(my_driver_init);
module_exit(my_driver_exit);


MODULE_AUTHOR("ZhangQing 2017/10/25");
MODULE_LICENSE("GPL v2");

