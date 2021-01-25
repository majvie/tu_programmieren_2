#include <stdio.h>
#include <stdlib.h>
#include "duma.h"

int main()
{
    char *p;
    printf("Hello world!\n");
    p=malloc(17);
    free(p);
    return 0;
}
