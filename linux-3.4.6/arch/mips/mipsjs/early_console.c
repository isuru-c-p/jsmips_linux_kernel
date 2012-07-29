#include <linux/init.h>





void dbgPrint(char * s){
    
    asm("move $a0,%0\n"
        "li $v0 , 4\n"
        "syscall\n"
        : 
        : "r" (s)
        : "v0", "a0","memory" );
    
}



void __init prom_putchar(char c)
{
    char s[2];
    s[0] = c;
    s[1] = 0;
    dbgPrint(&s[0]);
}


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
