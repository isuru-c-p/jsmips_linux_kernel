#include <linux/init.h>
#include <asm/bootinfo.h>

#include <linux/sched.h>
#include <linux/interrupt.h>
#include <linux/kernel_stat.h>
#include <asm/mips-boards/simint.h>
#include <asm/irq_cpu.h>

#include <linux/serial_8250.h>
#include <linux/platform_device.h>
#include <asm/time.h>

#define MIPS_CPU_IRQ_BASE 0

#define SMC_PORT(base, int)						\
{									\
	.iobase		= base,						\
	.irq		= int,						\
	.uartclk	= 1843200,					\
	.iotype		= UPIO_PORT,					\
	.flags		= UPF_BOOT_AUTOCONF | UPF_SKIP_TEST,		\
	.regshift	= 0,						\
}

#define CBUS_UART_FLAGS (UPF_BOOT_AUTOCONF | UPF_SKIP_TEST | UPF_IOREMAP)

/*static struct plat_serial8250_port uart8250_data[] = {
	SMC_PORT(0x3F8, 4),
	SMC_PORT(0x2F8, 3),
	{
		.mapbase	= 0x1f000900,
		.irq		= MIPS_CPU_IRQ_BASE + 2,
		.uartclk	= 3686400,
		.iotype		= UPIO_MEM32,
		.flags		= CBUS_UART_FLAGS,
		.regshift	= 3,
	},
	{ },
};

static struct platform_device mipsjs_uart8250_device = {
	.name			= "serial8250",
	.id			= PLAT8250_DEV_PLATFORM,
	.dev			= {
		.platform_data	= uart8250_data,
	},
};

static struct platform_device *mipsjs_devices[] __initdata = {
	&mipsjs_uart8250_device,
};

static int __init mipsjs_add_devices(void)
{
    int err;


    printk("Adding mipsjs devices...\n");

    err = platform_add_devices(mipsjs_devices, ARRAY_SIZE(mipsjs_devices));
    
    return err;
}*/

unsigned __cpuinit get_c0_compare_int(void)
{
    return 7;
}

void __init plat_time_init(void)
{
    // hardcode to 1MHz
    mips_hpt_frequency = 10000;
}

static inline int clz(unsigned long x)
{
	__asm__(
	"	.set	push					\n"
	"	.set	mips32					\n"
	"	clz	%0, %1					\n"
	"	.set	pop					\n"
	: "=r" (x)
	: "r" (x));

	return x;
}

static inline unsigned int irq_ffs(unsigned int pending)
{
    // irq number = number of 1s in interrupt pending bits
    return (-clz(pending) + 31 - CAUSEB_IP);
}

asmlinkage void plat_irq_dispatch(void)
{
	unsigned int pending = read_c0_cause() & read_c0_status() & ST0_IM;
	int irq;

	irq = irq_ffs(pending);

	if (irq > 0)
		do_IRQ(MIPS_CPU_IRQ_BASE + irq);
	else
		spurious_interrupt();	
}

static struct irqaction mipsjs_uartirq = {
	.handler = no_action,
	.name = "MIPSJS UART",
	.flags = IRQF_NO_THREAD,
};

void __init arch_init_irq(void)
{
    printk("arch_init_irq\n");
    mips_cpu_irq_init();

    setup_irq(MIPS_CPU_IRQ_BASE+2, &mipsjs_uartirq);
    //setup_irq(MIPS_CPU_IRQ_BASE+7, &timer_interrupt);
}



const char *get_system_type(void)
{
	return "MIPSJS";
}


void __init plat_mem_setup(void)
{
	struct uart_port s;

    add_memory_region(0x00000000, 0x00001000, BOOT_MEM_RESERVED);
    add_memory_region(0x00001000, 0x800000 /* 8 megs */, BOOT_MEM_RAM); //TODO ? alignment?

    set_io_port_base(0xbfd00000);


	memset(&s, 0, sizeof(s));

	s.iobase = 0x3f8;

	/* hardware int 4 - the serial int, is CPU int 6
	 but poll for now */
	s.irq =  0;
	s.uartclk = 1843200;
	s.flags = UPF_BOOT_AUTOCONF | UPF_SKIP_TEST;
	s.iotype = UPIO_PORT;
	s.regshift = 0;
	s.timeout = 4;

	if (early_serial_setup(&s) != 0) {
		printk(KERN_ERR "Serial setup failed!\n");
	}
    else
    {
        printk("Serial setup succeeded!\n");
    }


}


void __init prom_init(void){


}


void __init prom_free_prom_memory(void){

}

