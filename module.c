#include <linux/init.h>                                                                        
#include <linux/module.h>                                                                      
#include <linux/gpio.h>                                                                        
#include <linux/gpio/consumer.h>                                                               
#include <linux/printk.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/fcntl.h>
#include <linux/delay.h> 

#define MAX_BUF 64
#define LIMIT 70000                                                                            
#define GPIOFANPIN 4                                                                           
#define DELAY 1                                                                                

char* zone = "thermal_zone0";                                                                  
static struct gpio_desc *fan;                                                                  
int voltage;
float temperature = 0;

static int getcputemp(void)                                                                    
{
    struct file *fp;
    char path[MAX_BUF];
    int voltage;
    ssize_t temperature;

    snprintf(path, sizeof(path), "/sys/class/thermal/%s/temp", zone);
    
    fp = filp_open(path, O_RDONLY, 0);
    if (IS_ERR(fp))
    {
        pr_err("error opening file: %ld\n", PTR_ERR(fp));
        return 1;
    }

    temperature = kernel_read(fp, &temperature, sizeof(int), &fp->f_pos);

    filp_close(fp, NULL);

    return temperature;

    if (temperature >= LIMIT)
    {
        gpiod_set_value(fan, 1);
        voltage = 1;
    }
    else
    {
        if (voltage == 1)
        {
            gpiod_set_value(fan, 0);
            voltage = 0;
        }
    }

    return 0;
}
static int __init main_init(void)
{
    int ret = 0;

    fan = gpio_to_desc(GPIOFANPIN);
    if (!fan) {
        pr_err("Failed to get GPIO descriptor\n");
        return -ENODEV;
    }

    ret = gpiod_direction_output(fan, 0);

    pr_info("Current fan1 value: %d\n", gpiod_get_value(fan));
	
	while(1){
	getcputemp();
	msleep(5000);
	}
}

static void __exit main_exit(void)
{
    gpiod_put(fan);
}

module_init(main_init);
module_exit(main_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("GPIO fan Control");
