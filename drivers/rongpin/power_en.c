#include <linux/module.h>
#include <linux/err.h>
#include <linux/gpio.h>
#include <linux/kernel.h>
#include <linux/of_gpio.h>
#include <linux/of_platform.h>
#include <linux/delay.h>
// #include <linux/wakelock.h>
#include <linux/of.h>
#include <linux/of_fdt.h>
#include <linux/proc_fs.h>
struct gpio_gpio {
	int gpio_num;
	int val;
};

struct power_en_gpio {
	struct gpio_gpio gpio_5ven;
	struct gpio_gpio hub_rst;
	struct gpio_gpio gsm_en;
	struct gpio_gpio uart5_en;
	struct gpio_gpio fan_en;
	struct gpio_gpio vddio_en;
	int sleep_flag;
//	struct wake_lock rp_wake_lock;
};
//add hub proc------------------------------
static struct power_en_gpio *hub_data = NULL;
static ssize_t hub_rst_proc_write(struct file *file, const char *buffer,size_t count, loff_t *data)
{	
			printk("************\n");
			gpio_direction_output(hub_data->hub_rst.gpio_num, 0);
        	gpio_set_value(hub_data->hub_rst.gpio_num, 0);
			msleep(500);
			gpio_direction_output(hub_data->hub_rst.gpio_num, 1);
        	gpio_set_value(hub_data->hub_rst.gpio_num, 1);
			return count;
			
}

