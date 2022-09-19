#include <linux/module.h>
#include <linux/err.h>
#include <linux/gpio.h>
#include <linux/kernel.h>
#include <linux/of_gpio.h>
#include <linux/of_platform.h>
#include <linux/delay.h>
#include <linux/wakelock.h>
#include <linux/of.h>
#include <linux/of_fdt.h>

struct special_gpio {
	int gpio_num;
	int val;
};

struct special_power_en_gpio {
	struct special_gpio special_power;
};

static int special_power_probe(struct platform_device *pdev)
{
        int ret = 0;
        struct device_node *np = pdev->dev.of_node;
	struct special_power_en_gpio *data;

	data = devm_kzalloc(&pdev->dev, sizeof(struct special_power_en_gpio),GFP_KERNEL);
	if (!data) {
                dev_err(&pdev->dev, "failed to allocate memory\n");
                return -ENOMEM;
        }
	memset(data, 0, sizeof(struct special_power_en_gpio));

        data->special_power.gpio_num = of_get_named_gpio_flags(np, "special-power", 0, NULL);
        if (!gpio_is_valid(data->special_power.gpio_num))
                data->special_power.gpio_num = -1;
	if(data->special_power.gpio_num != -1){
		ret = gpio_request(data->special_power.gpio_num, "special_power");
        	if (ret < 0){
			printk("data->fan_en request error\n");
//        	        return ret;
		}else{
			gpio_direction_output(data->special_power.gpio_num, 1);
  			gpio_set_value(data->special_power.gpio_num, 1);
			gpio_free(data->special_power.gpio_num);
		}
	}
	        return 0;
}

static int special_power_remove(struct platform_device *pdev)
{
        return 0;
}

#ifdef CONFIG_PM 
static int special_power_suspend(struct device *dev) 
{ 
	struct platform_device *pdev = to_platform_device(dev);
        struct power_en_gpio *data = platform_get_drvdata(pdev);
 
	if(data->special_power.gpio_num != -1){
		ret = gpio_request(data->special_power.gpio_num, "special_power");
        	if (ret < 0){
			printk("data->fan_en request error\n");
//        	        return ret;
		}else{
			gpio_direction_output(data->special_power.gpio_num, 0);
  			gpio_set_value(data->special_power.gpio_num, 0);
			gpio_free(data->special_power.gpio_num);
		}
	}
        return 0; 
} 
 
static int special_power_resume(struct device *dev) 
{ 
	struct platform_device *pdev = to_platform_device(dev);
        struct power_en_gpio *data = platform_get_drvdata(pdev);
 
 	if(data->special_power.gpio_num != -1){
		ret = gpio_request(data->special_power.gpio_num, "special_power");
        	if (ret < 0){
			printk("data->fan_en request error\n");
//        	        return ret;
		}else{
			gpio_direction_output(data->special_power.gpio_num, 1);
  			gpio_set_value(data->special_power.gpio_num, 1);
			gpio_free(data->special_power.gpio_num);
		}
	}
        return 0; 
} 
 
static const struct dev_pm_ops special_power_pm_ops = { 
        .suspend        = special_power_suspend, 
        .resume         = special_power_resume, 
}; 
#endif

static const struct of_device_id special_power_en_of_match[] = {
        { .compatible = "special_en" },
        { }
};

static struct platform_driver special_power_en_driver = {
        .probe = special_power_probe,
        .remove = special_power_remove,
        .driver = {
                .name           = "special-en",
                .of_match_table = of_match_ptr(special_power_en_of_match),
#ifdef CONFIG_PM
                .pm     = &special_power_pm_ops,
#endif

        },
};


static int __init rpdzkj_cfg_init(void)
{
	return platform_driver_register(&special_power_en_driver);
}
subsys_initcall(rpdzkj_cfg_init);

static void __exit rpdzkj_cfg_exit(void)
{
	platform_driver_unregister(&special_power_en_driver);
}
module_exit(rpdzkj_cfg_exit);


MODULE_AUTHOR("bill liu");
MODULE_LICENSE("GPL");
