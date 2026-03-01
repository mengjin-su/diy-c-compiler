long *p;
int  n, m;

void foo()
{
    *p += 0xff000000;
    n += 1;
    m -= 0xff00;
}