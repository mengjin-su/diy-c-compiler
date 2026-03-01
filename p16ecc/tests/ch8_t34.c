short x, y;

void foo()
{
    x = y | 0x01ff00;
    x = y & 0x01ff00;
    x = y ^ 0x00ff01;
}