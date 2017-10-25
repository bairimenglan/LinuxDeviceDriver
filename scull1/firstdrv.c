#include <linux/module.h>
#include <linux/init.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <asm/uaccess.h> 
#include <asm/io.h>

#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/types.h>




#define MEM_SIZE 0x1000
#define MEM_CLEAR 0x1
#define MEM_DEV_NUM 6





//定义一个设备结构体
struct my_mem_device{
	struct cdev char_device;
	unsigned char device_mem[MEM_SIZE];
};



//定义设备号，以及一个设备指针
dev_t device_num = 0;
struct my_mem_device my_device_p[MEM_DEV_NUM];

	






//对某个设备的打开操作
static int mem_open(struct inode *inode_p, struct file *filp)
{
	printk("mem_open.\r\n");

	struct my_mem_device *dev;
	dev = container_of(inode_p->i_cdev, struct my_mem_device, char_device);

	//将my_device_p存放到file结构体的private_data中，需要时可由private_data取出
	filp->private_data = dev;
	return 0;
}

//对某个设备的关闭操作
static int mem_release(struct inode *inode_p, struct file *filp)
{
	printk("mem_release.\r\n");

	return 0;
}

//对设备进行写入数据
//参数如下：
//filp： 文件指针
//buf_towrite：将要写入的数据源
//write_length：将要写入的数据长度
//ppos：写入数据时候的偏移
//返回：成功写入的长度

static ssize_t mem_write(struct file *filp, const char __user *buf_to_write, size_t write_length, loff_t *ppos)
{
	unsigned long pos = *ppos;
	struct my_mem_device *dev = filp->private_data;
	int ret = 0;
	int count = write_length;
	printk("mem_write.\r\n");

	if(pos >= MEM_SIZE)
	{
		return count ? -ENXIO:0 ;
	}
	if(count > MEM_SIZE-pos)
	{
		count = MEM_SIZE-pos;
	}
	
	if( copy_from_user((dev->device_mem)+pos, buf_to_write,     count) ){
		return -EFAULT;	
	} else {
		ret = count;
		*ppos += count;
		printk("written %d bytes(s) from %d\n", count, pos);
	}

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

//对某个设备进行控制

static long mem_unlocked_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	struct my_mem_device *dev = filp->private_data;/*获得设备结构体指针*/

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















static const struct file_operations driver_ops = {
	.owner = THIS_MODULE,
	.llseek = mem_llseek,
	.open = mem_open,
	.release = mem_release,
	.read = mem_read,
	.write = mem_write,
	.unlocked_ioctl = mem_unlocked_ioctl
};








//初始化该设备的驱动
//为该设备申请设备号
//将该设备注册到内核


static int __init qing_init(void)
{
	//申请内存和注册设备号的状态
	int ret = 0;
	int err = 0;
	
	
	//定义设备驱动操作函数结构体


	//为设备申请内核资源
	my_device_p =	kmalloc(sizeof(struct my_mem_device)*MEM_DEV_NUM, GFP_KERNEL);
	if(!my_device_p)//申请内核资源失败
	{
		ret = -ENOMEM;
		printk("1.kmalloc for my_device_p failed.\r\n");
		return ret;
	}
	printk("kmalloc success.\r\n");
	memset(my_device_p, 0, sizeof(struct my_mem_device)*MEM_DEV_NUM);


	//动态申请设备号 MEM_DEV_NUM个设备
	ret = alloc_chrdev_region(&device_num, 0, MEM_DEV_NUM, "zhangqing_mem_device");
	if(ret < 0)//申请设备号失败
	{
		printk("2.register_chrdev_region failed.\r\n");
		return ret;
	}
	printk("register_chrdev_region success.\r\n");
	
	//绑定cdev和f_ops

	for(i=0;i<MEM_DEV_NUM;i++)
		{
			cdev_init( &((my_device_p+i)->char_device), &driver_ops);
		}
	
	printk("cdev_init success.\r\n");

	
	//注册设备
	err = cdev_add( &(my_device_p->char_device), device_num, MEM_DEV_NUM);
	if (err)//注册设备失败
	{
		printk("3.cdev_add fail: %d\r\n",err);
		unregister_chrdev_region(device_num, 1);
	}
	printk("cdev_add success.\r\n");
	
	
	printk("zhangqing mem driver init success.\r\n");
	
	return 0;
	
}


static void __exit qing_exit(void)
{
	int i;

	//cdev_del(&(my_device_p->char_device));

	for(i=0;i<MEM_DEV_NUM;i++)
		{
			cdev_del(&((my_device_p+i)->char_device));
		}
	
	kfree(my_device_p);
	unregister_chrdev_region(device_num, MEM_DEV_NUM);
	printk("unregister_chrdev_region success.\r\n");
	
	printk("zhangqing driver exit success.\r\n");
}



module_init(qing_init);
module_exit(qing_exit);

MODULE_AUTHOR("Zhangqing 2017/07/19");
MODULE_LICENSE("GPL v2");






