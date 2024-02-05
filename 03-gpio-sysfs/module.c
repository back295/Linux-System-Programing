#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>   /* Define device_create(), class_create() */
#include <linux/cdev.h>     /* Define cdev_init(), cdev_add() */
#include <linux/io.h>       /* This module defines functions such as ioremap/iounmap */
#include <linux/gpio.h>     /* Defines functions such as gpio_request/gpio_free */
#include <linux/kobject.h>
#include <linux/sysfs.h>
#include <linux/string.h>

#define DRIVER_AUTHOR "BachNX"
#define DRIVER_DESC "GPIO sysfs module"
#define LED 31

static int gpio_value = 0;
static int gpio_index = 0;
static char gpio_direction[5];

static struct {
    /* moi file duoi /sys la mot kernel object, tao kernel object moi*/
    struct kobject *m_kobj; 
}mdev;

/* show function tuong ung voi viec doc file, cat file*/
static ssize_t value_show(struct kobject *kobj, struct kobj_attribute *attr,
			char *buf);

/* store function ung voi viec ghi file, echo vao file */
static ssize_t value_store(struct kobject *kobj, struct kobj_attribute *attr,
			 const char *buf, size_t count);

static ssize_t direction_show(struct kobject *kobj, struct kobj_attribute *attr,
			char *buf);

static ssize_t direction_store(struct kobject *kobj, struct kobj_attribute *attr,
			 const char *buf, size_t count);

static ssize_t export_store(struct kobject *kobj, struct kobj_attribute *attr,
			 const char *buf, size_t count);

static ssize_t unexport_store(struct kobject *kobj, struct kobj_attribute *attr,
			 const char *buf, size_t count);

/* Khai bao cac attribute ma object support */
static struct kobj_attribute value = __ATTR(value, 0660, value_show, value_store);
static struct kobj_attribute direction = __ATTR(direction, 0660, direction_show, direction_store);
static struct kobj_attribute export = __ATTR(export, 0660, NULL, export_store);
static struct kobj_attribute unexport = __ATTR(unexport, 0660, NULL, unexport_store);

/* Viec tao ra nhieu attribute thi nen nhom chung thanh group roi add the group*/
static struct attribute *m_attr[] = {&value.attr, 
                                     &direction.attr,
                                     &export.attr,
                                     &unexport.attr,
                                     NULL};

static struct attribute_group attr_grp = {
    .attrs = m_attr
};

static ssize_t value_show(struct kobject *kobj, struct kobj_attribute *attr,
			char *buf)
{
    /* data show len userspace se duoc ghi vao buf*/
    return sprintf(buf, "%d", gpio_value);
}
static ssize_t value_store(struct kobject *kobj, struct kobj_attribute *attr,
			 const char *buf, size_t count)
{
    /*gan gia tri buf vao gpio_value*/
    sscanf(buf, "%d", &gpio_value);

    switch(gpio_value)
    {
        case 0:
            gpio_set_value(gpio_index, 0);
            pr_info("Turn OFF LED.\n");
            break;
        case 1:
            gpio_set_value(gpio_index, 1);
            pr_info("Turn ON LED.\n");
            break;
        default:
            break;
    }

    return count;
}
static ssize_t direction_show(struct kobject *kobj, struct kobj_attribute *attr,
			char *buf)
{
    return sprintf(buf, "%s", gpio_direction);
}
static ssize_t direction_store(struct kobject *kobj, struct kobj_attribute *attr,
			 const char *buf, size_t count)
{
    switch(count - 1) {
        case 3:
            gpio_direction_output(gpio_index, 0);
            pr_info("Set GPIO direction: OUTPUT.\n");
            break;
        case 2 :
            gpio_direction_input(gpio_index);
            pr_info("Set GPIO direction: INPUT.\n");
            break;
        default:
            break;
    }

    strncpy(gpio_direction, buf, count);

    return count;
}

static ssize_t export_store(struct kobject *kobj, struct kobj_attribute *attr,
			 const char *buf, size_t count)
{
    sscanf(buf, "%d", &gpio_index);

    gpio_request(gpio_index, "LED");

    return count;
}    

static ssize_t unexport_store(struct kobject *kobj, struct kobj_attribute *attr,
			 const char *buf, size_t count)
{
    sscanf(buf, "%d", &gpio_index);
    gpio_free(gpio_index);

    return count;
}    

static int __init gpio_sysfs_init(void)
{
    /* Tao ra object file la add vao system */
    mdev.m_kobj = kobject_create_and_add("bbb_gpio", NULL);

    /* Add cac attribute theo group */
    if(sysfs_create_group(mdev.m_kobj, &attr_grp)) {
        pr_err("Failed to create group!\n");
        goto rm_kobj;
    }

    pr_info("Initialize GPIO sysfs module successfully!\n");

    return 0;

rm_kobj:
    /* remove object file */
    kobject_put(mdev.m_kobj);
    return -1;
}

static void __exit gpio_sysfs_exit(void)
{
    gpio_free(gpio_index);
    /* 02. remove cac attribute file truoc */
    sysfs_remove_group(mdev.m_kobj, &attr_grp);
    /* 01. remove object file sau*/
    kobject_put(mdev.m_kobj);

    pr_info("GPIO Sysfs Module Exit.\n");
}

module_init(gpio_sysfs_init);
module_exit(gpio_sysfs_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_VERSION("v1.0.0");
