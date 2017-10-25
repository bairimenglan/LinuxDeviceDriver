#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
 .arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x405fe887, "module_layout" },
	{ 0x8a63f48d, "led_classdev_register" },
	{ 0x47229b5c, "gpio_request" },
	{ 0xea147363, "printk" },
	{ 0xbb27caaf, "led_classdev_unregister" },
	{ 0xfe990052, "gpio_free" },
	{ 0xa8f59416, "gpio_direction_output" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

