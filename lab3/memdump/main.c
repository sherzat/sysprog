#define _GNU_SOURCE //needed for 'asprintf' which is a GNU extension

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <unistd.h>

#include "memdump.h"

int main( int argc, char *argv[] )
{
    int first_var = 0xaaaaaaaa; // do not change this declaration!!!
    int var2 = 0xbbccddee;
    int var3 = 0xffffffff;
    long size;
    void  *start_addr;
    pid_t pid = getpid();
    char* str;
    int var4 = 0x11223344;

    asprintf(&str, "cat /proc/%d/maps", pid);
    printf("\n\nstr = %s\n\n", str);
    
    system(str);
    free(str);
    printf("\n\n");
    
    printf("FYI: address of main function in memory: %p\n", main );
    printf("FYI: address of the first declared local variable of main(): %p\n", &first_var );
    printf("FYI: address of the second declared local variable of main(): %p\n", &var2 );
    printf("FYI: address of the third declared local variable of main(): %p\n", &var3 );    
    printf("FYI: address of the last declared local variable of main(): %p\n", &var4 );  
    
    printf("\nEnter start address (hex-notation) of dump: ");
    scanf("%p", &start_addr);
    printf("\nEnter number of bytes to dump (negative or positive value): ");
    scanf("%20ld", &size);

    memdump( (byte_t *)start_addr, size );
    
    return 0;
}








