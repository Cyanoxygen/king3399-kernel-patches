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


struct wake_lock rp_wake_lock;

static int wake_lock_probe(struct platform_device *pdev)
{
	
		wake_lock_init(&rp_wake_lock,WAKE_LOCK_SUSPEND, "rpdzkj_no_deep_sleep");
		wake_lock(&rp_wake_lock);
	
    return 0;
}

static int wake_lock_remove(struct platform_device *pdev)
{
        return 0;
}

#ifdef CONFIG_PM 
static int wake_lock_suspend(struct device *dev) 
{ 
        return 0; 
} 
 
static int wake_lock_resume(struct device *dev) 
{ 
        return 0; 
} 
 
static const struct dev_pm_ops wake_lock_pm_ops = { 
        .suspend        = wake_lock_suspend, 
        .resume         = wake_lock_resume, 
}; 
#endif

static const struct of_device_id wake_lock_of_match[] = {
        { .compatible = "rp_wake_lock" },
        { }
};

static struct platform_driver wake_lock_driver = {
        .probe = wake_lock_probe,
        .remove = wake_lock_remove,
        .driver = {
                .name           = "rp_wack_loack",
                .of_match_table = of_match_ptr(wake_lock_of_match),
#ifdef CONFIG_PM
                .pm     = &wake_lock_pm_ops,
#endif

        },
};

module_platform_driver(wake_lock_driver);
MODULE_LICENSE("GPL");
