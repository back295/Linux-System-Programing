#include <linux/module.h>
#include <linux/platform_device.h> /* For platform device */
#include <linux/gpio/consumer.h> /* For GPIO Descriptor */
#include <linux/of.h> /* For DT */

#define DRIVER_AUTHOR "BachNX FSOFT"
#define DRIVER_DESC "LED blinky using basic devicetree"

static struct gpio_desc *led;

static const struct of_device_id gpiod_dt_ids[] = {
    { .compatible = "back,mled", }, 
    { /*nothing*/}
};

static int led_probe(struct platform_device *pdev)
{
    struct device *dev = &pdev->dev;

    pr_info("LED GPIO Descriptor module initialized successfully!\n");
    led = gpiod_get(dev, "mled", GPIOD_OUT_HIGH);
    gpiod_set_value(led, 1);


    return 0;
}

static int led_remove(struct platform_device *pdev)
{
    pr_info("LED GPIO Descriptor module exit.\n");
    gpiod_put(led);

    return 0;
}

static struct platform_driver mled_driver = {
    .probe = led_probe,
    .remove = led_remove,
    .driver = {
        .name = "back, mled",
        .of_match_table = of_match_ptr(gpiod_dt_ids),
        .owner = THIS_MODULE,
    },
};

module_platform_driver(mled_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_VERSION("v1.0.0");