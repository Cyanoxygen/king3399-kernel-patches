#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/gpio.h>
#include <linux/proc_fs.h>
#include <linux/wakelock.h>
#include <linux/err.h>
#include <linux/of_gpio.h>
#include <linux/of_platform.h>
#include <linux/delay.h>

#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/of_device.h>

#include <linux/slab.h>
#include <linux/device.h>
#include <asm/uaccess.h>


struct rpdzkj_config {
	char system_rotate[4];
	char back_camera_rotate[4];
	char front_camera_rotate[4];
	char lcd_density[4];
	char language[64];
	char time_zone[64];
	char not_navigation_bar[8];
	char not_status_bar[8];
	char default_launcher[8];
	char has_root[8];
	char usb_not_permission[8];
	char usb_camera_only_front[8];
	char gps_use[8];
	char gps_serial_port[16];
	char user_version[16];
	char primary_device[16];
	char extend_device[16];
	char extend_rotate[4];
	char rotation_efull[8];
	char home_apk[64];
};

static struct proc_dir_entry *system_config_entry;
static struct rpdzkj_config *tmp_data;



ssize_t rpdzkj_config_write(struct file * file,const char __user* buffer,size_t count,loff_t *data)
{
        return count;
}
static int rpdzkj_config_open(struct inode *inode, struct file *file)
{
        return 0;
}
#define STRING_READ(x) x##_read
#define STRING_READ_FUN(x) \
static ssize_t STRING_READ(x)(struct file *file, char __user *buf,size_t count, loff_t *ppos)\
{\
        if(copy_to_user(buf,tmp_data->x,sizeof(tmp_data->x))){\
                printk("copy_to_user erro __LINE__ = %d    __func__ = %s\n",__LINE__,__func__);\
                return 0;\
        }\
        return sizeof(tmp_data->x);\
}
#define STRING_READ_OPS(x) \
static const struct file_operations x##_ops = {\
        .open           = rpdzkj_config_open,\
        .write          = rpdzkj_config_write,\
        .read           = STRING_READ(x),\
};
#define STRING_FUN(x) 	STRING_READ_FUN(x)\
			STRING_READ_OPS(x)
			
#define STRING_READ_DTS(x,y,z)	\
	do{\
		if (!of_property_read_string(np, #x, &z)){\
			strcpy(tmp_data->x, z);\
		}\
		else\
			strcpy(tmp_data->x, y);\
	}while(0);
#define PROC_CREATE_STR(x) proc_create(#x,0666,system_config_entry,&x##_ops);

STRING_FUN(system_rotate)
STRING_FUN(back_camera_rotate)
STRING_FUN(front_camera_rotate)
STRING_FUN(lcd_density)
STRING_FUN(language)
STRING_FUN(time_zone)
STRING_FUN(not_navigation_bar)
STRING_FUN(not_status_bar)
STRING_FUN(default_launcher)
STRING_FUN(has_root)
STRING_FUN(usb_not_permission)
STRING_FUN(usb_camera_only_front)
STRING_FUN(gps_use)
STRING_FUN(gps_serial_port)
STRING_FUN(user_version)
STRING_FUN(primary_device)
STRING_FUN(extend_device)
STRING_FUN(extend_rotate)
STRING_FUN(rotation_efull)
STRING_FUN(home_apk)

static int rpdzkj_cfg_probe(struct platform_device *pdev)
{
	const char *temp;
        struct device_node *np = pdev->dev.of_node;

	tmp_data = devm_kzalloc(&pdev->dev, sizeof(struct rpdzkj_config),GFP_KERNEL);
	if (!tmp_data) {
                dev_err(&pdev->dev, "failed to allocate memory\n");
                return -ENOMEM;
        }
	memset(tmp_data, 0, sizeof(struct rpdzkj_config));

	STRING_READ_DTS(system_rotate,"0",temp)
	STRING_READ_DTS(back_camera_rotate,"0",temp)
	STRING_READ_DTS(front_camera_rotate,"0",temp)
	STRING_READ_DTS(lcd_density,"240",temp)
	STRING_READ_DTS(language,"en-US",temp)
	STRING_READ_DTS(time_zone,"Europe/London",temp)
	STRING_READ_DTS(not_navigation_bar,"false",temp)
	STRING_READ_DTS(not_status_bar,"false",temp)
	STRING_READ_DTS(default_launcher,"true",temp)
	STRING_READ_DTS(has_root,"false",temp)
	STRING_READ_DTS(usb_not_permission,"true",temp)
	STRING_READ_DTS(usb_camera_only_front,"false",temp)
	STRING_READ_DTS(gps_use,"false",temp)
	STRING_READ_DTS(gps_serial_port,"/dev/ttyS4",temp)
	STRING_READ_DTS(user_version,"rpdzkj",temp)
	STRING_READ_DTS(primary_device,"null",temp)
	STRING_READ_DTS(extend_device,"null",temp)
	STRING_READ_DTS(extend_rotate,"0",temp)
	STRING_READ_DTS(rotation_efull,"false",temp)
	STRING_READ_DTS(home_apk,"null",temp)

	platform_set_drvdata(pdev, tmp_data);
	
	system_config_entry = proc_mkdir("rp_prop", NULL);
	PROC_CREATE_STR(system_rotate)
	PROC_CREATE_STR(back_camera_rotate)
	PROC_CREATE_STR(front_camera_rotate)
	PROC_CREATE_STR(lcd_density)
	PROC_CREATE_STR(language)
	PROC_CREATE_STR(time_zone)
	PROC_CREATE_STR(not_navigation_bar)
	PROC_CREATE_STR(not_status_bar)
	PROC_CREATE_STR(default_launcher)
	PROC_CREATE_STR(has_root)
	PROC_CREATE_STR(usb_not_permission)
	PROC_CREATE_STR(usb_camera_only_front)
	PROC_CREATE_STR(gps_use)
	PROC_CREATE_STR(gps_serial_port)
	PROC_CREATE_STR(user_version)
	PROC_CREATE_STR(primary_device)
	PROC_CREATE_STR(extend_device)
	PROC_CREATE_STR(extend_rotate)
	PROC_CREATE_STR(rotation_efull)
	PROC_CREATE_STR(home_apk)

        return 0;
}

static int rpdzkj_cfg_remove(struct platform_device *pdev)
{

        return 0;
}
/*
static int rpdzkj_cfg_suspend(struct platform_device *pdev, pm_message_t state) 
{ 
 
        return 0; 
} 
 
static int rpdzkj_cfg_resume(struct platform_device *pdev) 
{ 
 
        return 0; 
} 
*/

static const struct of_device_id rpdzkj_cfg_of_match[] = {
        { .compatible = "rp_config" },
        { }
};

static struct platform_driver rpdzkj_cfg_driver = {
        .probe = rpdzkj_cfg_probe,
        .remove = rpdzkj_cfg_remove,
        .driver = {
                .name           = "rp_config_en",
                .of_match_table = of_match_ptr(rpdzkj_cfg_of_match),
				.owner   = THIS_MODULE,

        },
//	.suspend = rpdzkj_cfg_suspend,
//	.resume = rpdzkj_cfg_resume,
};


static int __init rpdzkj_cfg_init(void)
{
	return platform_driver_register(&rpdzkj_cfg_driver);
}
subsys_initcall(rpdzkj_cfg_init);

static void __exit rpdzkj_cfg_exit(void)
{
	platform_driver_unregister(&rpdzkj_cfg_driver);
}
module_exit(rpdzkj_cfg_exit);


MODULE_AUTHOR("bill liu");
MODULE_LICENSE("GPL");
