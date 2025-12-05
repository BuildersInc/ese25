#include <linux/module.h>
#include <linux/init.h>

static int __init my_init(void)
{
	int i = 5;
	printk("Hello kernel %i\n", i);
	return 0;
}

static void __exit my_exit(void)
{
	for (int i = 0; i <= 10; i++)
	{
		printk("In dem Kernelloop %i", i);
	}
	printk("Bye Bye kernel\n");
}

MODULE_LICENSE("GPL"); // required
MODULE_AUTHOR("Felix Schmidt"); //optional
MODULE_DESCRIPTION("Hello Kernel"); // Optional

module_init(my_init);
module_exit(my_exit);
