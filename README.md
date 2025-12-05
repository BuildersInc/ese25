# Linux Treiber Workshop auf dem ESE Kongress 2025
## Entwicklungsumgebung
### Compile auf Zielsystem

Einfach zu Setup

Resourcenarm

### Compile auf dev System

Schwieriger Setup
Benötigt den Kernel und Zielconfig



## Notwendige Installs

sudo apt install -y raspberrypi-kernel-headers
Mittlerweile
sudo apt install -y linux-headers-rpi

sudo apt install -y build-essential


Kernelheaders und Linux Kernel müssen gleiche Version haben


Prüfen:
ls /lib/modules/$(uname -r)/
build ordner muss da sein



# Hello Kernel

Bei einem printk unbedingt mit \n terminieren. 
Sonst wird das nicht in den Log geschrieben


```c
#include <linux/module.h>  
#include <linux/init.h>  
  
static int __init my_init(void)  
{  
	printk("Hello Kernel\n");
}  
  
// v--- Am besten diese Funktionen immer Static machen
static void __exit my_exit(void)  
{  
	printk("Exit Kernel");
}  
  
MODULE_LICENSE("GPL") // required  
MODULE_AUTHOR("Felix Schmidt") //optional  
MODULE_DESCRIPTION("Hello Kernel") // Optional

// Das __init und __exit ist nur der lesbarkeit und "optional"
```

```makefile
# Kernel Header Makefile kompiliert hello.c automatisch zu hello.o file  
obj-m += hello.o  
  
all:  
       make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules  
clean:  
       make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
```

obj-m => Als KernelModul
obj-y => in den Kernl direkt rein

## Wichtige commands
- lsmod => zeigt die geladenen Module
- dmesg => zeigt den Kernel log
	- sudo dmesg -W => Wartet auf neue logs
- insmod hello.ko => Lädt das Modul
- rmmod hello.ko => entfernt
- modprobe lädt das Module mit abhängigkeiten
	- das module muss im Kernel ausgeliefert sein
- 


## Gelernt
printk unterstüzt die Seleben formater wie printf

Es gibt auch fertige loglevel
https://www.kernel.org/doc/html/latest/core-api/printk-basics.html
pr_emerg[](https://www.kernel.org/doc/html/latest/core-api/printk-basics.html#c.pr_emerg "Permalink to this definition") printet zum Beispiel dierekt mit der Emergency stufe


Die Meisten standart c funktionen gehen ohne probleme. 
Malloc muss aber mit sowas wie kmalloc aufgerufen werden


## I2C Treiber
https://elixir.bootlin.com/u-boot/v2024.07/source


## Device Tree Overlay

```
/dts-v1/;
/plugin/;
/ {
        fragment@0 {
                target = <& i2c1 >;
                __overlay__ {
                        #address-cells = <1>;
                        #size-cells = <0>;
                        
                        my_dev : my_dev@12 {
                                compatible = "brightlight, mydev" ,
                                status = "okay";
                                reg = <0x12>;
                        };
                };
        };
};
```

Mehr Info: https://www.devicetree.org/
WICHTIG:
Die namen "compatible" MÜSSEN überall gleich sein

