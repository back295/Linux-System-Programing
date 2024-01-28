#include <linux/module.h> /*module_init va module_exit*/
#include <linux/fs.h> /*allocate major va minor number*/
#include <linux/device.h>   /* Define device_create(), class_create() */
#include <linux/cdev.h>     /* Define cdev_init(), cdev_add() */

#define DRIVER_AUTHOR "BachNX"
#define DRIVER_DESC "Hello kernel module"

#define LED_ON _IOW('b' , '1', uint8_t*)
#define LED_OFF _IOW('b', '0', uint8_t*)

struct mdev_t
{
    dev_t mdev_num;
    struct class *m_class;
    struct cdev m_cdev;
}mdev;

/* Prototype */
// static ssize_t m_read(struct file * file, char *, size_t size, loff_t *off_set);
// static ssize_t m_write(struct file * file, const char *, size_t size, loff_t *off_set);
// static int m_open(struct inode *inode, struct file * file);
// static int m_close(struct inode *inode, struct file * file);
static long m_ioctl(struct file *f, unsigned int cmd, unsigned long arg);
struct file_operations fops = {
        .owner = THIS_MODULE,
    //    .read = m_read,
    //    .write = m_write,
    //    .open = m_open,
    //    .release = m_close,
        .unlocked_ioctl = m_ioctl,
};

static long m_ioctl(struct file *f, unsigned int cmd, unsigned long arg)
{
    switch(cmd) {
        case LED_ON:
            pr_info("Turn LED ON.\n");
            break;
        case LED_OFF:
            pr_info("Turn LED OFF.\n");
            break;
        default:
            break;
    }

    return 0;
}
// static ssize_t m_read(struct file * file, char *, size_t size, loff_t *off_set)
// {
//     pr_info("System call read().\n");
//     return size;
// }
// static ssize_t m_write(struct file * file, const char *, size_t size, loff_t *off_set)
// {
//     pr_info("System call write().\n");
//     return size;
// }
// static int m_open(struct inode *inode, struct file * file)
// {
//     pr_info("System call open().\n");
//     return 0;
// }
// static int m_close(struct inode *inode, struct file * file)
// {
//     pr_info("System call close().\n");
//     return 0;
// }


static int __init hello_init(void)
{
    pr_info("Init Hello World kernel module.\n");

    /* 1.0 Dynamic allocating device number (cat /proc/devices) */
    if(alloc_chrdev_region(&mdev.mdev_num, 0 , 1,"m-dev") <0) {
        pr_err("Failed to allocate device number!\n");
        return -1;
    }

    pr_info("MAJOR: %d - MINOR: %d\n",MAJOR(mdev.mdev_num), MINOR(mdev.mdev_num));

    /*Initializes cdev, remembering fops, 
    making it ready to add to the system with cdev_add.*/
    cdev_init(&mdev.m_cdev, &fops);

    /*adds the device to the system, making it live immediately. */
    if(cdev_add(&mdev.m_cdev, mdev.mdev_num, 1) < 0) {
        pr_err("Failed to add mdev to system.\n");
        return -1;
    }

    /* 2.0 Creating struct class */
    if ((mdev.m_class = class_create("m_class")) == NULL) {
        pr_err("Cannot create the struct class for my device\n");
        goto rm_device_num;
    }

    /* 3.0 Creating device*/
    if((device_create(mdev.m_class, NULL, mdev.mdev_num, NULL, "m-dev") == NULL)) {
        pr_err("Failed to create device!\n");
        goto rm_class;
    }
    return 0;

rm_class:
    class_destroy(mdev.m_class);
rm_device_num:
    unregister_chrdev_region(mdev.mdev_num, 1);
    return -1;
}

static void __exit hello_exit(void)
{
    device_destroy(mdev.m_class, mdev.mdev_num);
    class_destroy(mdev.m_class);
    unregister_chrdev_region(mdev.mdev_num, 1);
    pr_info("Exit module.\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_VERSION("v1.0.0");