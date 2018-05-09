#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/moduleparam.h>
#include <linux/slab.h>
#include <linux/ioctl.h>
#include <linux/cdev.h>
#include <linux/delay.h>

#include <linux/gpio.h>
#include <mach/gpio.h>
#include <plat/gpio-cfg.h>
#include <linux/device.h>
#define DEVICE_NAME "leds"

static int led_gpios[] = {
        EXYNOS4212_GPM4(0),
        EXYNOS4212_GPM4(1),
        EXYNOS4212_GPM4(2),
        EXYNOS4212_GPM4(3),
};

#define LED_NUM         ARRAY_SIZE(led_gpios)
int leds_open (struct inode * n, struct file * flip)
{
	int ret;
	int i;

	for (i = 0; i < LED_NUM; i++) {
		ret = gpio_request(led_gpios[i], "LED");
		if (ret) {
			printk("%s: request GPIO %d for LED failed, ret = %d\n", DEVICE_NAME,
					led_gpios[i], ret);
			return ret;
		}

		s3c_gpio_cfgpin(led_gpios[i], S3C_GPIO_OUTPUT);
		gpio_set_value(led_gpios[i], 1);
	}
	return ret;
}
static long leds_ioctl(struct file *filp, unsigned int cmd,
                unsigned long arg)
{
        switch(cmd) {
                case 0:
                case 1:
                        if (arg > LED_NUM) {
                                return -EINVAL;
                        }

                        gpio_set_value(led_gpios[arg], !cmd);
                        printk(DEVICE_NAME": led:%d level:%d\n", arg, cmd);
                        break;

                default:
                        return -EINVAL;
        }

        return 0;
}

static struct file_operations le_fops = {
        .owner                  = THIS_MODULE,
	.open			= leds_open,
        .unlocked_ioctl 	= leds_ioctl,
};
static int major;
static struct class *cls;
static int leds_init(void)
{
	major = register_chrdev(0, "leds", &le_fops);
	cls = class_create(THIS_MODULE, "leds");
	device_create(cls, NULL, MKDEV(major, 0), NULL, "leds");
	return 0;
}
static void leds_exit(void)
{
	device_destroy(cls, MKDEV(major, 0));
	class_destroy(cls);
	unregister_chrdev(major, "leds");
}	
module_init(leds_init);
module_exit(leds_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("carrykun");
