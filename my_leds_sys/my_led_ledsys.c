#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/leds.h>
#include <linux/gpio.h>
#include <linux/slab.h>
#include <linux/module.h>  

#include <mach/hardware.h>
#include <mach/regs-gpio.h>



#define MY_LED_GPIO EXYNOS4_GPL2(0)

static void my_led_set(struct led_classdev *led_cdev, enum led_brightness brightness)
{
	gpio_direction_output(MY_LED_GPIO, brightness);
}

static struct led_classdev my_led_dev = {
	.name = "my_led_sys",
	.brightness_set = my_led_set,
	.default_trigger = "none",
};

static int __init led_init(void)
{
	int ret;
	ret = gpio_request(MY_LED_GPIO, "my_led_gpio");
	if(ret == 0)
		{
			printk("gpio_request succse.\r\n");
			gpio_direction_output(MY_LED_GPIO, 1);
		}
	else
		{ printk("gpio_request failed:%d\r\n",ret); }

	ret = led_classdev_register(NULL, &my_led_dev);
	if (ret == 0) 
		{ printk( "led_classdev_register success.\n"); }
	
	return ret;

}


static void __exit led_exit(void)
{
	gpio_direction_output(MY_LED_GPIO, 0);
	gpio_free(MY_LED_GPIO);

	led_classdev_unregister(&my_led_dev);
	
	printk("led_classdev_unregister \r\n");

}



module_init(led_init);
module_exit(led_exit);

MODULE_AUTHOR("Zhangqing 2017/08/05");
MODULE_LICENSE("GPL v2");


