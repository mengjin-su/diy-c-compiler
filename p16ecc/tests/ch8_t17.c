int (*fptr)(int x, char y);

void foo()
{
    fptr(1000, 200);
}