#include <stdio.h>

int main()
{
    int *p = malloc(sizeof(int));
    *p = 10;
    printf("%d\n", *p);

    free(*p);
    return 0;
}
