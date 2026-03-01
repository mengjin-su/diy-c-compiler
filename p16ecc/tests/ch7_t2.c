void foo(int n)
{
    int x;
    switch ( n )
    {
    case 1: x++; break;
    case 2: x--; break;
    default: x = 0;
    }

    x = (n)? x+1: x+2;
}