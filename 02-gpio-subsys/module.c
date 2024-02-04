#include <linux/module.h>   /* This module defines functions such as module_init/module_exit */
#include <linux/fs.h>       /*allocate major va minor number*/
#include <linux/device.h>   /* Define device_create(), class_create() */
#include <linux/cdev.h>     /* Define cdev_init(), cdev_add() */
#include <linux/io.h>       /* This module defines functions such as ioremap/iounmap */
#include <linux/gpio.h>     /* Defines functions such as gpio_request/gpio_free */

#define DRIVER_MODULE_DESC "GPIO Subsystem IOCTL"
#define DRIVER_AUTHOR "BachNX4"

#define LED 31

#define ON_CMD _IOW('b','1', uint8_t*)
#define OFF_CMD _IOW('b','0', uint8_t*)

struct mdev_t {
    dev_t dev_num;
    struct class *dev_class;
    struct cdev cdev;
}mdev;

static long m_ioctl(struct file *f, unsigned int cmd, unsigned long arg);

struct file_operations fops = {
    .owner = THIS_MODULE,
    .unlocked_ioctl = m_ioctl,
};

static long m_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
{
    switch(cmd) {
        case ON_CMD:
            gpio_set_value(LED, 1);
            pr_info("Turn ON LED.\n");
            break;
        case OFF_CMD:
            gpio_set_value(LED, 0);
            pr_info("Turn OFF LED.\n");
            break;
        default:
            break;
    }

    return 0;
}

static int __init gpio_subsys_init(void)
{
    int ret;

    pr_info("GPIO Subsystem IOCTL Module.\n");

    ret = gpio_request(LED, "LED");
    if(ret < 0) {
        pr_err("Request GPIO failed! Error: %d\n", ret);
        return -1;
    }

    ret = gpio_direction_output(LED, 0);
    if(ret < 0) {
        pr_err("Set GPIO direction failed! Error: %d\n", ret);
        return -1;
    } 

    ret = alloc_chrdev_region(&mdev.dev_num, 0, 1, "mdev");
    if(ret < 0) {
        pr_err("Failed to alloc device number. Error: %d\n", ret);
        return -1;
    }

    pr_info("MAJOR: %d - MINOR: %d\n", MAJOR(mdev.dev_num), MINOR(mdev.dev_num));
    cdev_init(&mdev.cdev, &fops);

    ret = cdev_add(&mdev.cdev, mdev.dev_num, 1);
    if(ret < 0) {
        pr_err("Failed to init add character device to the system. Error: %d\n", ret);
        return -1; 
    }

    if((mdev.dev_class = class_create(THIS_MODULE, "mclass")) == NULL) {
        pr_err("Failed to create class device. Error: %d\n", ret);
        goto rm_device_num;
    }

    if((device_create(mdev.dev_class, NULL, mdev.dev_num, NULL, "m-dev")) == NULL) {
        pr_err("Failed to create device file. Error: %d\n", ret);
       goto rm_class;
    }
    
    return 0;
rm_class:
    class_destroy(mdev.dev_class);
rm_device_num:
    unregister_chrdev_region(mdev.dev_num, 1);
    return -1;
}

static void __exit gpio_subsys_exit(void)
{
    gpio_free(LED);

    device_destroy(mdev.dev_class, mdev.dev_num);
    class_destroy(mdev.dev_class);
    unregister_chrdev_region(mdev.dev_num, 1);

    pr_info("GPIO Subsystem IOCTL Exit.\n");
}

module_init(gpio_subsys_init);
module_exit(gpio_subsys_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_MODULE_DESC);
MODULE_VERSION("v1.0.0");

