
#include <stdio.h>
#ifdef __cplusplus
extern "C"
{
#endif
    int sum(int a, int b);
#ifdef __cplusplus
}
#endif

int main()
{

    const int a = 20;
    int *p      = (int *)&a;
    *p          = 30;
    printf("%d,%d,%d \n", a, *p, *(&a));
    printf("%d \n", sum(a, *p));
    return 0;
}
