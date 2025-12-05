/* Required Header */
#include <linux/i2c.h>
#include <linux/init.h>
#include <linux/module.h>

/* Benenne alle kompatiblen Geräte */
static struct i2c_device_id my_ids[] = {
	/* TODO: Fge das kompatible Gerät ein*/
	{"rgb_brd"},
	{} /* leeres Element signalisiert das Ende der Liste */
};
MODULE_DEVICE_TABLE(i2c, my_ids); // important for the autoloading
// nur dann wichtig wenn ich mit in den Kernel reinkompilen will

/* Funktion wird aufgerufen, wenn ein kompatibles I2C Geraet hinzugefuegt wird */
static int my_probe(struct i2c_client *client)
{
	/* TODO: Füge den benötigten Code ein... */
	printk("Hallo vom I2C Slave mit der Adresse: 0x%x\n", client->addr);
	return 0;
}

/* Funktion wird aufgerufen , wenn ein kompatibles I2C Gerät entfernt wird*/
static void my_remove(struct i2c_client *client)
{
	/* TODO: Fgedenbentigten Code ein... */
	printk("Bye bye 0x%x\n", client->addr);
}

/* Fasse kompatible Geräte , Probe - und Remove - Funktionen in Treiber zusammen */
static struct i2c_driver my_driver = {
	.probe = my_probe,
	.remove = my_remove,
	.id_table = my_ids,
	.driver = {
		.name = "my-i2c-driver",
	}
};


static int __init my_driver_init(void)
{
	int ret = 0;
	ret = i2c_add_driver(&my_driver);
	printk("I2C Driver returned %i\n", ret);

	return ret;
}

static void my_driver_exit(void)
{
	printk("Bye Bye I2C");
	i2c_del_driver(&my_driver);
}

// Alternativ
// module_i2c_driver(my_driver);

// Adden des devices
// cd /sys/bus/i2c/devices/i2c-1
// ADD: 	echo "rbg_brd 0x20 " | sudo tee new_devices
// REMOVE	echo "0x20 " | sudo tee delete_device

/* Registriere den Treiber beim OS */
// module_i2c_driver(my_driver);

/* Informationen über den Treiber */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Felix Schmidt");
MODULE_DESCRIPTION("Basic I2C Treiber");

module_init(my_driver_init);
module_exit(my_driver_exit);
