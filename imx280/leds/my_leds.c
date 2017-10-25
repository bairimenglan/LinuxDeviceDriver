/**
* 该模块用于创建两个LED类设备，在sys/class下直接操作led设备
*
*/
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/leds.h>
#include <linux/slab.h>
#include <linux/gpio.h>
#include <linux/module.h>

#include <../arch/arm/mach-mx28/mx28_pins.h>

#define MY_GPIO_LED1 MXS_PIN_TO_GPIO(PINID_SAIF0_MCLK)
#define MY_GPIO_LED2 MXS_PIN_TO_GPIO(PINID_SSP0_DATA7)

#define MY_LED_NAME1 "MY_LED1"
#define MY_LED_NAME2 "MY_LED2"

void my_brightness_set(struct led_classdev *led_cdev, enum led_brightness brightness)
{
/*	if(led_cdev)
	{ gpio_direction_output(MY_GPIO_LED1, brightness);}
	else
 	{ gpio_direction_output(MY_GPIO_LED2, brightness);}*/

	gpio_direction_output(MY_GPIO_LED1, brightness);
}


struct led_classdev my_leddev1 = {
	.name = "my_led_1",
	.brightness_set = my_brightness_set,
	.default_trigger = "heartbeat",
};
struct led_classdev my_leddev2 = {
	.name = "my_led_2",
	.brightness_set = my_brightness_set,
	.default_trigger = "none",
};

static int __init leds_init(void)
{
        int ret;
        ret = gpio_request(MY_GPIO_LED1, "LED1");
        if(ret == 0)
        {
                printk("LED1 gpio_request succse.\r\n");
                gpio_direction_output(MY_GPIO_LED1, 1);
        }
        else
		{ printk("gpio_request LED1 failed:%d\r\n",ret); }
		ret = gpio_request(MY_GPIO_LED2, "LED2");
        if(ret == 0)
        {
                printk("LED2 gpio_request succse.\r\n");
                gpio_direction_output(MY_GPIO_LED2, 1);
        }
        else
		{ printk("gpio_request LED2 failed:%d\r\n",ret); }

		
        ret = led_classdev_register(NULL, &my_leddev1);
        if (ret == 0) 
                { printk( "led_classdev_register MY_LEDDEV1 success.\n"); }
		ret = led_classdev_register(NULL, &my_leddev2);
        if (ret == 0) 
                { printk( "led_classdev_register MY_LEDDEV2 success.\n"); }
        return ret;
}


static void __exit leds_exit(void)
{
	gpio_direction_output(MY_GPIO_LED1, 0);
	gpio_free(MY_GPIO_LED1);
	gpio_direction_output(MY_GPIO_LED2, 0);
	gpio_free(MY_GPIO_LED2);

	led_classdev_unregister(&my_leddev1);
	led_classdev_unregister(&my_leddev2);
	printk("led_classdev_unregister \r\n");
}

module_init(leds_init);
module_exit(leds_exit);


MODULE_AUTHOR("ZhangQing 15/10/2017");
MODULE_LICENSE("GPL v2");

