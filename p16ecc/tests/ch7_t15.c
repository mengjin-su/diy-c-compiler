int foo(int *p)
{
    char x;
    *(int*)x = *p;
    p[5] = x;
}