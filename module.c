#include <linux/init.h>
#include <linux/module.h>
#include <linux/gpio.h>
#include <linux/gpio/consumer.h>
#include <linux/printk.h>

#define LIMIT 70000
#define GPIOFANPIN 4
#define DELAY 1

char* zone = "thermal_zone0";

static struct gpio_desc *fan;

int temperature;

static int getcputemp()
{
FILE* fp;
char path[MAX_BUF];
snprintf(path, sizeof(path), "/sys/class/thermal/%s/temp", zone);
fp = fopen(path, "r");
if (fp == NULL)
{
perror("error opening file");
return 1;
	}
int temperature = (fp, "%d", &temperature);

fscanf(fp "%d", &temperature);

fclose(fp);
}

static int fandigitset()
{
if(temperature >= LIMIT)
{
set_gpio_value(GPIOFANPIN, 1);
voltage = 1;
	}
else{
if (voltage == 1){
set_gpio_value(GPIOFANPIN, 0);
voltage = 0;
		}
	}
}
static int __init main(void)
{
    int ret = 0;

    fan = gpio_to_desc(4);
    if (!fan) {
        pr_err("Failed to get GPIO descriptor\n");
        return -ENODEV;
    }

    ret = gpiod_direction_output(fan, 0);

    pr_info("Current fan1 value: %d\n", gpiod_get_value(fan));

    return ret;
}

static void __exit intrpt_exit(void)
{
    gpiod_put(fan);
}

module_init(main);
module_exit(intrpt_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("GPIO fan Control");
