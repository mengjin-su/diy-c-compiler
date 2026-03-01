void delay(int n)
{
    while ( n )
    {
        if ( n > 1000 )
            break;
        n--;
    }
}