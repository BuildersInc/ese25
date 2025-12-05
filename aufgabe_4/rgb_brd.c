/* Benötigte Header */
#include <linux/i2c.h>
#include <linux/kobject.h>

/* Globale variable für das Kernel Objekt um dynamische Speicherallokierung zu umgehen */
struct kobject* my_kobj;
struct i2c_client* i2c_dev;
/* TODO: Füge einen globalen Pointer für das I2C Gerät */

/* Otpional TODO: Implementiere die Show Funktion, um den Zustand des Tasters einzulesen*/
// Die Read Funktion
static ssize_t rgb_show(struct kobject *kobj, struct kobj_attribute *attr, char *buffer)
{
	// der Buffer ist in der Regel 4KB big
	int reg_state = i2c_smbus_read_byte(i2c_dev);
	return sprintf(buffer, "BTN STATE : %s \n", reg_state & 1 ? "NOT PRESSED" : "PRESSED");
}

/* TODO: Implementiere die store Funktion um die RGB LED anzusteuern */
// Die Write Funktion
static ssize_t rgb_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buffer, size_t count)
{
	// Count: Die Anzahl Bytes die geschrieben wurden
	int value = 0;
	int ret = 0;
	ret = kstrtoint(buffer, 2, &value);
	ret = i2c_smbus_write_byte(i2c_dev, ((~(value)<< 1) & 0xFF) | 1);
	printk("I got %s\n", buffer);
	return count;
}

/* TODO: Benenne das Attribute in led um */
static struct kobj_attribute mydev_attr = __ATTR(led, 0660, NULL, rgb_store);
						// name des Attr
						// Datei Permissions
						// Function Pointer Show
						// Function Pointer Store
static struct kobj_attribute mydev_btn  = __ATTR(btn, 0660, rgb_show, NULL);

/* Benenne alle kompatiblen Geraete */
static struct i2c_device_id my_ids[] = {
	{"rgb_brd"},
	{} /* leeres Element signalisiert das Ende der Liste */
};
MODULE_DEVICE_TABLE(i2c, my_ids);

/* Funktion wird aufgerufen , wenn ein kompatibles I2C Gerät hinzugefügt wird */
static int my_probe(struct i2c_client *client)
{
	int status;
	i2c_dev = client;
	/* TODO: change the name of kobjects */
	my_kobj = kobject_create_and_add("rgb_brd", my_kobj);
	if (!my_kobj) {
		printk("Error creating kernel object\n");
		return -ENOMEM;
	}

	status = sysfs_create_file(my_kobj, &mydev_attr.attr);
	if (status) {
		printk("Error creating /sys/my_kobj/led\n");
		goto rm_kobj;
	}
	
        status = sysfs_create_file(my_kobj, &mydev_btn.attr);
        if (status) {
                printk("Error creating /sys/my_kobj/btn\n");
                goto rm_kobj;
        }
	/* Optional TODO: Lege eine Datei für den Taster an */
	return 0;

rm_kobj:
	kobject_put(my_kobj);
	return status;
}

/* Funktion wird aufgerufen , wenn ein kompatibles I2C Gerät entfernt wird */
static void my_remove(struct i2c_client *client)
{
	sysfs_remove_file(my_kobj, &mydev_attr.attr);
	sysfs_remove_file(my_kobj, &mydev_btn.attr);
	/* Optional TODO: Entferne die Datei für den Taster */
	kobject_put(my_kobj);
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

/* Registriere den Treiber beim OS */
module_i2c_driver(my_driver);

/* Informationen über den Treiber */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Felix Schmidt");
MODULE_DESCRIPTION("Basic i2c Treiber");