static const struct file_operations hub_rst_proc = {
	.owner		= THIS_MODULE,
	.write		= hub_rst_proc_write,
};
static struct proc_dir_entry *gpio_ctl_entry;
//------------------------------------------
static int power_en_probe(struct platform_device *pdev)
{
        int ret = 0;
        struct device_node *np = pdev->dev.of_node;
	struct power_en_gpio *data;
	
	//add hub proc------------------------------
	gpio_ctl_entry = proc_mkdir("hub_rst_proc", NULL);
	proc_create("hub_rst_",0666,gpio_ctl_entry,&hub_rst_proc);
	//------------------------------------------
	data = devm_kzalloc(&pdev->dev, sizeof(struct power_en_gpio),GFP_KERNEL);
	if (!data) {
                dev_err(&pdev->dev, "failed to allocate memory\n");
                return -ENOMEM;
        }
	memset(data, 0, sizeof(struct power_en_gpio));
	//add hub proc------------------------------	
	hub_data = data;
	//------------------------------------------
        data->gpio_5ven.gpio_num = of_get_named_gpio_flags(np, "power_en-gpio", 0, NULL);
        if (!gpio_is_valid(data->gpio_5ven.gpio_num))
                data->gpio_5ven.gpio_num = -1;

        data->vddio_en.gpio_num = of_get_named_gpio_flags(np, "vddio_en", 0, NULL);
        if (!gpio_is_valid(data->vddio_en.gpio_num))
                data->vddio_en.gpio_num = -1;
				
        data->uart5_en.gpio_num = of_get_named_gpio_flags(np, "uart5_en-gpio", 0, NULL);
        if (!gpio_is_valid(data->uart5_en.gpio_num))
                data->uart5_en.gpio_num = -1;

        data->hub_rst.gpio_num = of_get_named_gpio_flags(np, "hub_rst", 0, NULL);
        if (!gpio_is_valid(data->hub_rst.gpio_num))
                data->hub_rst.gpio_num = -1;
                
        data->gsm_en.gpio_num = of_get_named_gpio_flags(np, "3v3_power_en", 0, NULL);
        if (!gpio_is_valid(data->gsm_en.gpio_num))
                data->gsm_en.gpio_num = -1;

        data->fan_en.gpio_num = of_get_named_gpio_flags(np, "fan_open", 0, NULL);
        if (!gpio_is_valid(data->fan_en.gpio_num))
                data->fan_en.gpio_num = -1;
	of_property_read_u32(np, "rp_not_deep_leep", &data->sleep_flag);


	platform_set_drvdata(pdev, data);

	if(data->gpio_5ven.gpio_num != -1){
		ret = gpio_request(data->gpio_5ven.gpio_num, "power_5v_en");
        	if (ret < 0){
			printk("data->gpio_5ven request error\n");
//        	        return ret;
		}else{
			gpio_direction_output(data->gpio_5ven.gpio_num, 1);
        		gpio_set_value(data->gpio_5ven.gpio_num, 1);
		}
	}

	if(data->vddio_en.gpio_num != -1){
		ret = gpio_request(data->vddio_en.gpio_num, "vddio_en");
        	if (ret < 0){
			printk("data->vddio_en request error\n");
//        	        return ret;
		}else{
			gpio_direction_output(data->vddio_en.gpio_num, 1);
        		gpio_set_value(data->vddio_en.gpio_num, 1);
		}
	}

	if(data->hub_rst.gpio_num != -1){
		ret = gpio_request(data->hub_rst.gpio_num, "hub_rst");
        	if (ret < 0){
			printk("data->hub_rst request error\n");
//        	        return ret;
		}else{
//			gpio_direction_output(data->hub_rst.gpio_num, 0);
//        		gpio_set_value(data->hub_rst.gpio_num, 0);
//			msleep(100);
			gpio_direction_output(data->hub_rst.gpio_num, 1);
        		gpio_set_value(data->hub_rst.gpio_num, 1);
		}
	}

	if(data->uart5_en.gpio_num != -1){
		ret = gpio_request(data->uart5_en.gpio_num, "uart5_en");
        	if (ret < 0){
			printk("data->uart5_en request error\n");
//        	        return ret;
		}else{
			gpio_direction_output(data->uart5_en.gpio_num, 1);
        		gpio_set_value(data->uart5_en.gpio_num, 1);
		}
	}
        
	if(data->gsm_en.gpio_num != -1){
		ret = gpio_request(data->gsm_en.gpio_num, "3v3_en");
        	if (ret < 0){
			printk("data->gsm_en request error\n");
//        	        return ret;
		}else{
			gpio_direction_output(data->gsm_en.gpio_num, 1);
  			gpio_set_value(data->gsm_en.gpio_num, 1);
		}
	}
  
	if(data->fan_en.gpio_num != -1){
		ret = gpio_request(data->fan_en.gpio_num, "fan_en");
        	if (ret < 0){
			printk("data->fan_en request error\n");
//        	        return ret;
		}else{
			gpio_direction_output(data->fan_en.gpio_num, 1);
  			gpio_set_value(data->fan_en.gpio_num, 1);
		}
	}
	// if(data->sleep_flag != 0){
	// 	wake_lock_init(&data->rp_wake_lock,WAKE_LOCK_SUSPEND, "rpdzkj_no_deep_sleep");
	// 	wake_lock(&data->rp_wake_lock);
	// }

        return 0;
}

static int power_en_remove(struct platform_device *pdev)
{
        struct power_en_gpio *data = platform_get_drvdata(pdev);


	if(data->gpio_5ven.gpio_num != -1){
		gpio_direction_output(data->gpio_5ven.gpio_num, 0);
		gpio_free(data->gpio_5ven.gpio_num);
	}
	if(data->vddio_en.gpio_num != -1){
		gpio_direction_output(data->vddio_en.gpio_num, 0);
		gpio_free(data->vddio_en.gpio_num);
	}
	if(data->hub_rst.gpio_num != -1){
		gpio_direction_output(data->hub_rst.gpio_num, 0);
		gpio_free(data->hub_rst.gpio_num);
	}
	if(data->uart5_en.gpio_num != -1){
		gpio_direction_output(data->uart5_en.gpio_num, 0);
		gpio_free(data->uart5_en.gpio_num);
	}
	if(data->gsm_en.gpio_num != -1){
		gpio_direction_output(data->gsm_en.gpio_num, 0);
		gpio_free(data->gsm_en.gpio_num);
	}
	if(data->fan_en.gpio_num != -1){
		gpio_direction_output(data->fan_en.gpio_num, 0);
		gpio_free(data->fan_en.gpio_num);
	}
	// if(data->sleep_flag != 0){
	// 	wake_unlock(&data->rp_wake_lock);
	// }

        return 0;
}

