#include <linux/module.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <asm/uaccess.h> 


#include <asm/io.h>
#include <mach/gpio.h>
#include <mach/regs-gpio.h>



#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/types.h>


#define MY_LED_MINOR 0X01
#define MY_LED_NAME "my_led"
//#define MY_LED_GPIO EXYNOS4_GPX1(0)

#define MY_LED_GPIO EXYNOS4_GPL2(0)




//对某个设备的打开操作
static int mem_open(struct inode *inode_p, struct file *filp)
{
	int ret;
	printk("led open.\r\n");

	
	return 0;
}

//对某个设备的关闭操作
static int mem_release(struct inode *inode_p, struct file *filp)
{
	printk("led close.\r\n");

	return 0;
}


/*
//对设备进行写入数据
//参数如下：
//filp： 文件指针
//buf_towrite：将要写入的数据源
//write_length：将要写入的数据长度
//ppos：写入数据时候的偏移
//返回：成功写入的长度

static ssize_t mem_write(struct file *filp, const char __user *buf_to_write, size_t write_length, loff_t *ppos)
{

	return ret;
}

//对设备进行读取数据
//filp： 文件指针
//buf_read_to：读取数据缓冲（读出的数据将拷贝在这里）
//write_length：将要写入的数据长度
//ppos：写入数据时候的偏移

static ssize_t mem_read(struct file *filp, char __user *buf_read_to, size_t read_length, loff_t *ppos)
{
	unsigned long pos = *ppos;
	struct my_mem_device *dev = filp->private_data;
	int ret = 0;
	int count = read_length;
	printk("mem_read.\r\n");

	if(pos >= MEM_SIZE)
	{
		return count ? -ENXIO: 0;
	}
	if(count > (MEM_SIZE - pos) )
	{
		count = MEM_SIZE - pos;
	}
	
	if( copy_to_user( buf_read_to, (void *)(dev->device_mem +pos),count) ){
		return -EFAULT;	
	} else {
		ret = count;
		*ppos += count;
		printk("read %d bytes(s) from %d\n", count, pos);
	}

	return ret;
}

static loff_t mem_llseek (struct file *filp, loff_t offset, int orig)
{
	loff_t ret = 0;
	switch (orig)
	{
		case 0:
			if( offset < 0)
			{
				ret = -EINVAL;
				break;
			}
			if ((unsigned int)offset > MEM_SIZE)
		     {
		        ret =  - EINVAL;
				break;
		     }
			filp->f_pos = (unsigned int)offset;
      		ret = filp->f_pos;
			break;
			
		case 1:
			if((filp->f_pos + offset) > MEM_SIZE  )
			{
				ret =  - EINVAL;
        		break;
			}
			if ((filp->f_pos + offset) < 0)
		    {
		        ret =  - EINVAL;
		        break;
		    }
      		filp->f_pos += offset;
      		ret = filp->f_pos;
			break;

		default:
			ret =  - EINVAL;
			break;
	}
	return ret;
}




//对某个设备进行控制

static long mem_unlocked_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	struct my_mem_device *dev = filp->private_data;

	switch (cmd)
	{
	case MEM_CLEAR:
	  memset(dev->device_mem, 0, MEM_SIZE);      
	  printk("globalmem is set to zero\n");
	  break;

	default:
	  return  - EINVAL;
	}
	return 0;
}


*/















static const struct file_operations driver_ops = {
	.owner = THIS_MODULE,
	.open = mem_open,
	.release = mem_release,
};



static struct miscdevice my_led_dev = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = MY_LED_NAME,
	.fops = &driver_ops
};






//初始化该设备的驱动
//为该设备申请设备号
//将该设备注册到内核


static int __init qing_init(void)
{
	int ret;

	//申请GPIO用于控制led
	ret = gpio_request(MY_LED_GPIO, "my_led_gpio");
	if(ret == 0)
		{
			printk("gpio_request succse.\r\n");
			gpio_direction_output(MY_LED_GPIO,1);
		}
	else
		{ printk("gpio_request failed:%d\r\n",ret); }
	



	
	ret = 	misc_register(&my_led_dev);
	if(ret != 0)
 		{ printk("my_led_dev misc_register failed:%d\r\n",ret); }
	
	else
		{ printk("my_led_dev misc_register success.\r\n"); }
	return 0;
}


static void __exit qing_exit(void)
{
	int ret;
	gpio_direction_output(MY_LED_GPIO,0);
	gpio_free(MY_LED_GPIO);
	


	
	misc_deregister(&my_led_dev);
	
	if(ret != 0)
 		{ printk("my_led_dev misc_deregister failed:%d\r\n",ret); }
	
	else
		{ printk("my_led_dev misc_deregister success.\r\n"); }
}



module_init(qing_init);
module_exit(qing_exit);

MODULE_AUTHOR("Zhangqing 2017/08/05");
MODULE_LICENSE("GPL v2");






