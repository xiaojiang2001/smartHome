#include <linux/fs.h>			// 包含了文件系统相关的数据结构和函数		file_operations声明
#include <linux/init.h>			// 包含了模块初始化和清理函数的宏定义		__init  __exit 宏定义声明
#include <linux/module.h>		// 提供了Linux内核模块的基本函数和宏		module_init  module_exit声明
#include <linux/cdev.h>			// 定义了字符设备相关的结构和函数			cdev_init 字符设备初始化
#include <linux/device.h>		// 包含了设备类和设备的定义				class  devise声明
#include <linux/uaccess.h>		// 提供了用户空间和内核空间数据传输的函数	copy_from_user 的头文件
#include <linux/types.h>		// 提供了用户空间和内核空间数据传输的函数	设备号  dev_t 类型声明

//变量定义
static struct class  *pin4_class;
static struct device *pin4_class_dev;

static dev_t devno;             // 设备号
static int major = 231;         // 主设备号
static int minor = 0;           // 次设备号
static  char* module_name = "pin4"; // 模块名


volatile unsigned int* GPFSEL0 = NULL;
volatile unsigned int* GPSET0  = NULL;
volatile unsigned int* GPCLR0  = NULL;

// led_open 函数
static int pin4_open(struct inode *inode, struct file *file)
{
    printk("pin4 open\n");         //内核的打印函数用 printk
   
    // 配置为输出引脚 bits 12-14位为001
    *GPFSEL0 &= ~(0x6 << 12);	// bit13 14配置为0
    *GPFSEL0 |= 0x1 << 12;	// bit12配置为1

    return 0;
}

// led_write 函数
static int pin4_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{

    int userCmd;

    printk("pin4 write\n");
   
    // 获取上层write的值
    copy_from_user(&userCmd, buf, count);
    printk("get user val\n");

    // 根据值操作IO口 高低电平
    if(userCmd == 1){
        printk("set 1\n");
        *GPSET0 |= 0x01 << 4;
    }
    else if(userCmd == 0) {
        printk("set 0\n");
        *GPCLR0 |= 0x01 << 4;
    }
    else
	    printk("undo do");

    

    return 0;
}

// led_read 函数
static int pin4_read(struct file* file, char __user * buf, size_t count, loff_t *ppos)
{
    printk("pin4 read\n");
    return 0;
}

static struct file_operations pin4_fops = {
    .owner = THIS_MODULE,
    .open  = pin4_open,
    .write = pin4_write,
    .read  = pin4_read
};

// 真实驱动入口
int __init pin4_drv_init(void)    
{ 
    int ret;
    devno = MKDEV(major, minor);    // 2. 创建设备号
    //3. 注册驱动 告诉内核 把这个驱动加入到内核链表中
    ret = register_chrdev(major, module_name, &pin4_fops);  

    // 让代码在dev下自动生成设备 
    pin4_class = class_create(THIS_MODULE, "myfirstdemo");
    // 创建设备文件
    pin4_class_dev = device_create(pin4_class, NULL, devno, NULL, module_name);

    // 物理地址映射为虚拟地址
    GPFSEL0 = (volatile unsigned int*)ioremap(0x3f200000, 4);
    GPSET0  = (volatile unsigned int*)ioremap(0x3f20001C, 4);
    GPCLR0  = (volatile unsigned int*)ioremap(0x3f200028, 4);

    printk("insmod driver pin4 success\n");

    return 0;
}

void __exit pin4_drv_exit(void)
{
    // 卸载驱动
    iounmap(GPFSEL0);
    iounmap(GPSET0);
    iounmap(GPCLR0);


    device_destroy(pin4_class, devno);
    class_destroy(pin4_class);
    unregister_chrdev(major, module_name);          //卸载驱动
}

module_init(pin4_drv_init);     //入口, 内核加载该驱动的时候，这个宏会被调用
module_exit(pin4_drv_exit);
MODULE_LICENSE("GPL v2");