#ifdef CONFIG_PM 
static int power_en_suspend(struct device *dev) 
{ 
	struct platform_device *pdev = to_platform_device(dev);
        struct power_en_gpio *data = platform_get_drvdata(pdev);
 
	if(data->gpio_5ven.gpio_num != -1){
		gpio_direction_output(data->gpio_5ven.gpio_num, 0);
		gpio_set_value(data->gpio_5ven.gpio_num,0);
	}
	if(data->vddio_en.gpio_num != -1){
		gpio_direction_output(data->vddio_en.gpio_num, 0);
		gpio_set_value(data->vddio_en.gpio_num,0);
	}
	if(data->hub_rst.gpio_num != -1){
		gpio_direction_output(data->hub_rst.gpio_num, 0);
		gpio_set_value(data->hub_rst.gpio_num,0);
	}
	if(data->uart5_en.gpio_num != -1){
		gpio_direction_output(data->uart5_en.gpio_num, 0);
		gpio_set_value(data->uart5_en.gpio_num,0);
	}
	if(data->gsm_en.gpio_num != -1){
		gpio_direction_output(data->gsm_en.gpio_num, 0);
		gpio_set_value(data->gsm_en.gpio_num,0);
	}
	if(data->fan_en.gpio_num != -1){
		gpio_direction_output(data->fan_en.gpio_num, 0);
		gpio_set_value(data->fan_en.gpio_num,0);
	}
 
        return 0; 
} 
 
static int power_en_resume(struct device *dev) 
{ 
	struct platform_device *pdev = to_platform_device(dev);
        struct power_en_gpio *data = platform_get_drvdata(pdev);
 
	if(data->gpio_5ven.gpio_num != -1){
			gpio_direction_output(data->gpio_5ven.gpio_num, 1);
        	gpio_set_value(data->gpio_5ven.gpio_num, 1);
	}
	
	if(data->vddio_en.gpio_num != -1){
			gpio_direction_output(data->vddio_en.gpio_num, 1);
        	gpio_set_value(data->vddio_en.gpio_num, 1);
	}

	if(data->hub_rst.gpio_num != -1){
			//gpio_direction_output(data->hub_rst.gpio_num, 0);
        		//gpio_set_value(data->hub_rst.gpio_num, 0);
			//msleep(100);
			gpio_direction_output(data->hub_rst.gpio_num, 1);
        	gpio_set_value(data->hub_rst.gpio_num, 1);
	}

	if(data->uart5_en.gpio_num != -1){
			gpio_direction_output(data->uart5_en.gpio_num, 1);
        		gpio_set_value(data->uart5_en.gpio_num, 1);
	}
        
	if(data->gsm_en.gpio_num != -1){
			gpio_direction_output(data->gsm_en.gpio_num, 1);
  			gpio_set_value(data->gsm_en.gpio_num, 1);
	}
  
	if(data->fan_en.gpio_num != -1){
			gpio_direction_output(data->fan_en.gpio_num, 1);
  			gpio_set_value(data->fan_en.gpio_num, 1);
	}
        return 0; 
} 
 
static const struct dev_pm_ops power_en_pm_ops = { 
        .suspend        = power_en_suspend, 
        .resume         = power_en_resume, 
}; 
#endif

static const struct of_device_id power_en_of_match[] = {
        { .compatible = "5v-en-gpio" },
        { }
};

static struct platform_driver power_en_driver = {
        .probe = power_en_probe,
        .remove = power_en_remove,
        .driver = {
                .name           = "5v-en-gpio",
                .of_match_table = of_match_ptr(power_en_of_match),
#ifdef CONFIG_PM
                .pm     = &power_en_pm_ops,
#endif

        },
};

module_platform_driver(power_en_driver);

MODULE_LICENSE("GPL");
