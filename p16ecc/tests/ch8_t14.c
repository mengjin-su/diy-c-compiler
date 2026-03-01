int x, y;

void foo()
{
    x += 0x0100;
    x += 0xff00;
    x |= 0x0180;
    x ^= 0xff00;
    x &= y;
}