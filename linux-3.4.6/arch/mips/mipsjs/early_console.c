#include <linux/init.h>
#include <linux/console.h>
#include <linux/serial_reg.h>
#include <asm/io.h>


#define PORT(offset) (0x1f000900 + (offset))


static inline unsigned int serial_in(int offset)
{
	return inb(PORT(offset));
}

static inline void serial_out(int offset, int value)
{
	outb(value, PORT(offset));
}

int prom_putchar(char c)
{
	while ((serial_in(UART_LSR) & UART_LSR_THRE) == 0)
		;

	serial_out(UART_TX, c);

	return 1;
}




/*void dbgPrint(char * s){
    
    asm("move $a0,%0\n"
        "li $v0 , 4\n"
        "syscall\n"
        : 
        : "r" (s)
        : "v0", "a0","memory" );
    
}



void prom_putchar(char c)
{
    char s[2];
    s[0] = c;
    s[1] = 0;
    dbgPrint(&s[0]);
}*/




/*
void dbgPrintI(int i){
    
    char s[2];
    char c;
    
    if(i < 0){
        s[0] = '-';
        s[1] = 0;
        dbgPrint(&s[0]);
        i = -i;
    }
    
    do {
        c = '0' + i%10;
        i = i / 10;
        s[0] = c;
        s[1] = 0;
        dbgPrint(&s[0]);
    } while(i);
}


*/
