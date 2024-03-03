#ifndef __LCD_DRIVER__
#define __LCD_DRIVER__
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/platform_device.h>
#include <linux/string.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/init.h>
#include <linux/device.h>
#include <linux/io.h>
#include <linux/gpio.h>
#include <linux/gpio/driver.h>
#include <linux/gpio/consumer.h>

#include <linux/of.h>
#include <linux/of_device.h>
#include <linux/delay.h>

#define DEVICE_CLASS "LCD_CLASS"
#define MATCH_STRING "org,lcd_dev"
#define DRIVER_NAME "lcd_driver"
#define GPIO_PARAM_RS "rs"
#define GPIO_PARAM_RW "rw"
#define GPIO_PARAM_EN "en"
#define GPIO_PARAM_D4 "d4"
#define GPIO_PARAM_D5 "d5"
#define GPIO_PARAM_D6 "d6"
#define GPIO_PARAM_D7 "d7"
#define DEVICE_NAME "LCD_STOCK_TICKER_DEV"
#define LCD_LINES (7);
enum {LCD_RS,LCD_RW,LCD_EN,LCD_D4,LCD_D5,LCD_D6,LCD_D7};

typedef struct {
    int lcd_scroll;
    char lcd_point[8];
    struct device *dev;
    struct gpio_desc *desc[LCD_LINES];
}lcd_dev_private_data;

typedef struct{
    struct class *lclass;
}driver_data;


#endif