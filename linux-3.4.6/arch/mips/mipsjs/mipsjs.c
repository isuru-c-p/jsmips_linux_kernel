#include <linux/init.h>
#include <asm/bootinfo.h>


void __init plat_time_init(void)
{

}


/* asmlinkage TODO uncomment? */ void plat_irq_dispatch(void)
{
	
}

void __init arch_init_irq(void)
{

}



const char *get_system_type(void)
{
	return "MIPSJS";
}


void __init plat_mem_setup(void)
{

    add_memory_region(0x00000000, 0x00001000, BOOT_MEM_RESERVED);
    add_memory_region(0x00001000, 0x800000 /* 8 megs */, BOOT_MEM_RAM); //TODO ? alignment?

}


void __init prom_init(void){


}


void __init prom_free_prom_memory(void){

}

