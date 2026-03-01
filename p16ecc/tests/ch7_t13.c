void foo(char x)
{
    int y;

    do { y++;
    } while ( --x );
    if ( --x ) y = 0;
}