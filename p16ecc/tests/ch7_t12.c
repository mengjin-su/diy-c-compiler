int func();

void foo()
{
    int x;

    x = func();

    if ( func() == 0 ) x++;
}