#include ".\LIB\rev_STDIO.h"
#include <stdio.h>
#include "lib_OV7670.c"
int main(void)
{
    rev_STDIO_set();
    printf("START Setting\n");
    setup();
    printf("START\n");
    while(1)
    {
        printf("START %.2X\n",PIND);
    }
}
