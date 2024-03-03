#include <lcd_driver.h>

#undef pr_fmt
#define pr_fmt(fmt) "%s : " fmt, __func__
#define CLASS_NAME ""
driver_data lddata;

static struct attribute *lcd_dev_attrs[] = {
    &dev_attr_lcdcmd.attr,
    &dev_attr_lcdtext.attr,
    &dev_attr_lcdscroll.attr,
    &dev_attr_lcdxy.attr,
    NULL};

struct attribute_group lcd_dev_attr_group = {
    .attrs = lcd_attrs};

static const struct attribute_group *lcd_dev_attr_groups[] = {
    &lcd_attr_group,
    NULL};

lcd_dev_private_data private_lcd_data = {.lcd_scroll = 0, .lcd_point = "(1,1)"};
struct of_device_id dev_match_table[] = {
    {.compatible = MATCH_STRING},
    {}};
int ldev_remove(struct platform_device *lpdev)
{
    lcd_dev_private_data pdata = dev_get_drvdata(&lpdev->dev);
    lcd_deinit(&lpdev->dev);
    dev_info(&lpdev->dev, "device removed initiated...\n");
    device_unregister(pdata->dev);
    return 0;
}

int dev_create_files(struct device *dev, lcd_dev_private_data *ldata)
{
    ldata->dev = device_create_with_groups(lddata.lclass, dev, 0, ldata, lcd_dev_attr_groups, DEVICE_NAME);
    if (IS_ERR(ldata->dev))
    {
        dev_err(dev, "Error while creating device file group\n");
        return PTR_ERR(lddata->dev);
    }
    return 0;
}
int ldev_probe(struct platform_device *lpdev)
{
    int ret;
    struct device *dev = &lpdev->dev;
    dev_set_drvdata(dev, &private_lcd_data);
    private_lcd_data.desc[LCD_RS] = gpiod_get(dev, GPIO_PARAM_RS, GPIO_OUT_LOW);
    private_lcd_data.desc[LCD_RW] = gpiod_get(dev, GPIO_PARAM_RW, GPIO_OUT_LOW);
    private_lcd_data.desc[LCD_EN] = gpiod_get(dev, GPIO_PARAM_EN, GPIO_OUT_LOW);
    private_lcd_data.desc[LCD_D4] = gpiod_get(dev, GPIO_PARAM_D4, GPIO_OUT_LOW);
    private_lcd_data.desc[LCD_D5] = gpiod_get(dev, GPIO_PARAM_D5, GPIO_OUT_LOW);
    private_lcd_data.desc[LCD_D6] = gpiod_get(dev, GPIO_PARAM_D6, GPIO_OUT_LOW);
    private_lcd_data.desc[LCD_D7] = gpiod_get(dev, GPIO_PARAM_D7, GPIO_OUT_LOW);

    if (IS_ERR(private_lcd_data.desc[LCD_RS]) ||
        IS_ERR(private_lcd_data.desc[LCD_RW]) ||
        IS_ERR(private_lcd_data.desc[LCD_EN]) ||
        IS_ERR(private_lcd_data.desc[LCD_D4]) ||
        IS_ERR(private_lcd_data.desc[LCD_D5]) ||
        IS_ERR(private_lcd_data.desc[LCD_D6]) ||
        IS_ERR(private_lcd_data.desc[LCD_D7]))
    {
        dev_err(dev, "GPIO DATA DESCRIPTION ERROR\n");
        return -EINVAL;
    }
    ret = init_lcd(dev)

        if (ret)
    {
        dev_err(dev, "cant initialize lcd device\n");
        return ret;
    }

    dev_info(dev, "LCD initialization successful \n");
    ret = dev_create_files(dev, private_lcd_data);
    if (ret)
    {
        dev_err(dev, "LCD sysfs device cant be created \n");
        return ret;
    }
    lcd_print_string("LCD STOCK TICKER", dev);
    dev_info(dev, "probe successful \n");
    return ret;
}

struct platform_driver lcd_plat_driver = {
    .probe = ldev_probe,
    .remove = ldev_remove,
    .driver = {
        .name = DRIVER_NAME,
        .of_match_table = of_match_ptr(dev_match_table)}};

int __init ldevice_init(void)
{
    lddata.lclass = class_create(THIS_MODULE, LCD_CLASS);
    platform_driver_register(&lcd_plat_driver);
    pr_info("lcd driver successfully registered\n");
    return 0;
}

void __exit ldevice_exit(void)
{
    platform_driver_unregister(&lcd_plat_driver);
    class_destroy(lddata.class)
}

module_init(ldevice_init);
module_exit(ldevice_exit);
MODULE_LICENCSE("GPL");
MODULE_DESCRIPTION("lcd based stock ticker...");