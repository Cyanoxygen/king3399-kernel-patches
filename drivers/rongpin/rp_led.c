#include <linux/module.h>
#include <linux/err.h>
#include <linux/gpio.h>
#include <linux/kernel.h> 
#include <linux/of_gpio.h>
#include <linux/of_platform.h>
#include <linux/delay.h>

struct led_gpio {
	int gpio_num;
	int val;
};

struct led_led_gpio {
	struct led_gpio breathe_led;
};

struct led_led_gpio *led_data; // WARN: Unsafe!

static struct timer_list mytimer;

// void function(unsigned long data){
void function(struct timer_list *list) {
	struct led_led_gpio *tmp = (struct led_led_gpio *)led_data;
	if (tmp == NULL) {
		printk("rp-led: GPIO struct is uninitialized.");
		return;
	}
	tmp->breathe_led.val ^= 1;
	gpio_direction_output(tmp->breathe_led.gpio_num,tmp->breathe_led.val);
	mod_timer(&mytimer, jiffies + msecs_to_jiffies(2000));
	return;
}

static int led_probe(struct platform_device *pdev)
{
	int ret = 0;
	struct device_node *np = pdev->dev.of_node;
	struct led_led_gpio *data = led_data;

	led_data = devm_kzalloc(&pdev->dev, sizeof(struct led_led_gpio),GFP_KERNEL);
	if (!data) {
		dev_err(&pdev->dev, "failed to allocate memory\n");
		return -ENOMEM;
	}
	memset(data, 0, sizeof(struct led_led_gpio));

	data->breathe_led.gpio_num = of_get_named_gpio_flags(np, "breathe-led", 0, NULL);
	if (!gpio_is_valid(data->breathe_led.gpio_num))
		data->breathe_led.gpio_num = -1;

	platform_set_drvdata(pdev, data);

	if(data->breathe_led.gpio_num != -1){
		ret = gpio_request(data->breathe_led.gpio_num, "breathe-led");
		if (ret < 0){
			printk("data->gpio_5ven request error\n");
			return ret;
		}else{
			gpio_direction_output(data->breathe_led.gpio_num, 0);
			gpio_set_value(data->breathe_led.gpio_num, 0);
			data->breathe_led.val = 0;
		}
	}
	// init_timer(&mytimer);
	
	// mytimer.function = function;
	// mytimer.data = (unsigned long )data;
	// add_timer(&mytimer);
	mytimer.expires = jiffies + jiffies_to_msecs(2000);
	timer_setup(&mytimer, function, 0);
	add_timer(&mytimer);
	return 0;
}

static int led_remove(struct platform_device *pdev)
{
	struct led_led_gpio *data = platform_get_drvdata(pdev);

	if(data->breathe_led.gpio_num != -1){
		gpio_direction_output(data->breathe_led.gpio_num, 1);
		gpio_free(data->breathe_led.gpio_num);
	}

	return 0;
}

#ifdef CONFIG_PM 
static int led_suspend(struct device *dev) 
{ 
	struct platform_device *pdev = to_platform_device(dev);
	struct led_led_gpio *data = platform_get_drvdata(pdev);
 
	del_timer(&mytimer);
	if(data->breathe_led.val == 0){
		gpio_direction_output(data->breathe_led.gpio_num,1);
		data->breathe_led.val = 1;
	}
 
	return 0; 
} 
 
static int led_resume(struct device *dev) 
{ 
	struct platform_device *pdev = to_platform_device(dev);
	struct led_led_gpio *data = platform_get_drvdata(pdev);
 
	gpio_direction_output(data->breathe_led.gpio_num,0);
	data->breathe_led.val = 0;
	add_timer(&mytimer);
	return 0; 
} 
 
static const struct dev_pm_ops led_pm_ops = { 
	.suspend	= led_suspend, 
	.resume	 = led_resume, 
}; 
#endif

static const struct of_device_id led_of_match[] = {
	{ .compatible = "led-led" },
	{ }
};

static struct platform_driver led_driver = {
	.probe = led_probe,
	.remove = led_remove,
	.driver = {
		.name	   = "led-led",
		.of_match_table = of_match_ptr(led_of_match),
#ifdef CONFIG_PM
		.pm     = &led_pm_ops,
#endif

	},
};

module_platform_driver(led_driver);

MODULE_LICENSE("GPL");
