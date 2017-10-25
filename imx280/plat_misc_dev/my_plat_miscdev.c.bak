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

#define DEV_MINOR 0X01


int my_open (struct inode *node, struct file *filp)
{
    printk("-----plat misc my_open-----\n");
	return 0;
}

int my_release (struct inode *node, struct file *filp)
{
    printk("-----plat misc my_release-----\n");
	return 0;
}

struct file_operations myfops = {
	.owner		= THIS_MODULE,
    .open       = my_open,
    .release    = my_release,
};

struct miscdevice mymisc_dev = {
    .minor            	  	= DEV_MINOR,
    .name             	  	= "my_plat_misc_dev",
    .fops    				= &myfops,
};

int my_probe(struct platform_device *dev)
{
    misc_register(&mymisc_dev);
    return 0;
}
int my_remove(struct platform_device *dev)
{
    misc_deregister(&mymisc_dev);
    return 0;
}
struct platform_driver my_misc_platdriver={
	.driver.name = "my_plat_misc",
    .probe      = my_probe,
    .remove 	= my_remove,
};

static int __init  my_driver_init(void)
{
    platform_driver_register(&my_misc_platdriver);
	printk("-----misc platform_driver_register-----\n");
    return 0;
}

static void __exit  my_driver_exit(void)
{
	platform_driver_unregister(&my_misc_platdriver);
	printk("-----misc platform_driver_unregister-----\n");
}


module_init(my_driver_init);
module_exit(my_driver_exit);


MODULE_AUTHOR("ZhangQing 2017/10/25");
MODULE_LICENSE("GPL v2");

